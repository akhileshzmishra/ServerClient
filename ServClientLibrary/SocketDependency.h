/*
 * SocketDependency.h
 *
 *  Created on: 11-Oct-2015
 *      Author: akhilesh
 */

#ifndef SOCKETDEPENDENCY_H_
#define SOCKETDEPENDENCY_H_

#include "XLDependency.h"

#ifndef __UNIX__

/////////////For Windows OS////////////START
#include <windows.h>
#include <windowsx.h>
#include <winsock.h>

#ifndef __WINDOWS_OS_
#define __WINDOWS_OS_
#endif

typedef SOCKET Socket_Identifier;

//////////////////////////////////////END
#else
/////////////For Linux based OS////////////START
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#ifndef __LINUX_OS_
#define __LINUX_OS_
#endif

typedef int WSADATA;
typedef int Socket_Identifier;

#ifndef PASCAL
#define PASCAL
#endif

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr* PSOCKADDR;
typedef struct sockaddr_in* LPSOCKADDR;

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

////////////////////////////////////////////END
#endif // Windows part


//The below code is to make openssl active.
//define this macro if you want to use openssl for socket communication
#ifdef _OPENSSL_ACTIVE_
#include "bio.h"
#include "ssl.h"
#include "err.h"
#include "engine.h"
#include "conf.h"
#endif // _OPENSSL_ACTIVE_
////////////////////////////////////////////////////////////////////////

#endif /* SOCKETDEPENDENCY_H_ */
