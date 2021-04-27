#include "../includes/ft_ping.h"

extern t_mypair	g_work;

BOOL	makeDelay(double delay_sec, BOOL *is_firstIter)
{
	struct timeval	beg;
	struct timeval	curr;
	double			deltatime;

	if (*is_firstIter)
	{
		*is_firstIter = FALSE;
		return (TRUE);
	}
	initTime(&beg);
	while (!g_work.alreadyend)
	{
		initTime(&curr);
		deltatime = (double)(curr.tv_sec - beg.tv_sec)
			+ (double)(curr.tv_usec - beg.tv_usec) / 1000000.;
		if (deltatime >= delay_sec)
			break ;
	}
	if (g_work.alreadyend)
		return (FALSE);
	return (TRUE);
}

BOOL	run(t_IcmpTargetType *targets)
{
	int	delta;

	delta = targets->timeaft.tv_sec - targets->timebegin.tv_sec;
	printf("%d\n", delta);
	if (g_work.alreadyend)
		return (FALSE);
	if ((targets->currFl.inf_c || targets->currFl.c-- > 0)
		&& (targets->currFl.inf_w || targets->currFl.w > delta))
		return (TRUE);
	return (FALSE);
}

BOOL	pingPrepare(t_IcmpTargetType *targets)
{
	nullifyTargets(targets);
	if (!createSocket(targets))
		return (FALSE);
	if (!prepareTargets(targets))
		return (FALSE);
	if (!initTime(&targets->timebegin))
		return (FALSE);
	if (!initTime(&targets->timeaft))
		return (FALSE);
	printHead(targets);
	return (TRUE);
}

BOOL	ping(t_IcmpTargetType *targets)
{
	BOOL	firstIter;

	firstIter = TRUE;
	if (!pingPrepare(targets))
		return (FALSE);
	while (run(targets))
	{
		if (!makeDelay(targets->currFl.dt, &firstIter))
			continue ;
		recyclePacket(targets);
		if (!sender(targets))
			continue ;
		if (!initTime(&targets->timebef))
			return (FALSE);
		alarm(targets->currFl.w);
		if (!receiver(targets))
			continue ;
		if (!initTime(&targets->timeaft))
			return (FALSE);
		printBody(targets);
	}
	if (!initTime(&targets->timeend))
		return (FALSE);
	printTail(targets);
	return (TRUE);
}
