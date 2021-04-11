#include "../includes/ft_ping.h"

bool    createSocket(IcmpTargetType *target)
{
    int yes;
    int result;

    yes = 1;
    target->hints.ai_family = AF_INET;
    target->hints.ai_socktype = SOCK_RAW;
    target->hints.ai_protocol = IPPROTO_ICMP;
    result = getaddrinfo(target->dst_addr, NULL, &target->hints, &target->_addrinfo);
    if (result != 0 || !target->_addrinfo)
        return printAndExit("No address associated with hostname");
    target->_socket = socket(target->_addrinfo->ai_family,
                              target->_addrinfo->ai_socktype,
                              target->_addrinfo->ai_protocol);
    if (target->_socket < 0)
        return printAndExit("Can't create socket");
    if (setsockopt(target->_socket, IPPROTO_IP, IP_HDRINCL, &yes, sizeof(int)) < 0)
        return printAndExit("Bad with setting socket options");
    return true;
}