/*
 * XLThreadDependency.h
 *
 *  Created on: 11-Oct-2015
 *      Author: akhilesh
 */

#ifndef XLTHREADDEPENDENCY_H_
#define XLTHREADDEPENDENCY_H_

#define __MAC_OS_

#ifndef __UNIX__

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

#else

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

#endif

#endif /* XLTHREADDEPENDENCY_H_ */
