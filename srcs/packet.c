#include "../includes/ft_ping.h"

extern t_mypair	g_work;

int	printInfo(void)
{
	printf("\nUsage\n\tping [options] <destination>\n");
	printf("\nOptions:\n  <destination>\t\tdns name or ip address");
	printf("\n  -dt <delay>\t\tinstall <delay>(may "
		"be less than 1.0) time for delay between pings");
	printf("\n  -c <count>\t\tstop after <count> replies");
	printf("\n  -h\t\t\tprint help and exit");
	printf("\n  -s <size>\t\tuse <size> as number of data bytes to be sent");
	printf("\n  -t <ttl>\t\tdefine time to live");
	printf("\n  -w <deadline>\t\treply wait <deadline> in seconds");
	printf("\n");
	return (SUCCESS_CODE);
}

void	recyclePacket(t_IcmpTargetType *target)
{
	target->_icmp->un.echo.sequence++;
	target->_icmp->checksum = 0;
	target->_icmp->checksum = ~GetCSum((uint8_t *)target->_icmp,
			sizeof(struct icmphdr) + target->currFl.s, 0);
	memset(target->packToRecv, 0, target->packlen);
	target->iov.iov_base = target->packToRecv;
	target->iov.iov_len = target->packlen;
	target->msg.msg_iov = &target->iov;
	g_work.stillwait = TRUE;
}

void	clearPacket(t_IcmpTargetType *target)
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

BOOL	bindingPacket(t_IcmpTargetType *targets)
{
	targets->packlen = sizeof(struct iphdr) + sizeof(struct icmphdr)
		+ targets->currFl.s;
	targets->packToSend = (char *)malloc(sizeof(char) * targets->packlen);
	if (!targets->packToSend)
		return (printAndExit("System error. Bad allocate memory", FALSE));
	targets->packToRecv = (char *)malloc(sizeof(char) * targets->packlen);
	if (!targets->packToRecv)
		return (printAndExit("System error. Bad allocate memory", FALSE));
	targets->_ip = (struct iphdr *)targets->packToSend;
	targets->_icmp = (struct icmphdr)(targets->packToSend
			+ sizeof(struct iphdr));
	return (TRUE);
}
