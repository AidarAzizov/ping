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

BOOL	PrintAssctdWSeqInfo(const char *real_dst,
			int _icmp_seq, int _icmp_code)
{
	printf("From %s icmp_seq=%d ", real_dst, _icmp_seq);
	if (_icmp_code == ICMP_UNREACH)
		printf("destination unreachable");
	if (_icmp_code == ICMP_SOURCEQUENCH)
		printf("packet lost");
	if (_icmp_code == ICMP_REDIRECT)
		printf("redirect code");
	if (_icmp_code == ICMP_ALTHOSTADDR)
		printf("alternate-address");
	if (_icmp_code == ICMP_ECHO)
		printf("echo service");
	if (_icmp_code == ICMP_ROUTERADVERT)
		printf("router advertisement");
	if (_icmp_code == ICMP_ROUTERSOLICIT)
		printf("router solicitation");
	if (_icmp_code == ICMP_TIMXCEED)
		printf("time to live exceeded");
	if (_icmp_code == ICMP_PARAMPROB)
		printf("parameter-problem");
	if (_icmp_code == ICMP_TSTAMP)
		printf("timestamp request");
	if (_icmp_code == ICMP_TSTAMP)
		printf("timestamp request");
	return (FALSE);
}
