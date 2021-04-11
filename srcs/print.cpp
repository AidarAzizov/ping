#include "../includes/ft_ping.h"

bool checkHeader(const IcmpTargetType *target)
{
    iphdr   *ip_h;
    icmphdr *icmp_h;

    icmp_h = (struct icmphdr*)(target->packToRecv + sizeof(iphdr));
    ip_h = (struct iphdr*)target->packToRecv;
    if (ip_h->saddr != target->_ip->daddr ||
        ip_h->protocol != target->_ip->protocol ||
        ip_h->tot_len != target->_ip->tot_len ||
        icmp_h->un.echo.id != target->_icmp->un.echo.id ||
        icmp_h->un.echo.sequence != target->_icmp->un.echo.sequence)
        return printAndExit("Received packet with wrong header");
    return true;
}

bool    isValidPacket(IcmpTargetType *target)
{
    icmphdr     *icmp_h;
    uint16_t    temp;
    size_t      offset;

    if (!checkHeader(target))
        return false;
    icmp_h = (struct icmphdr*)(target->packToRecv + sizeof(iphdr));
    offset = sizeof(struct iphdr) + sizeof(struct icmphdr);
    temp = icmp_h->checksum;
    icmp_h->checksum = 0;
    uint16_t res = ~GetCSum((uint8_t*)icmp_h,
                            sizeof(struct icmphdr) + target->currentFlags.count_s);
    if (icmp_h->code != 0 || icmp_h->type!= ICMP_ECHOREPLY)
        return printAndExit("Appeared network error with try to ping");
    if (res != temp)
        return printAndExit("In received header checksum is not true");
    if (strncmp(target->packToRecv + offset, target->packToSend + offset,
                target->currentFlags.count_s) != 0)
        return printAndExit("Payload is not the same like original");
    return true;
}

void    printHead(IcmpTargetType *targets)
{
    int len;

    len = targets->currentFlags.count_s +
              sizeof(struct iphdr) + sizeof(struct icmphdr);
    printf("PING %s (%s) %ld(%d) bytes of data.\n",
           targets->addr_from_arg, targets->dst_addr,
           targets->currentFlags.count_s, len);
}

void    printBody(IcmpTargetType *target)
{
    double          time;
    uint16_t        packlen;
    struct iphdr    *ip_h;

    ip_h = (struct iphdr*)target->packToRecv;
    if (!isValidPacket(target))
        return;
    time = (target->timeafter.tv_sec - target->timebefore.tv_sec)*1000 +
            (double)(target->timeafter.tv_usec - target->timebefore.tv_usec)/1000.;
    packlen = target->packlen - sizeof(struct iphdr);
    if (strcmp(target->addr_from_arg, target->dst_addr) == 0)
        printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.1f ms\n",
               packlen, target->dst_addr, target->_icmp->un.echo.sequence,
               ip_h->ttl, time);
    else
        printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.1f ms\n",
               packlen, target->addr_from_arg, target->dst_addr,
               target->_icmp->un.echo.sequence, ip_h->ttl, time);
    target->pinginfo.min = (target->pinginfo.min > time) ? time : target->pinginfo.min;
    target->pinginfo.max = (target->pinginfo.max < time) ? time : target->pinginfo.max;
    target->pinginfo.mdev.sum2 += (time * time);
    target->pinginfo.mdev.sum += time;
    target->pinginfo.avg += time;
    target->pinginfo.received++;
}

void    printTail(IcmpTargetType *target)
{
    int     percent_of_lost;
    int     ms;
    double  mdev;
    printf("\n--- %s ping statistics ---\n", target->addr_from_arg);

    ms = (target->timeend.tv_sec - target->timebegin.tv_sec)*1000 +
            (target->timeend.tv_usec - target->timebegin.tv_usec)/1000;
    if (target->_icmp->un.echo.sequence == 0)
        percent_of_lost = 0;
    else
        percent_of_lost = 100 - ((target->pinginfo.received * 100) / target->_icmp->un.echo.sequence);
    printf("%d packets transmitted, %d received, %d%% packet loss, time %d ms\n",
           target->_icmp->un.echo.sequence, target->pinginfo.received,
           percent_of_lost, ms);

    target->pinginfo.avg /= target->pinginfo.received;
    target->pinginfo.mdev.sum /= target->pinginfo.received;
    target->pinginfo.mdev.sum2 /= target->pinginfo.received;
    mdev = sqrt(target->pinginfo.mdev.sum2 - (pow(target->pinginfo.mdev.sum, 2)));
    if (target->pinginfo.received > 0)
    {
        printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
               target->pinginfo.min, target->pinginfo.avg, target->pinginfo.max, mdev);
    }
}
