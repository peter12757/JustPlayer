//
// Created by Peter Xi on 2022/6/1.
//

#ifndef ANDROIDTEST_LOCK_H
#define ANDROIDTEST_LOCK_H

#include "IntTypes.h"
#ifdef WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

namespace Lock {
#ifdef _WIN32
    class MediaMutexLock
    {
     public:
      MediaMutexLock();
      ~MediaMutexLock();

      void Lock(int lastlockid = 0);
      void Unlock();

      // return count of recusive locked.
      int GetLockCnt() const;

     private:
      MediaMutexLock(const MediaMutexLock& other);
      MediaMutexLock& operator=(const MediaMutexLock& other);

      CRITICAL_SECTION iHandle;
      int iLockCnt;
      int m_lastLockId;
    };

    /// synchronization

    /// recusive mutex.
    class MediaMutex
    {
     public:
      MediaMutex();
      ~MediaMutex();

      void Lock();
      void Unlock();

     private:
      MediaMutex(const MediaMutex& other);
      MediaMutex& operator=(const MediaMutex& other);

      CRITICAL_SECTION m_mutex;
    };

    class MediaEvent
    {
     public:
      // the event is of state reset by default.
      MediaEvent();
      ~MediaEvent();

      /// timeoutMS - reserved. always return true.
      bool Wait(unsigned int timeoutMS);
      bool Wait();

      /// release all threads that are waiting on this event.
      /// and then event is reset autoly.
      void Signal();

     private:
      MediaEvent(const MediaEvent& other);
      MediaEvent& operator=(const MediaEvent& other);

      HANDLE m_event;
      CRITICAL_SECTION m_mute;
    };

    class RWLock
    {
      const int MAX_READ_NUM = 7;

     public:
      RWLock();
      ~RWLock();

      void acquireLockExclusive();
      void releaseLockExclusive();

      void rcquireLockShared();
      void releaseLockShared();

     private:
      HANDLE m_readSem;
      HANDLE m_writeSem;
    };

#else

    class MediaMutexLock {
    public:
        MediaMutexLock();

        ~MediaMutexLock();

        void Lock(int lastlockid = 0);

        void Unlock();

        // return count of recusive locked.
        int GetLockCnt() const;

    private:
        MediaMutexLock(const MediaMutexLock &other);

        MediaMutexLock &operator=(const MediaMutexLock &other);

        void *iHandle;
        int iLockCnt;
        int m_lastLockId;
    };

/// synchronization

/// recusive mutex.
    class MediaMutex {
    public:
        MediaMutex();

        ~MediaMutex();

        void Lock();

        void Unlock();

    private:
        MediaMutex(const MediaMutex &other);

        MediaMutex &operator=(const MediaMutex &other);

        pthread_mutex_t m_mutex;
    };

    class MediaEvent {
    public:
        // the event is of state reset by default.
        MediaEvent();

        ~MediaEvent();

        /// timeoutMS - reserved. always return true.
        bool Wait(unsigned int timeoutMS);

        bool Wait();

        /// release all threads that are waiting on this event.
        /// and then event is reset autoly.
        void Signal();

    private:
        MediaEvent(const MediaEvent &other);

        MediaEvent &operator=(const MediaEvent &other);

        void *iHandle;
    };

    class RWLock {
    public:
        RWLock();

        ~RWLock();

        void acquireLockExclusive();

        void releaseLockExclusive();

        void rcquireLockShared();

        void releaseLockShared();

    private:
        pthread_rwlock_t m_lock;
    };

#endif

    class MutexStackLock {
    public:
        explicit MutexStackLock(MediaMutex &mutex);

        ~MutexStackLock();

    private:
        MutexStackLock &operator=(const MutexStackLock &other);

        MutexStackLock(const MutexStackLock &other);

        MediaMutex &iMutex;
    };

    class ReadLockScoped {
    public:
        explicit ReadLockScoped(RWLock &rwLock) : m_rwLock(rwLock) { m_rwLock.rcquireLockShared(); }

        ~ReadLockScoped() { m_rwLock.releaseLockShared(); }

    private:
        RWLock &m_rwLock;
    };

    class WriteLockScoped {
    public:
        explicit WriteLockScoped(RWLock &rwLock) : m_rwLock(
                rwLock) { m_rwLock.acquireLockExclusive(); }

        ~WriteLockScoped() { m_rwLock.releaseLockExclusive(); }

    private:
        RWLock &m_rwLock;
    };

    class ReferenceCalc // 引用计数器
    {
    public:
        ReferenceCalc(MediaMutex &mutex, uint32_t &count);

        ~ReferenceCalc();

    private:
        MediaMutex &m_mutex;
        uint32_t &m_count;
    };
}


#endif //ANDROIDTEST_LOCK_H
