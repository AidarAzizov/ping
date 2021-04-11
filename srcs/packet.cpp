#include "../includes/ft_ping.h"

extern mypair   work;

void    recyclePacket(IcmpTargetType *target)
{
    target->_icmp->un.echo.sequence++;
    target->_icmp->checksum = 0;
    target->_icmp->checksum = ~GetCSum((uint8_t*)target->_icmp,
                                       sizeof(struct icmphdr) +
                                               target->currentFlags.count_s);
    memset(target->packToRecv, 0, target->packlen);
    target->iov.iov_base = target->packToRecv;
    target->iov.iov_len = target->packlen;
    target->msg.msg_iov = &target->iov;
    work.stillwait = true;
}

void    clearPacket(IcmpTargetType *target)
{
    if (target->addr_from_arg)
        free(target->addr_from_arg);
    if (target->_addrinfo)
        freeaddrinfo(target->_addrinfo);
    if (target->packToSend)
        free(target->packToSend);
    if (INITED(target->_socket))
        close(target->_socket);
}

bool    bindingPacket(IcmpTargetType *targets)
{
    targets->packlen = sizeof(struct iphdr) +
            sizeof(struct icmphdr) + targets->currentFlags.count_s;
    if (!(targets->packToSend = (char*)malloc(sizeof(char) * targets->packlen)))
        return printAndExit("System error. Bad allocate memory");
    if (!(targets->packToRecv = (char*)malloc(sizeof(char) * targets->packlen)))
        return printAndExit("System error. Bad allocate memory");
    targets->_ip = (struct iphdr*)targets->packToSend;
    targets->_icmp = (struct icmphdr*)(targets->packToSend + sizeof(struct iphdr));
    return true;
}
