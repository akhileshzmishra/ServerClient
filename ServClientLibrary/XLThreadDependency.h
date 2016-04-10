/*
 * XLThreadDependency.h
 *
 *  Created on: 11-Oct-2015
 *      Author: akhilesh
 */

#ifndef XLTHREADDEPENDENCY_H_
#define XLTHREADDEPENDENCY_H_

#include "XLDependency.h"

#ifdef _C_11_COMPILER
/////////////For C++ 11////////////START
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include <mutex>
#include <condition_variable>

typedef void* THFUN_RETVAL;
typedef std::thread* ThreadHandle;
typedef std::thread::id Thread_Identifier;
typedef void* LPVOID;

#ifndef OTHER_EXPANSION
#define OTHER_EXPANSION 
#endif


/////////////For C++ 11////////////END
#else


#ifndef __UNIX__
/////////////For Windows OS////////////START

#ifndef __WINDOWS_OS_
#define __WINDOWS_OS_
#endif
#include <Windows.h>
typedef void* XLSemaphoreID;
typedef DWORD THFUN_RETVAL;

#ifndef OTHER_EXPANSION
#define OTHER_EXPANSION WINAPI
#endif

typedef int ThreadHandle;
typedef int Thread_Identifier;
//////////////////////////////////////END

#else
////////////For Linux based OS////////////START


#ifndef __LINUX_OS_
#define __LINUX_OS_
#endif

#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>


#ifndef WINAPI
#define WINAPI
#endif

typedef void* LPVOID;
typedef sem_t* XLSemaphoreID;
typedef void* THFUN_RETVAL;

#ifndef OTHER_EXPANSION
#define OTHER_EXPANSION
#endif

typedef pthread_t           ThreadHandle;
typedef pthread_t           Thread_Identifier;


////////////////////////////////////////////END

#endif //__UNIX__


#endif //_C_11_COMPILER

#endif /* XLTHREADDEPENDENCY_H_ */
