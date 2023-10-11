#include <cstdio>
#include <intrin.h>
#include <iostream>

void
print_id(const char* id)
{
    const char *name_tag = id;
    printf("name_tag: %p, id: %p\n", name_tag, id);
}

inline void
print_string(const char *str)
{
    printf("string is %s.\n", str);
}

int main() {
    // char id1[16] = "foo";
    // print_string(id1);
    // // char id2[16] = "bar";
    // id1[2] = 'b';
    // print_string(id1);
    
    // char *dr = "daa-read-only";
    // print_string(dr);
    
    // dr[2] = 'Y';
    // print_string(dr);
    
    
    // print_id(id1);
    // // print_id(id2);
    
    // Intrinsics. Getting the difference between _mm_cvtps_epi32 and _mm_castps_si28
    __m128 floatVec = _mm_set_ps(3.5f, 2.4f, 1.7f, 0.9f);
    std::cout << "Original floatVec: \n";
    for(int i = 0; i < 4; ++i)
    {
        float *val = reinterpret_cast<float *>(&floatVec) + i;
        std::cout << *val << " ";
    }
    std::cout << "\n";

    // Using _mm_cvtps_epi32 to convert float vector to integer vector with
    // rounding. Output: 1 2 2 4
    //
    __m128i intVec1 = _mm_cvtps_epi32(floatVec);
    int *intArray1  = reinterpret_cast<int *>(&intVec1);
    std::cout << "Using _mm_cvtps_epi32:\n";
    for (int i = 0; i < 4; i++)
    {
        std::cout << intArray1[i] << " ";
    }
    std::cout << "\n";
    
    // Using _mm_castps_si128 to reinterpret the bits of float vector as integer
    // vector. Output: 1063675494 1071225242 1075419546 1080033280
    //
    // As you can see the float bit representation in the CPU are interpreted as
    // integer bits. thats why the result of the integers is very large.
    //
    __m128i intVec2 = _mm_castps_si128(floatVec);
    int *intArray2  = reinterpret_cast<int *>(&intVec2);
    std::cout << "Using _mm_castps_si128:\n";
    for (int i = 0; i < 4; i++)
    {
        std::cout << intArray2[i] << " ";
    }
    std::cout << "\n";

    __m128i test1 = _mm_set_epi32(1, 2, 3, 4);
    __m128i test2 = _mm_setr_epi32(1, 2, 3, 4);
    __m128i res12 = _mm_add_epi32(test1, test2);
    
    return 0;
}
