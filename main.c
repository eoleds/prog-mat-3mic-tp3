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


    START_CHRONO(generation);
    Person* ps = generate_random_persons(num_elements);
    END_CHRONO(generation);
    PRINT_CHRONO(generation, num_elements);

    START_CHRONO(db_initialisation);
    DB *db = db_init(ps, num_elements);
    END_CHRONO(db_initialisation);
    PRINT_CHRONO(db_initialisation, num_elements);

    // set the number of accesses to measure to 10M
    uint64_t num_access = 1024 * 1024 * 1000;

    // go through the entire database in sequence
    START_CHRONO(sequential_access);
    for(uint32_t i = 0 ; i < num_access ; i++) {
        db_get(db, i % num_elements);
    }
    END_CHRONO(sequential_access);
    PRINT_CHRONO(sequential_access, num_access);

    // look up random
    START_CHRONO(random_access);
    for(uint32_t i = 0 ; i < num_access ; i++) {
        db_get(db, RANDOM_NUMBER(i) % num_elements);
    }
    END_CHRONO(random_access);
    PRINT_CHRONO(random_access, num_access);


    START_CHRONO(count_male);
    int cnt = db_count_male(db);
    END_CHRONO(count_male);
    PRINT_CHRONO(count_male, num_elements);


    START_CHRONO(max_age);
    int age = db_max_age(db);
    END_CHRONO(max_age);
    PRINT_CHRONO(max_age, num_elements);


    START_CHRONO(oldest);
    pid oldest_person = db_oldest(db);
    END_CHRONO(oldest);
    PRINT_CHRONO(oldest, num_elements);


    START_CHRONO(closest);
    pid id_closest = db_closest(db, loc_from_int(rand()), loc_from_int(rand()));
    END_CHRONO(closest);
    PRINT_CHRONO(closest, num_elements);

    START_CHRONO(query1);
    pid id_query1 = db_query1(db, loc_from_int(rand()), loc_from_int(rand()));
    END_CHRONO(query1);
    PRINT_CHRONO(query1, num_elements);

    printf("(count_male: %d) (max-age: %d) (oldest: %ld) (closest: %ld) (query1: %ld)\n", cnt, age, oldest_person, id_closest, id_query1);
    return 0;
}
