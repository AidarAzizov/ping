#include "../includes/ft_ping.h"

extern mypair work;

bool    sender(IcmpTargetType *targets)
{
    sockaddr_in connection;
    int         howSended;
    int         len;

    connection.sin_port = targets->_addrinfo->ai_protocol;
    connection.sin_family = targets->_addrinfo->ai_family;
    connection.sin_addr.s_addr =
            ((sockaddr_in*)&targets->_addrinfo->ai_addr)->sin_addr.s_addr;
    len = targets->packlen;
    howSended = sendto(targets->_socket, targets->packToSend, len, 0,
                       (struct sockaddr*)&connection, sizeof(struct sockaddr));
    if (howSended == len)
        return true;
    if (howSended == -1)
        return printAndExit(strerror(errno));
    return printAndExit("Bad try to send packet");
}

bool    receiver(IcmpTargetType *target)
{
    ssize_t howReceived;

    howReceived = -1;
    while (work.stillwait && !work.alreadyend && howReceived == -1)
        howReceived = recvmsg(target->_socket, &target->msg, MSG_DONTWAIT);
    alarm(0);
    if (work.alreadyend)
        return false;
    if (howReceived == target->packlen)
        return true;
    if (howReceived == -1)
        return printAndExit(strerror(errno));
    return printAndExit("Bad try to receive packet");
}
