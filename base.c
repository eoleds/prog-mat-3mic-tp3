#include "api.h"


typedef struct DB {
    // array of persons
    Person * persons;
    // number of persons in the database
    int size;
} DB;

/** Creates and populates a new Database from an array of Persons */
DB* db_init(Person *persons_array, int num_elements) {
    DB * db = malloc(sizeof(DB));
    db->size = num_elements;

    // allocate a new array to hold the persons of the database
    unsigned long array_start = (unsigned long) malloc(num_elements * sizeof(Person));
    // 2.K : Décaler le début du tableau pour forcer l'alignement
    // array_start = array_start + ???;
    db->persons = (Person *) array_start;

    // copy of all persons into the database
    for(int i = 0 ; i<num_elements ; i++) {
        db->persons[i] = persons_array[i];
    }

    // 2.J AFFICHER LES ADRESSES/DECALAGE DES DEUX PREMIERS ELEMENTS
    unsigned long address_first = (unsigned long) db->persons;
    unsigned long address_second = (unsigned long) &(db->persons[1]);

    return db;
}

/** Returns the Person with the given id */
Person db_get(DB *db, pid person_id) {
    return db->persons[person_id];
}

/** Count the number of males in the database */
uint32_t db_count_male(DB *db) {
    Person *ps = db->persons;
    int size = db->size;

    uint32_t cnt = 0;
    for(int i=0 ; i < size ; i++) {
        if(ps[i].male == 1)
            cnt += 1;
    }
    return cnt;
}

void db_free(DB * db) {
    free(db->persons);
    free(db);
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

/** Counts the number of female muggles in the database */
uint32_t db_count_female_muggles(DB *db) {
    int size = db->size;
    Person *ps = db->persons;
    
    // counter for female muggles
    uint32_t count = 0;
    
    for(int i = 0 ; i<size ; i++) {
        // 1 if the person is a wizard
        // 0 if the person is a muggle (non-wizard)
        char wizard = ps[i].wizard;

        // 1 if the person is male
        // 0 if the person is female
        char male = ps[i].male;

        // if the person is male, skip the rest of the loop
        if(male == 1)
            continue;
        
        // This was previously:
        //     if (wizard == 0) {
        //         count += 1;
        //     }
        //
        // A very clever intern told us 
        // that the result of  `wizard == 0` is
        //   - `1` if wizard has the value 0 
        //   - `0` otherwise
        //
        // Thus adding the result of `wizard == 0`
        // to the counter is equivalent to the previous
        // conditional increment

        count += (wizard == 0);  
        
    }
    return count;
}
