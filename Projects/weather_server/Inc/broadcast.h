#ifndef BROADCAST_H_INCLUDED
#define BROADCAST_H_INCLUDED
#include <stdint.h>

#define BROADCAST_IP                "255.255.255.255"

#define BROADCAST_UNIQUE_STR        "SMARTHOME_HQ"
#define BROADCAST_UNIQUE_STR_LEN    (strlen(BROADCAST_UNIQUE_STR)+1)

#define BROADCAST_MSG_LEN           1024
#define TCP_MSG_LEN                 1024

#define BROADCAST_LISTENING_PORT    12345    // Same for every device

#endif // BROADCAST_H_INCLUDED