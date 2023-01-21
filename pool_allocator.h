#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H



#include <stddef.h>
#include <stdint.h>
#include <stdalign.h>

#include "pool_allocator_config.h"


#define PA_BLOCKS_COUNT  (PA_CONFIG_POOL_SIZE / PA_CONFIG_BLOCK_SIZE)


#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Allocator with fixed size and block size.
 *
 *
 * There is a requirement that allocator must work with C and C++ code.
 * I have chosen C language for allocator because it is easier to meet this requiremet.
 * C functions easier to connect to CPP code than CPP functions to C code in my opinion.
 * Also there can be only C project (and more likely) for microcontroller for example.
 *
 *
 * For full usage of CPP functionality wrapper can be written (see pool_allocator_cpp.h).
 *
 *
 * Allocator is global just because no requirements set in specification about this. It makes code more simple.
 * In microcontroller project developer will need to make global heap likely.
 *
 *
 * Chunk size must be multiply of alignof(std::max_align_t).
 * Returned blocks should be aligned for portability. I have chosen simple approach to set restrictions to block size.
 * Another approach is to make block bigger and adjust pointer to needed alignment. I have chosen this approach because it is easier and makes code more simple.
 * No requirements set in specification.
 *
 *
 * Lock is user-defined because it is unknown what objects user OS will provide.
 *
 * User must provide functions pa_lock for aquiring lock and pa_unlock for releasing lock. Calls must be blocking.
 *
 * During compilation user defines allocator size and block size. Definitions are in file pool_allocator_config.h.
 *
 *
 * Allocation and deallocation have O(1) complexity.
 *
 */

/*!
 * \brief pa_lock
 *
 * Interface for aquiring lock for user.
 * User must provide this function.
 * \warning Call must be blocking.
 */
void pa_lock();

/*!
 * \brief pa_unlock
 *
 * Interface for releasing lock for user.
 * User must provide this function.
 * \warning Call must be blocking.
 */
void pa_unlock();


/*!
 * \brief pa_init
 *
 * Initializes allocator.
 * Works also as full reset.
 *
 * \warning Not thread-safe.
 * \warning Call interpreted as reset. All user pointers after call become invalidated.
 */

void pa_init(void);


/*!
 * \brief pa_allocate
 * \note Thread-safe.
 * \param size Size of block in bytes.
 * \return Pointer to block if success, NULL if failed (size is too large or allocator is full).
 */
void *pa_allocate(size_t size);

/*!
 * \brief pa_allocate_block
 * \note Thread-safe.
 * \return Pointer to block if success, NULL if failed.
 */
void *pa_allocate_block(void);

/*!
 * \brief pa_deallocate
 * \param ptr Pointer to block to deallocate.
 */
void pa_deallocate(void *ptr);


#ifdef __cplusplus
}
#endif

#endif // POOL_ALLOCATOR_H
