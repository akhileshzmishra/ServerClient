#ifndef _COMMUNICATION_HEADER_H_
#define _COMMUNICATION_HEADER_H_



#include <stdio.h>
#include <stdlib.h>





#define MAX_MESS_LENGTH 50
#define DEFAULT_SERVER_ACCEPTORS 2
#define MAX_NUM_CONN_TRIES 5
#define MAX_RECIEVE_TIME_OUT 1000
#define BIND_SLEEP 10000
#define CLIENT_HEART_BEAT_ALARM_TIME 4000

#include "XLAutoPtr.h"

typedef XLAutoPtr::XLSharedArraySmPtr<char> CHARSTORAGE;



enum XLEventQueueConfig
{
   XLEventQueueConfig_MaxSize = 256,
   XLEventQueueMaxm_pDataSize = 128
};




#endif
