#ifndef SIMD_H
#define SIMD_H

#include <immintrin.h>  // for simd intrinsics
#include <stdio.h>      // for printf
#include <stdint.h>     // for uint32_t

// Extract the index-th value in a SIMD 256bits vector (8 floats)
// index should be a value in [0,7]
float extract_float(__m256 float_vector, int index) {
    return float_vector[index];
}

// Extract the index-th 32bits value in a SIMD 256bits vector (8 uint32_t)
// index should be a value in [0, 7]
uint32_t extract_uint32(__m256i epu32_vector, int index) {
    switch (index)
    {
        case 0: return _mm256_extract_epi32(epu32_vector, 0);
        case 1: return _mm256_extract_epi32(epu32_vector, 1);
        case 2: return _mm256_extract_epi32(epu32_vector, 2);
        case 3: return _mm256_extract_epi32(epu32_vector, 3);
        case 4: return _mm256_extract_epi32(epu32_vector, 4);
        case 5: return _mm256_extract_epi32(epu32_vector, 5);
        case 6: return _mm256_extract_epi32(epu32_vector, 6);
        case 7: return _mm256_extract_epi32(epu32_vector, 7);
        default: exit(1);
    }
}

// Extract the smallest float in a SIMD 256bits vector. 
float min_float_in_vector(__m256 floats) {
    float min = extract_float(floats, 0);
    for(int i=1; i<8; i++) {
        float candidate = extract_float(floats, i);
        if (candidate < min) {
            min = candidate;
        }
    }
    return min;
}

// Prints the 8 float values of a SIMD 256bits vector.
// The provided label will be printed at the beginning of the line
void print_vec(char * label, __m256 vec) {
    printf("%20s: ", label);
     for(int i=0; i<8; i++) {
        printf("%10.2f  ", vec[i]);
    }
    printf("\n");
}

// Prints the 8 unsigned int values of aa SIMD 256bits vector.
// The provided label will be printed at the beginning of the line
void print_vec_epu32(char * label, __m256i vec) {
    printf("%20s: ", label);
     for(int i=0; i<8; i++) {
        printf("%10u  ", extract_uint32(vec, i));
    }
    printf("\n");
}

// Prints in hexadecimal the 8 unsigned int values of aa SIMD 256bits vector.
// The provided label will be printed at the beginning of the line
void print_vec_mask(char * label, __m256i vec) {
    printf("%20s: ", label);
     for(int i=0; i<8; i++) {
        printf("%10X  ", extract_uint32(vec, i));
    }
    printf("\n");
}

#endif