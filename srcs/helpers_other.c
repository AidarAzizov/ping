#include "../includes/ft_ping.h"

BOOL	INITED(int x)
{
	return (x != -1);
}

double	firstTernare(double tone, double ttwo)
{
	if (tone > ttwo)
		return (ttwo);
	return (tone);
}

double	secondTernare(double tone, double ttwo)
{
	if (tone < ttwo)
		return (ttwo);
	return (tone);
}

BOOL	initTime(struct timeval *timeout)
{
	if (gettimeofday(timeout, NULL) != SUCCESS_CODE)
		return (FALSE);
	return (TRUE);
}

BOOL	PrintAssctdWSeqInfo(uint32_t real_dst,
			int _icmp_seq, int _icmp_code)
{
	char	str[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &real_dst, str, INET_ADDRSTRLEN);
	printf("From %s icmp_seq=%d ", str, _icmp_seq);
	if (_icmp_code == ICMP_UNREACH)
		printf("destination unreachable\n");
	else if (_icmp_code == ICMP_SOURCEQUENCH)
		printf("packet lost\n");
	else if (_icmp_code == ICMP_REDIRECT)
		printf("redirect code\n");
	else if (_icmp_code == ICMP_ECHO)
		printf("echo service\n");
	else if (_icmp_code == ICMP_ROUTERADVERT)
		printf("router advertisement\n");
	else if (_icmp_code == ICMP_ROUTERSOLICIT)
		printf("router solicitation\n");
	else if (_icmp_code == ICMP_TIMXCEED)
		printf("time to live exceeded\n");
	else if (_icmp_code == ICMP_PARAMPROB)
		printf("parameter-problem\n");
	else if (_icmp_code == ICMP_TSTAMP)
		printf("timestamp request\n");
	return (FALSE);
}
