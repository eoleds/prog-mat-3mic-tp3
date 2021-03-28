#ifndef API_H
#define API_H

// Some useful imports so you do not have to do this yourself
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>


typedef struct Person {
    char *first_name; // pointer to a string representing the first name
    char *last_name;  // pointer to a string representing the first name
    char male;        // 1 for male, 0 for female
    char wizard;      // 1 for wizard, 0 for muggle
    uint32_t age;     // 32 bits unsigned int representing the age
    float latitude;
    float longitude;
} Person;

/** Data structure holding the content of the database. */
typedef struct DB DB;

/** ID of a person  64bits integer. */
typedef int64_t pid ;

/** Initializes the database with the 'num_elements' persons given in the array. */
DB* db_init(Person* persons_array, int num_elements);

/** Frees all memory used by the database. */
void db_free(DB *);

/** Returns the person with the given id */
Person db_get(DB *db, pid person_id);

/** Returns the number of males in the database. */
uint32_t db_count_male(DB * db);

/** Returns the age of the oldest person. */
uint32_t db_max_age(DB * db);

/** Returns the ID of the oldest person. If multiple person have the same age, this should return the one with the highest ID. */
pid db_oldest(DB * db);

/** Returns the distance of the closest person to the given coordinates. */
float db_min_dist(DB *db, float lat, float lon);

/** Return ID of the closest person in the database. */
pid db_closest(DB *db, float lat, float lon);

/** Counts the number of female muggles in the database.  */
uint32_t db_count_female_muggles(DB *db);


/**************** Requests ****************/


typedef enum Op { GET_AGE, SET_AGE, GET_SEX, GET_DIST } Op;

typedef struct Request {
    // Request to perform.
    Op op;
    // Id of the person that is the target of the request.
    pid person_id;
    // value to write in the case of a SET request
    uint32_t value;
    // Memory location where the result should be written.
    // If the field is NULL, then the result must be discarded.
    void * result;
} Request;

void db_process_many_requests(DB * db, Request * request_array, int num_requests);
void db_process_many_requests_parallel(DB * db, Request * request_array, int num_requests, int num_threads, bool split);
uint32_t db_sum_individual_accesses(DB * db);
uint32_t db_total_accesses(DB * db);
void db_clear_accesses(DB * db);





#endif