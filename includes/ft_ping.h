#pragma once

#ifndef FT_PING_H
# define FT_PING_H

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <iomanip>
#include <stdint.h>
#include <time.h>
#include <ctime>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <netdb.h>
#include <signal.h>
#include <math.h>
#include <ifaddrs.h>

#define SUCCESS_CODE 0
#define ERROR_CODE 1

#define BYTE uint8_t
#define LOCALHOST "0.0.0.0"

#ifndef DBL_MAX
#define DBL_MAX 1.79769e+308
#endif

#ifndef DBL_MIN
#define DBL_MIN 2.22507e-308
#endif

#define ROOT_UID 0
#define SOCKET int
#define INITED(x) (x != -1)

typedef struct
{
    uint8_t i1;
    uint8_t i2;
    uint8_t i3;
    uint8_t i4;
} ip_addr;

typedef struct
{
    uint64_t    count_s;
    uint64_t    count_t;
    uint64_t    count_c;
    uint64_t    count_w;
    double      delay_time;
    bool        infinity_c;
    bool        infinity_w;
    bool        print_info;
    bool        more_info;
} flags;

typedef struct
{
    double sum;
    double sum2;
} t_mdev;

typedef struct
{
    double min;
    double avg;
    double max;
    t_mdev mdev;

    uint32_t received;
    struct timeval times;
} pingstat;

typedef struct
{
    bool stillwait;
    bool alreadyend;
} mypair;

typedef struct
{
    pingstat        pinginfo;
    flags           currentFlags;
    struct iphdr    *_ip;
    struct icmphdr  *_icmp;
    struct addrinfo *_addrinfo;
    struct addrinfo hints;
    struct iovec    iov;
    struct msghdr   msg;
    struct timeval  timebefore;
    struct timeval  timeafter;
    SOCKET          _socket = 0;
    struct timeval  timebegin;
    struct timeval  timeend;
    __uid_t         uid;

    bool            addr_from_arg_host;
    char            *addr_from_arg;
    char            dst_addr[INET_ADDRSTRLEN];
    char            src_addr[INET_ADDRSTRLEN];
    char            *packToRecv;
    char            *packToSend;
    uint16_t        packlen;
} IcmpTargetType;


void        printHead(IcmpTargetType *targets);
void        printBody(IcmpTargetType *target);
void        printTail(IcmpTargetType *target);

bool        createSocket(IcmpTargetType *target);

bool        init_addr(IcmpTargetType *targets, const char* arg);
void        parseArgs(IcmpTargetType *targets, int argc, char *argv[], bool *result);

int         count_not_of(const char *str, char symb);
bool        initTime(timeval* timeout);
int         printAndExit(const char *message, int code);
bool        printAndExit(const char *message);
uint16_t	GetCSum(const uint8_t *Buf, uint32_t Len, uint16_t StartVal = 0);

bool        prepareTargets(IcmpTargetType *target);
void        nullifyTargets(IcmpTargetType *target);

void        recyclePacket(IcmpTargetType *target);
void        clearPacket(IcmpTargetType *target);
bool        bindingPacket(IcmpTargetType *targets);

bool        sender(IcmpTargetType* targets);
bool        receiver(IcmpTargetType* target);

bool        ping(IcmpTargetType *targets);

# endif