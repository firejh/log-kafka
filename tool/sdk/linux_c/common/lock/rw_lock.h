#ifndef _COMMON_RWLOCK_H_
#define _COMMON_RWLOCK_H_

#include <pthread.h>

namespace common{

enum RWLockPrefer
{
    RWLockReadPrefer,
    RWLockWritePrefer
};

class RWLock
{
public:
    RWLock(RWLockPrefer mode = RWLockWritePrefer)
    {
        ::pthread_rwlockattr_t attr;
        pthread_rwlockattr_init(&attr);

#ifdef __linux
        if (mode == RWLockWritePrefer)
        {
            pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
        }
        else
        {
            pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_READER_NP);
        }
#endif

        pthread_rwlock_init(&mRwLock, &attr);
    }

    ~RWLock()
    {
        pthread_rwlock_destroy(&mRwLock);
    }

public:
    inline void read_lock()
    {
        pthread_rwlock_rdlock(&mRwLock);
    }

    inline void unlock()
    {
        pthread_rwlock_unlock(&mRwLock);
    }

    inline void write_lock()
    {
        pthread_rwlock_wrlock(&mRwLock);
    }

private:
    ::pthread_rwlock_t mRwLock;
};

enum RWLockMode
{
    RWLOCK_READ,
    RWLOCK_WRITE
};

class RWLockScopeGuard
{
public:
    RWLockScopeGuard(RWLock& lock, const RWLockMode& mode) : lock_(lock)
    {
        if (mode == RWLOCK_READ)
        {
            lock_.read_lock();
        }
        else
        {
            lock_.write_lock();
        }
    }

    ~RWLockScopeGuard()
    {
        lock_.unlock();
    }

private:
    RWLock& lock_;
};

}
#endif
