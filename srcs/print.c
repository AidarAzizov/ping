#include "../includes/ft_ping.h"

BOOL	checkHeader(const t_IcmpTargetType *target)
{
	struct iphdr	*ip_h;
	struct icmphdr	*icmp_h;

	icmp_h = (struct icmphdr *)(target->packToRecv + sizeof(struct iphdr *));
	ip_h = (struct iphdr *)target->packToRecv;
	if (ip_h->saddr != target->_ip->daddr
		|| ip_h->protocol != target->_ip->protocol
		|| ip_h->tot_len != target->_ip->tot_len
		|| icmp_h->un.echo.id != target->_icmp->un.echo.id
		|| icmp_h->un.echo.sequence != target->_icmp->un.echo.sequence)
		return (printAndExit("Received packet with wrong header", FALSE));
	return (TRUE);
}

BOOL	isValidPacket(t_IcmpTargetType *target)
{
	struct icmphdr	*icmp_h;
	uint16_t		temp;
	size_t			offset;
	uint16_t		res;

	if (!checkHeader(target))
		return (FALSE);
	icmp_h = (struct icmphdr *)(target->packToRecv + sizeof(struct iphdr));
	offset = sizeof(struct iphdr) + sizeof(struct icmphdr);
	temp = icmp_h->checksum;
	icmp_h->checksum = 0;
	res = ~GetCSum((uint8_t *)icmp_h,
			sizeof(struct icmphdr) + target->currFl.s, 0);
	if (icmp_h->code != 0 || icmp_h->type != ICMP_ECHOREPLY)
		return (printAndExit("Appeared network error with try to ping", FALSE));
	if (res != temp)
		return (printAndExit("In received header checksum is not true", FALSE));
	if (strncmp(target->packToRecv + offset, target->packToSend + offset,
			 target->currFl.s) != 0)
		return (printAndExit("Payload is not the same like original", FALSE));
	return (TRUE);
}

void	printHead(t_IcmpTargetType *targets)
{
	int	len;

	len = targets->currFl.s
		+ sizeof(struct iphdr *) + sizeof(struct icmphdr *);
	printf("PING %s (%s) %lld(%d) bytes of data.\n",
		targets->addr_from_arg, targets->dst_addr,
		targets->currFl.s, len);
}

void	printBody(t_IcmpTargetType *target)
{
	double			time;
	uint16_t		packlen;
	struct iphdr	*ip_h;

	ip_h = (struct iphdr *)target->packToRecv;
	if (!isValidPacket(target))
		return ;
	time = (double)(target->timeaft.tv_sec - target->timebef.tv_sec) *1000.
		+ (double)(target->timeaft.tv_usec - target->timebef.tv_usec) / 1000.;
	packlen = target->packlen - sizeof(struct iphdr *);
	if (strcmp(target->addr_from_arg, target->dst_addr) == 0)
		printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.1f ms\n",
			packlen, target->dst_addr, target->_icmp->un.echo.sequence,
			ip_h->ttl, time);
	else
		printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.1f ms\n",
			packlen, target->addr_from_arg, target->dst_addr,
			target->_icmp->un.echo.sequence, ip_h->ttl, time);
	target->pinfo.min = firstTernare(target->pinfo.min, time);
	target->pinfo.max = secondTernare(target->pinfo.max, time);
	target->pinfo.mdev.sum2 += (time * time);
	target->pinfo.mdev.sum += time;
	target->pinfo.avg += time;
	target->pinfo.received++;
}

void	printTail(t_IcmpTargetType *target)
{
	int		percent_of_lost;
	int		ms;
	double	mdev;

	printf("\n--- %s ping statistics ---\n", target->addr_from_arg);
	ms = (target->timeend.tv_sec - target->timebegin.tv_sec) * 1000
		+ (target->timeend.tv_usec - target->timebegin.tv_usec) / 1000;
	if (target->_icmp->un.echo.sequence == 0)
		percent_of_lost = 0;
	else
		percent_of_lost = 100 - ((target->pinfo.received * 100)
				/ target->_icmp->un.echo.sequence);
	printf("%d packets transmitted, %d received, %d%% packet loss, time %d ms\n",
		target->_icmp->un.echo.sequence, target->pinfo.received,
		percent_of_lost, ms);
	target->pinfo.avg /= target->pinfo.received;
	target->pinfo.mdev.sum /= target->pinfo.received;
	target->pinfo.mdev.sum2 /= target->pinfo.received;
	mdev = sqrt(target->pinfo.mdev.sum2 - (pow(target->pinfo.mdev.sum, 2)));
	if (target->pinfo.received > 0)
	{
		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
			target->pinfo.min,
			target->pinfo.avg, target->pinfo.max, mdev);
	}
}
