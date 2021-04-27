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
