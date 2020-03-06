#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "api.h"

#define STRING_SIZE 16

// Marcos to help measure the elapsed time.
#define TICK(X) double X[2] = { FLA_Clock(), 0.0 };
#define TOCK(X) X[1] = FLA_Clock();
#define PLOT(X, OPS) printf("%-30s: %g sec\t %g ns / op\n", (#X), (X[1] - X[0]), (X[1] - X[0]) / (double) (OPS) * 10e9)

// A global variable used when FLA_Clock_helper() is defined in terms of
// clock_gettime()/gettimeofday().
double gtod_ref_time_sec = 0.0;

/** Get a time in nanoseconds. Implementation from flame graphs. */
double FLA_Clock()
{
    double the_time, norm_sec;
    struct timespec ts;

    clock_gettime( CLOCK_MONOTONIC, &ts );

    if ( gtod_ref_time_sec == 0.0 )
        gtod_ref_time_sec = ( double ) ts.tv_sec;

    norm_sec = ( double ) ts.tv_sec - gtod_ref_time_sec;

    the_time = norm_sec + ts.tv_nsec * 1.0e-9;

    return the_time;
}

#define NUM_NAMES 4
char first_names[NUM_NAMES][STRING_SIZE] = { "Tom", "Arthur", "Harry", "Felix"};
char last_names[NUM_NAMES][STRING_SIZE] = { "Bombadil", "Potter", "Jedusor", "Dumbledore" };


#define NUM_RANDS (1024 * 1024 * 128)
// array of random numbers. Those are pregenrated because generating random number is expensive
// and might impact benchmark time.
int rands[NUM_RANDS];

#define loc_from_int(i) ((float) (i % (1024)))

/** Generates randomized data to load into the database */
Person* gen(int n) {
    Person * ps = malloc(sizeof(Person) * n);
    srand(0);
    for(int i=0; i<NUM_RANDS ; i++) {
        rands[i] = rand();
    }
    for(int i=0 ; i<n ; i++) {
        int x = rands[i % NUM_RANDS];
        Person p;
        p.first_name = first_names[x % NUM_NAMES];
        p.last_name = last_names[(x >> 10) % NUM_NAMES];
        p.age = x % 128;
        p.latitude = loc_from_int(x);
        p.longitude = loc_from_int(x / 1024);
        p.male = (x & (1 << 20)) ? true : false; // true if the 20th bit is 1
        p.wizard = x % 8 != 0; // 1 wizard in 8 persons
        ps[i] = p;
    }

    return ps;
}

int main(int argc, char **argv) {
    int pow = 25;
    if (argc >= 2) {
        pow = atoi(argv[1]);
    }
    printf("\n--------- %s %d ---------\n", argv[0], pow);
    uint64_t len = 1UL << pow;
    if(len < 64) {
        printf("Common, ask for something bigger please...\n");
        return 1;
    }
    printf("Num elements: %lu\n", len);

    uint64_t data_size = len * sizeof(Person);
    if (data_size > 4UL * 1024UL * 1024UL * 1024UL) {
        printf("More that 4GB requested... be reasonable please.\n");
        return 1;
    }
    if (data_size > 1024 * 1024) {
        printf("Total data size: %lu Mo\n", data_size / (1024UL * 1024UL) );
    } else {
        printf("Total data size: %lu Ko\n", data_size / (1024UL) );
    }


    TICK(generation);
    Person* ps = gen(len);
    TOCK(generation);
    PLOT(generation, len);

    TICK(db_initialisation);
    DB *db = db_init(ps, len);
    TOCK(db_initialisation);
    PLOT(db_initialisation, len);

    TICK(count_male);
    int cnt = db_count_male(db);
    TOCK(count_male);
    PLOT(count_male, len);


    TICK(max_age);
    int age = db_max_age(db);
    TOCK(max_age);
    PLOT(max_age, len);


    TICK(oldest);
    pid oldest_person = db_oldest(db);
    TOCK(oldest);
    PLOT(oldest, len);

    uint64_t num_access = 1024 * 1024 * 10;

    // uncounted sequential access for warm up
    for(uint32_t i = 0 ; i < num_access ; i++) {
        db_get(db, i & (len -1));
    }

    // go through the entire data base in sequence
    TICK(sequential_access);
    for(uint32_t i = 0 ; i < num_access ; i++) {
        db_get(db, i & (len -1));
    }
    TOCK(sequential_access);
    PLOT(sequential_access, num_access);

    // look up random
    TICK(random_access);
    for(uint32_t i = 0 ; i < num_access ; i++) {
        db_get(db, rands[i & (NUM_RANDS-1)] & (len -1));
    }
    TOCK(random_access);
    PLOT(random_access, num_access);


    TICK(closest);
    pid id_closest = db_closest(db, loc_from_int(rand()), loc_from_int(rand()));
    TOCK(closest);
    PLOT(closest, len);

    TICK(query1);
    pid id_query1 = db_query1(db, loc_from_int(rand()), loc_from_int(rand()));
    TOCK(query1);
    PLOT(query1, len);

    printf("(count_male: %d) (max-age: %d) (oldest: %ld) (closest: %ld) (query1: %ld)\n", cnt, age, oldest_person, id_closest, id_query1);
    return 0;
}
