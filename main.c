#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "api.h"



int main(int argc, char **argv) {
    int N = 25; // N default value = 25
    if (argc >= 2) {
        N = atoi(argv[1]); // read N value from arguments
    }
    printf("\n--------- %s %d ---------\n", argv[0], N);
    uint64_t num_elements = 1UL << N; // num_elems = 2^N

    // exit if we didn't get at least 64 elements
    if(num_elements < 64) {
        printf("Common, ask for something bigger please...\n");
        return 1;
    }
    printf("Num elements: %lu\n", num_elements);


    uint64_t database_size = num_elements * sizeof(Person);

    // exit if we requested to much to avoid consuming all RAM.
    if (database_size > 4UL * 1024UL * 1024UL * 1024UL) {
        printf("More that 4GB requested... be reasonable please.\n");
        return 1;
    }

    // print data size
    if (database_size > 1024 * 1024) {
        printf("Total data size: %lu Mo\n", database_size / (1024UL * 1024UL) );
    } else {
        printf("Total data size: %lu Ko\n", database_size / (1024UL) );
    }

    Person* ps = generate_random_persons(num_elements);

    DB *db = db_init(ps, num_elements);

    // set the number of accesses to measure to 10M
    uint64_t num_access = 1024 * 1024 * 10;

    // go through the entire database in sequence
    START_CHRONO(sequential_access);
    for(uint32_t i = 0 ; i < num_access ; i++) {
        db_get(db, i % num_elements);
    }
    STOP_CHRONO(sequential_access, num_access);

    // look up random
    START_CHRONO(random_access);
    for(uint32_t i = 0 ; i < num_access ; i++) {
        db_get(db, RANDOM_NUMBER(i) % num_elements);
    }
    STOP_CHRONO(random_access, num_access);

    int cnt;
    START_CHRONO(count_male);
    cnt = db_count_male(db);
    STOP_CHRONO(count_male, num_elements);

    int age;
    START_CHRONO(max_age);
    age = db_max_age(db);
    STOP_CHRONO(max_age, num_elements);

    pid oldest_person;
    START_CHRONO(oldest);
    oldest_person = db_oldest(db);
    STOP_CHRONO(oldest, num_elements);

    pid id_closest;
    START_CHRONO(closest);
    id_closest = db_closest(db, loc_from_int(rand()), loc_from_int(rand()));
    STOP_CHRONO(closest, num_elements);

    pid id_query1;
    START_CHRONO(query1);
    id_query1 = db_query1(db, loc_from_int(rand()), loc_from_int(rand()));
    STOP_CHRONO(query1, num_elements);

    printf("(count_male: %d) (max-age: %d) (oldest: %ld) (closest: %ld) (query1: %ld)\n", cnt, age, oldest_person, id_closest, id_query1);

    printf("Execution sucessfull\n");
    return 0;
}
