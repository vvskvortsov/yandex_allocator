#include <stdbool.h>
#include <assert.h>

#include "pool_allocator.h"


static_assert(PA_CONFIG_BLOCK_SIZE % alignof(max_align_t) == 0,
              "Chunk size must be multiply of alignof(std::max_align_t)");

static_assert(PA_CONFIG_POOL_SIZE > PA_CONFIG_BLOCK_SIZE,
              "Chunk size must be more than size of allocator");



static alignas(alignof(max_align_t))    uint8_t  pa_pool[PA_CONFIG_POOL_SIZE] = {0};


static size_t pa_first_free_block = 0;
static size_t pa_last_free_block = 0;
static bool pa_no_free_blocks = false;
static size_t pa_next_free_block[PA_BLOCKS_COUNT];

void pa_init(void)
{
    for (size_t i = 0; i < PA_BLOCKS_COUNT; i++) {
        pa_next_free_block[i] = i + 1;
    }

    pa_first_free_block = 0;
    pa_last_free_block = PA_BLOCKS_COUNT - 1;
    pa_no_free_blocks = false;
}


void* pa_allocate(size_t size)
{
    if (size > PA_CONFIG_BLOCK_SIZE) {
        return NULL;
    }

    return pa_allocate_block();
}

void* pa_allocate_block(void)
{
    pa_lock();

    if (pa_no_free_blocks) {
        pa_unlock();
        return NULL;
    }

    size_t idx = pa_first_free_block;

    if (pa_first_free_block == pa_last_free_block) {
        pa_no_free_blocks = true;
    }
    else {
        pa_first_free_block = pa_next_free_block[idx];
    }

    pa_unlock();

    return (void*) &pa_pool[PA_CONFIG_BLOCK_SIZE * idx];
}

void pa_deallocate(void* ptr)
{
    pa_lock();
    size_t idx =  (size_t)(((uint8_t*)ptr - pa_pool) / PA_CONFIG_BLOCK_SIZE);

    if (pa_no_free_blocks) {
        pa_no_free_blocks = false;
        pa_first_free_block = idx;
        pa_last_free_block = idx;
    }
    else {
        pa_next_free_block[pa_last_free_block] = idx;
        pa_last_free_block = idx;
    }

    pa_unlock();
}







