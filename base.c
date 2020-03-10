#include "api.h"


typedef struct DB {
    Person * persons;
    int size;
} DB;

/** Creates a new Database from an array of Persons */
DB* db_init(Person *persons_array, int num_elements) {
    DB * db = malloc(sizeof(DB));
    db->size = num_elements;
    db->persons = malloc(num_elements * sizeof(Person));
    for(int i = 0 ; i<num_elements ; i++) {
        db->persons[i] = persons_array[i];
    }
    return db;
}

/** Retruns the Person with the given id */
Person db_get(DB *db, pid person_id) {
    return db->persons[person_id];
}

/** Count the number of males in the database */
pid db_count_male(DB *db) {
    Person *ps = db->persons;
    int size = db->size;

    int cnt = 0;
    for(int i=0 ; i < size ; i++) {
        if(ps[i].male == 1)
            cnt += 1;
    }
    return cnt;
}

/** Returns the age of the oldest person. */
uint32_t db_max_age(DB * db) {
    Person *ps = db->persons;
    int size = db->size;

    uint32_t max = 0;
    for(int i=0 ; i < size ; i++) {
        if(ps[i].age > max)
            max = ps[i].age;
    }
    return max;
}

/** Returns the ID of the oldest person. If multiple person have the same age, this should return the one with the highest ID. */
pid db_oldest(DB * db) {
    Person *ps = db->persons;
    int size = db->size;

    uint32_t max = 0;
    pid oldest = -1;
    for(int i=0 ; i < size ; i++) {
        if(ps[i].age >= max) {
            max = ps[i].age;
            oldest = i;
        }

    }
    return oldest;

}



/** Find the closest person to the given coordinates */
pid db_closest(DB *db, float lat, float lon) {
    float min_dist = 1.0 / 0.0; // infinity
    int closest = -1;
    int size = db->size;
    for(int i = 0 ; i<size ; i++) {
        float dx = lat - db->persons[i].latitude;
        float dy = lon - db->persons[i].longitude;
        float dist = sqrtf(dx * dx + dy * dy);
        if(dist < min_dist) {
            min_dist = dist;
            closest = i;
        }
    }
    return closest;
}

/** Find the closest male muggle to the indicated point */
pid db_query1(DB *db, float lat, float lon) {
    float min_dist = 1.0 / 0.0; // infinity
    int closest = -1;
    int size = db->size;
    Person *ps = db->persons;
    for(int i = 0 ; i<size ; i++) {
        if(ps[i].male != 1)
            continue;

        if(ps[i].wizard == 0)
            continue;

        float dx = lat - ps[i].latitude;
        float dy = lon - ps[i].longitude;
        float dist = sqrtf(dx * dx + dy * dy);
        if(dist < min_dist) {
            min_dist = dist;
            closest = i;
        }
    }
    return closest;
}