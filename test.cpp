#include "CppUTest/TestHarness.h"

#include "pool_allocator.h"
#include <stdalign.h>


TEST_GROUP(AllocatorGroup)
{
    void setup() {
        pa_init();
    }
};

TEST(AllocatorGroup, AllocationReturnsValidPointer)
{
    uint64_t* p = (uint64_t*)(pa_allocate(sizeof(uint64_t)));

    CHECK(p != NULL);
}


TEST(AllocatorGroup, AllocationReturnsNullPointerIfRequestedSizeIsBiggerThanSizeBlock)
{
    uint64_t* p = static_cast<uint64_t*>(pa_allocate(sizeof(uint64_t) + PA_CONFIG_BLOCK_SIZE));

    CHECK(p == NULL);
}


TEST(AllocatorGroup, AllocationReturnsNullPointerIfAllocatorIsFull)
{
    size_t max_allocations_count = PA_BLOCKS_COUNT;
    uint64_t* ptr[PA_BLOCKS_COUNT];

    for (size_t i = 0; i < max_allocations_count; i++) {
        ptr[i] = static_cast<uint64_t*>(pa_allocate(sizeof(uint64_t)));
    }

    uint64_t* ptr_error = static_cast<uint64_t*>(pa_allocate(sizeof(uint64_t)));


    for (size_t i = 0; i < max_allocations_count; i++) {
        CHECK(ptr[i] != NULL);
    }

    CHECK(ptr_error == NULL);
}

TEST(AllocatorGroup, AllocationReturnsValidPointerAfterAllocatorWasFullAndSomeBlocksWereFreed)
{
    size_t max_allocations_count = PA_BLOCKS_COUNT;

    uint64_t* ptr[PA_BLOCKS_COUNT];

    for (size_t i = 0; i < max_allocations_count; i++) {
        ptr[i] = static_cast<uint64_t*>(pa_allocate(sizeof(uint64_t)));
    }

    pa_deallocate(ptr[0]);

    uint64_t* ptr_new = static_cast<uint64_t*>(pa_allocate(sizeof(uint64_t)));

    CHECK(ptr_new != NULL);
}

void f_test1(void)
{

}
void f_test2(void)
{

}
TEST(AllocatorGroup, VariablesGetByAllocatorSaveItsValuesWhileMakingOperationsOnToThem)
{
    size_t max_allocations_count = PA_BLOCKS_COUNT;

    const uint32_t TESTED_TYPES_COUNT = 4;


    int64_t* int_ptr[PA_BLOCKS_COUNT / TESTED_TYPES_COUNT];
    double*  d_ptr[PA_BLOCKS_COUNT / TESTED_TYPES_COUNT];
    uint8_t* (ch_array_ptr)[PA_BLOCKS_COUNT / TESTED_TYPES_COUNT];
    void (*f_ptr[PA_BLOCKS_COUNT / TESTED_TYPES_COUNT])(void) ;


    for (size_t i = 0; i < max_allocations_count / TESTED_TYPES_COUNT; i++) {
        int_ptr[i] = static_cast<int64_t*>(pa_allocate(sizeof(int64_t)));
        d_ptr[i] = static_cast<double*>(pa_allocate(sizeof(double)));
        ch_array_ptr[i] = static_cast<uint8_t*>(pa_allocate(sizeof(uint8_t [PA_CONFIG_BLOCK_SIZE])));
        f_ptr[i] = reinterpret_cast<void (*)(void)>(pa_allocate(sizeof(void (*)(void))));
    }


    for (size_t i = 0; i < max_allocations_count / TESTED_TYPES_COUNT; i++) {
        *int_ptr[i] = 123;
        *d_ptr[i] = 345.5;
        ch_array_ptr[i][0] = 11;
        ch_array_ptr[i][PA_CONFIG_BLOCK_SIZE - 1] = 22;
        f_ptr[i] = f_test1;
    }

    for (size_t i = 0; i < max_allocations_count / TESTED_TYPES_COUNT; i++) {
        CHECK(*int_ptr[i] == 123);
        DOUBLES_EQUAL(*d_ptr[i], 345.5, 2.0);
        CHECK(ch_array_ptr[i][0] == 11);
        CHECK(ch_array_ptr[i][PA_CONFIG_BLOCK_SIZE - 1] == 22);
        CHECK(f_ptr[i] == f_test1);
    }


    for (size_t i = 0; i < max_allocations_count / TESTED_TYPES_COUNT; i++) {
        *int_ptr[i] = 333;
        *d_ptr[i] = 31345.5;
        ch_array_ptr[i][0] = 135;
        ch_array_ptr[i][PA_CONFIG_BLOCK_SIZE - 1] = 44;
        f_ptr[i] = f_test2;
    }

    for (size_t i = 0; i < max_allocations_count / TESTED_TYPES_COUNT; i++) {
        CHECK(*int_ptr[i] == 333);
        DOUBLES_EQUAL(*d_ptr[i], 31345.5, 2.0);
        CHECK(ch_array_ptr[i][0] == 135);
        CHECK(ch_array_ptr[i][PA_CONFIG_BLOCK_SIZE - 1] == 44);
        CHECK(f_ptr[i] == f_test2);
    }


}


template <typename T>
void AllocatorGroup_make_alignment_check(T* ptr)
{
    CHECK_TEXT(reinterpret_cast<uintptr_t>(ptr) % alignof(T) == 0, std::string("type " + std::string(typeid(T).name()) + " alignment failed.").c_str());
}

TEST(AllocatorGroup, VariablesAlignmentIsValid)
{
    const size_t max_allocations_count = PA_BLOCKS_COUNT;

    const uint32_t TESTED_TYPES_COUNT = 9;

    int16_t* i16_ptr[PA_BLOCKS_COUNT / TESTED_TYPES_COUNT];
    int32_t* i32_ptr[PA_BLOCKS_COUNT / TESTED_TYPES_COUNT];
    int64_t* i64_ptr[PA_BLOCKS_COUNT / TESTED_TYPES_COUNT];

    float* f_ptr[PA_BLOCKS_COUNT / TESTED_TYPES_COUNT];
    double*  d_ptr[PA_BLOCKS_COUNT / TESTED_TYPES_COUNT];
    long double*  ld_ptr[PA_BLOCKS_COUNT / TESTED_TYPES_COUNT];


    uint8_t* (ch_array_ptr)[PA_BLOCKS_COUNT / TESTED_TYPES_COUNT];
    uintptr_t ptr[PA_BLOCKS_COUNT / TESTED_TYPES_COUNT];
    void (*func_ptr[PA_BLOCKS_COUNT / TESTED_TYPES_COUNT])(void) ;





    for (size_t i = 0; i < max_allocations_count / TESTED_TYPES_COUNT; i++) {

        i16_ptr[i] = static_cast<int16_t*>(pa_allocate(sizeof(int16_t)));
        i32_ptr[i] = static_cast<int32_t*>(pa_allocate(sizeof(int32_t)));
        i64_ptr[i] = static_cast<int64_t*>(pa_allocate(sizeof(int64_t)));

        f_ptr[i] = static_cast<float*>(pa_allocate(sizeof(float)));
        d_ptr[i] = static_cast<double*>(pa_allocate(sizeof(double)));
        ld_ptr[i] = static_cast<long double*>(pa_allocate(sizeof(long double)));

        ch_array_ptr[i] = static_cast<uint8_t*>(pa_allocate(sizeof(uint8_t [PA_CONFIG_BLOCK_SIZE])));

        ptr[i] = reinterpret_cast<uintptr_t>(pa_allocate(sizeof(uintptr_t)));

        func_ptr[i] = reinterpret_cast<void (*)(void)>(pa_allocate(sizeof(void (*)(void))));
    }


    for (size_t i = 0; i < max_allocations_count / TESTED_TYPES_COUNT; i++) {
        AllocatorGroup_make_alignment_check<int16_t>(i16_ptr[i]);
        AllocatorGroup_make_alignment_check<int32_t>(i32_ptr[i]);
        AllocatorGroup_make_alignment_check<int64_t>(i64_ptr[i]);

        AllocatorGroup_make_alignment_check<float>(f_ptr[i]);
        AllocatorGroup_make_alignment_check<double>(d_ptr[i]);
        AllocatorGroup_make_alignment_check<long double>(ld_ptr[i]);

        AllocatorGroup_make_alignment_check<uint8_t*>(ch_array_ptr);

        CHECK(ptr[i] % alignof(uintptr_t) == 0);
        CHECK(reinterpret_cast<uintptr_t>(func_ptr[i]) % alignof(void (*) (void)) == 0);
    }


}
