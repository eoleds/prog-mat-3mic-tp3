#include "api.h"

typedef struct DB {
    char **first_names;
    char **last_names;
    uint32_t *age;
    char *male;
    char *wizard;
    float *latitude;
    float *longitude;
    int len;
} DB;


DB* db_init(Person *persons, int len) {
    DB * db = malloc(sizeof(DB));
    db->first_names = malloc(sizeof(char *) * len);
    db->last_names = malloc(sizeof(char *) * len);
    db->age = malloc(sizeof(uint32_t) * len);
    db->male = malloc(sizeof(char) * len);
    db->wizard = malloc(sizeof(char) * len);
    db->latitude = malloc(sizeof(float) * len);
    db->longitude = malloc(sizeof(float) * len);
    db->len = len;
    for(int i = 0 ; i<len ; i++) {
        db->first_names[i] = persons[i].first_name;
        db->last_names[i] = persons[i].last_name;
        db->age[i] = persons[i].age;
        db->male[i] = persons[i].male;
        db->wizard[i] = persons[i].wizard;
        db->latitude[i] = persons[i].latitude;
        db->longitude[i] = persons[i].longitude;
    }
    return db;
}

void db_free(DB * db) {
    free(db->first_names);
    free(db->last_names);
    free(db->age);
    free(db->male);
    free(db->wizard);
    free(db->latitude);
    free(db->longitude);
    free(db);
}

Person db_get(DB *db, pid i) {
    Person p = {
            .first_name = db->first_names[i],
            .last_name = db->last_names[i],
            .age = db->age[i],
            .male = db->male[i],
            .wizard = db->wizard[i],
            .latitude = db->latitude[i],
            .longitude = db->longitude[i]
    };
    return p;
}

int db_count_male(DB *db) {
    return -1;
}

/** Returns the age of the oldest person. */
uint32_t db_max_age(DB * db) {
    return 0;
}

/** Returns the ID of the oldest person. If multiple person have the same age, this should return the one with the highest ID. */
pid db_oldest(DB * db) {
    return -1;
}

pid db_closest(DB *db, float lat, float lon) {
    return -1;
}

pid db_query1(DB *db, float lat, float lon) {
    return -1;
}