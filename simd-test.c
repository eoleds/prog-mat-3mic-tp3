#include "simd.h"


int main() {



    __m256i zeros = _mm256_set1_epi32(0);
    print_vec_epu32("zeros", zeros);
    __m256i ones = _mm256_set1_epi32(1);
    print_vec_epu32("ones", ones);

    __m256 fours = _mm256_set1_ps(4.0f);
    print_vec("fours", fours);

    __m256 nums = _mm256_setr_ps(0, 1, 2, 3, 4, 5, 6, 7);
    print_vec("nums", nums);

    // compare two vectors and create a mask representing the result of the comparison
    __m256i lt_four = (__m256i) _mm256_cmp_ps(fours, nums, 1);
    print_vec_mask("mask", lt_four);

    // create a new vector whose value is taken either from "zeros" or from "ones" depending on the result of the comparison
    __m256i zero_ones = _mm256_blendv_epi8(zeros, ones, lt_four);
    print_vec_epu32("zero_ones", zero_ones);
    


    return 0;
}