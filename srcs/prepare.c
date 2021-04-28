#include "../includes/ft_ping.h"

extern t_mypair	g_work;

BOOL	ip_prepare(t_IcmpTargetType *target)
{
	target->_ip->tot_len = htons(sizeof(struct iphdr)
			+ sizeof(struct icmphdr)
			+ target->currFl.s);
	target->_ip->protocol = target->_addrinfo->ai_protocol;
	target->_ip->ttl = target->currFl.t;
	target->_ip->ihl = sizeof(*(target->_ip)) >> 2;
	target->_ip->frag_off = 0;
	target->_ip->version = 4;
	target->_ip->check = 0;
	target->_ip->tos = 0;
	if (inet_pton(target->_addrinfo->ai_family,
			   target->src_addr, &(target->_ip->saddr)) != 1)
		return (printAndExit("Error inet_pton1", FALSE));
	if (inet_pton(target->_addrinfo->ai_family,
			   target->dst_addr, &(target->_ip->daddr)) != 1)
		return (printAndExit("Error inet_pton2", FALSE));
	return (TRUE);
}

void	msg_prepare(t_IcmpTargetType *target)
{
	memset(target->packToRecv, 0, target->packlen);
	memset(&target->msg, 0, sizeof(struct msghdr));
	target->msg.msg_iovlen = 1;
	target->msg.msg_name = target->_addrinfo->ai_addr;
	target->msg.msg_namelen = target->_addrinfo->ai_addrlen;
	target->msg.msg_flags = 0;
	target->msg.msg_control = NULL;
	target->msg.msg_controllen = 0;
}

void	icmp_prepare(t_IcmpTargetType *target)
{
	target->_icmp->un.echo.sequence = 1;
	target->_icmp->type = ICMP_ECHO;
	target->_icmp->un.echo.id = 1;
	target->_icmp->checksum = 0;
	target->_icmp->code = 0;
}

BOOL	prepareTargets(t_IcmpTargetType *target)
{
	uint16_t	i;
	size_t		offset;
	uint16_t	payloadLength;

	strcpy(target->src_addr, LOCALHOST);
	if (!ip_prepare(target))
		return (FALSE);
	icmp_prepare(target);
	msg_prepare(target);
	if (gettimeofday(&target->pinfo.times, NULL) != SUCCESS_CODE)
		return (FALSE);
	payloadLength = target->currFl.s;
	offset = sizeof(struct iphdr) + sizeof(struct icmphdr);
	i = 0;
	while (i++ < payloadLength)
		target->packToSend[offset + i] = (char)i;
	return (TRUE);
}

void	nullifyTargets(t_IcmpTargetType *target)
{
	memset(&target->iov, 0, sizeof(target->iov));
	memset(&target->_ip->saddr, 0, sizeof(in_addr_t));
	memset(&target->_ip->daddr, 0, sizeof(in_addr_t));
	memset(target->packToSend, 0, target->packlen);
	memset(&target->hints, 0, sizeof(struct addrinfo));
	target->_addrinfo = NULL;
	target->_socket = -1;
	target->pinfo.avg = 0.0;
	target->pinfo.max = DBL_MIN;
	target->pinfo.min = DBL_MAX;
	target->pinfo.mdev.sum = 0.0;
	target->pinfo.mdev.sum2 = 0.0;
	target->pinfo.received = 0;
	g_work.alreadyend = FALSE;
}
