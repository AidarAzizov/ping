#include "../includes/ft_ping.h"

t_mypair	g_work;

void	setSignal(int sig)
{
	if (sig == SIGALRM)
		g_work.stillwait = FALSE;
	else if (sig == SIGINT)
		g_work.alreadyend = TRUE;
}

void	setBaseFlags(t_flags *fl)
{
	fl->w = 2;
	fl->s = 56;
	fl->c = 0;
	fl->t = 64;
	fl->inf_w = TRUE;
	fl->inf_c = TRUE;
	fl->i = 1.0;
	fl->more_info = FALSE;
	fl->info = FALSE;
}

BOOL	checkArgs(t_flags *fl)
{
	if (!fl->inf_c && (fl->c < 1
			|| fl->c > 9223372036854775807))
		return (printAndExit(" invalid argument for -dt flag:"
				" out of range: 0 <= value <= 9223372036854775807", FALSE));
	if (!fl->inf_w && fl->w > 2147483647)
		return (printAndExit(" invalid argument for -w flag:"
				" out of range: 0 <= value <= 2147483647", FALSE));
	if (fl->s > 127992)
		return (printAndExit(" invalid argument for -s flag:"
				" out of range: 0 <= value <= 127992", FALSE));
	if (fl->t > 255)
		return (printAndExit(" invalid argument for -T flag:"
				" out of range: 0 <= value <= 255", FALSE));
	if (fl->i < 0.001 || fl->i > 600.)
		return (printAndExit(" invalid argument for -i flag:"
				" out of range: 0.001 <= value <= 600.000", FALSE));
	return (TRUE);
}

BOOL	baseInit(t_IcmpTargetType *targets, int argc, char *argv[])
{
	targets->uid = getuid();
	if (argc == 1)
	{
		printInfo();
		return (FALSE);
	}
	if (targets->uid != ROOT_UID)
		return (printAndExit("Launch this program from "
				"root for create RAW socket\n", FALSE));
	setBaseFlags(&targets->currFl);
	if (!init_addr(targets, argv[1]))
		return (FALSE);
	return (TRUE);
}

int	main(int argc, char *argv[])
{
	t_IcmpTargetType	targets;
	BOOL				res;

	res = TRUE;
	signal(SIGALRM, setSignal);
	signal(SIGINT, setSignal);
	if (!baseInit(&targets, argc, argv))
		return (ERROR_CODE);
	parseArgs(&targets, argc - 1, argv + 1, &res);
	if (targets.currFl.info)
		return (printInfo());
	if (!res || !checkArgs(&targets.currFl))
		return (ERROR_CODE);
	if (!bindingPacket(&targets))
		return (ERROR_CODE);
	if (!ping(&targets))
		return (ERROR_CODE);
	clearPacket(&targets);
	return (SUCCESS_CODE);
}
