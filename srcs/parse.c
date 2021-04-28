#include "../includes/ft_ping.h"

BOOL	HostToIP(t_IcmpTargetType *targets)
{
	struct sockaddr_in	*in;
	struct addrinfo		hints;
	struct addrinfo		*res;
	int					family;

	res = NULL;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	ft_memset(targets->dst_addr, 0, INET_ADDRSTRLEN);
	printf("%s\n", targets->addr_from_arg);
	if (getaddrinfo(targets->addr_from_arg, NULL, &hints, &res) != 0)
	{
		if (res)
			freeaddrinfo(res);
		return (printExitWStr(targets->addr_from_arg,
				": Name or service not known"));
	}
	family = res->ai_family;
	in = (struct sockaddr_in *)res->ai_addr;
	freeaddrinfo(res);
	if (!inet_ntop(family, &(in->sin_addr),
			targets->dst_addr, sizeof(targets->dst_addr)))
		return (printExitWStr(
				"Error inet_ntop() for ", targets->addr_from_arg));
	return (TRUE);
}

BOOL	init_count(uint64_t *res, int *index, int argc, char *argv[])
{
	char		flag;
	char		symb;
	uint64_t	checker;

	checker = 0;
	if (sscanf(argv[*index], " -%c %ld %c ", &flag, res, &symb) == 2)
		return (TRUE);
	if (ft_strlen(argv[*index]) > 2)
		return (printExitWStr("invalid argument: ", argv[*index]));
	(*index)++;
	if (*index >= argc)
		return (printExitWStr("option requires an argument -- ",
				argv[(*index) - 1]));
	if (sscanf(argv[*index], " %ld %c ", res, &symb) != 1)
		return (printExitWStr("invalid argument: ", argv[*index]));
	sscanf(argv[*index], " %ld %c ", &checker, &symb);
	if (*res != checker)
		return (printExitWInt("invalid argument: ", (int)checker));
	return (TRUE);
}

BOOL	init_delaytime(double *res, int *index, int argc, char *argv[])
{
	char	symb;

	//if (count_not_of(argv[*index], ' ') > 3
	//	&& sscanf(argv[*index], " -i %lf %c ", res, &symb) == 1)
	if (sscanf(argv[*index], " -i %lf %c ", res, &symb) == 1)
		return (TRUE);
	if (ft_strlen(argv[*index]) > 2)
		return (printExitWStr("invalid argument: ", argv[*index]));
	(*index)++;
	if (*index >= argc)
		return (printExitWStr("option requires an argument -- ",
				argv[(*index) - 1]));
	if (sscanf(argv[*index], " %lf %c ", res, &symb) != 1)
		return (printExitWStr("invalid argument: ", argv[*index]));
	return (TRUE);
}

BOOL	init_addr(t_IcmpTargetType *targets, const char *arg)
{
	char		symb;
	t_ip_addr	IPcheck;

	targets->addr_from_arg = (char *)malloc(sizeof(char) * NI_MAXHOST);
	if (!targets->addr_from_arg)
		return (printAndExit("Bad with allocate memory", FALSE));
	targets->addr_from_arg_host = FALSE;
	ft_memset(targets->addr_from_arg, 0, NI_MAXHOST);
	if (sscanf(arg, "%hhu.%hhu.%hhu.%hhu%c",
			&IPcheck.i1, &IPcheck.i2, &IPcheck.i3, &IPcheck.i4, &symb) == 4)
	{
		ft_strcpy(targets->addr_from_arg, arg);
		ft_strcpy(targets->dst_addr, arg);
		return (TRUE);
	}
	else if (sscanf(arg, " %s %c", targets->addr_from_arg, &symb) == 1)
	{
		targets->addr_from_arg_host = HostToIP(targets);
		return (targets->addr_from_arg_host);
	}
	return (printExitWStr(arg, ": Name or service not known"));
}

void	parseArgs(t_IcmpTargetType *targets,
			   int argc, char *argv[], BOOL *result)
{
	int	i;

	i = 0;
	while (*result && ++i < argc)
	{
		if (ft_strncmp(argv[i], "-h", 2) == 0 && ft_strlen(argv[i]) == 2)
			targets->currFl.info = TRUE;
		else if (ft_strncmp(argv[i], "-v", 2) == 0)
			targets->currFl.more_info = TRUE;
		else if (ft_strncmp(argv[i], "-w", 2) == 0)
			targets->currFl.inf_w = !(*result
					= init_count(&targets->currFl.w, &i, argc, argv));
		else if (ft_strncmp(argv[i], "-s", 2) == 0)
			*result = init_count(&targets->currFl.s, &i, argc, argv);
		else if (ft_strncmp(argv[i], "-t", 2) == 0)
			*result = init_count(&targets->currFl.t, &i, argc, argv);
		else if (ft_strncmp(argv[i], "-c", 2) == 0)
			targets->currFl.inf_c = !(*result
					= init_count(&targets->currFl.c, &i, argc, argv));
		else if (ft_strncmp(argv[i], "-i", 2) == 0)
			*result = init_delaytime(&targets->currFl.i, &i, argc, argv);
		else
			*result = printExitWStr("invalid argument: ", argv[i]);
	}
}
