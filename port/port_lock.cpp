
#include <mutex>


/*!
 * User implementation of lock.
 */


static std::mutex pool_allocator_lock;

extern "C" void pa_lock() {
    pool_allocator_lock.lock();
}
extern "C" void pa_unlock() {
    pool_allocator_lock.unlock();
}
