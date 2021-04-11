#include "../includes/ft_ping.h"

mypair  work;

void    setSignal(int sig)
{
    if (sig == SIGALRM) work.stillwait = false;
    else if (sig == SIGINT) work.alreadyend = true;
}

void    setBaseFlags(flags *fl)
{
    fl->count_w = 0;
    fl->count_s = 56;
    fl->count_c = 0;
    fl->count_t = 64;
    fl->infinity_w = true;
    fl->infinity_c = true;
    fl->delay_time = 1.0;
    fl->more_info = false;
    fl->print_info = false;
}

int     printInfo()
{
    printf("\nUsage\n\tping [options] <destination>\n");
    printf("\nOptions:\n  <destination>\t\tdns name or ip address");
    printf("\n  -dt <delay>\t\tinstall <delay>(may"
           " be less than 1.0) time for delay between pings");
    printf("\n  -c <count>\t\tstop after <count> replies");
    printf("\n  -h\t\t\tprint help and exit");
    printf("\n  -s <size>\t\tuse <size> as number of data bytes to be sent");
    printf("\n  -t <ttl>\t\tdefine time to live");
    printf("\n  -w <deadline>\t\treply wait <deadline> in seconds");
    printf("\n");
    return SUCCESS_CODE;
}

bool    checkArgs(flags *fl)
{
    if (!fl->infinity_c && (fl->count_c < 1 || fl->count_c > 9223372036854775807))
        return printAndExit(" invalid argument for -dt flag:"
                            " out of range: 0 <= value <= 9223372036854775807");
    if (!fl->infinity_w && (fl->count_w < 0 || fl->count_w > 2147483647))
        return printAndExit(" invalid argument for -w flag:"
                            " out of range: 0 <= value <= 2147483647");
    if (fl->count_s < 0 || fl->count_s > 127992)
        return printAndExit(" invalid argument for -s flag:"
                            " out of range: 0 <= value <= 127992");
    if (fl->count_t < 0 || fl->count_t > 255)
        return printAndExit(" invalid argument for -t flag:"
                            " out of range: 0 <= value <= 255");
    if (fl->delay_time < 0.001 || fl->delay_time > 600.)
        return printAndExit(" invalid argument for -dt flag:"
                            " out of range: 0.001 <= value <= 600.000");
    return true;
}

bool    baseInit(IcmpTargetType  *targets, int argc, char *argv[])
{
    targets->uid = getuid();
    if (argc == 1)
        return printAndExit("usage error:"
                            "Destination address required");
    if (targets->uid != ROOT_UID)
        return printAndExit("Launch this program "
                            "from root for create RAW socket\n");
    setBaseFlags(&targets->currentFlags);
    if (!init_addr(targets, argv[1]))
        return false;
    return true;
}

int     main(int argc, char *argv[])
{
    IcmpTargetType  targets;
    bool            res;

    res = true;
    signal(SIGALRM, setSignal);
    signal(SIGINT, setSignal);
    if (!baseInit(&targets, argc, argv))
        return ERROR_CODE;

    parseArgs(&targets, argc - 1, argv + 1, &res);
    if (targets.currentFlags.print_info)
        return printInfo();
    if (!res || !checkArgs(&targets.currentFlags))
        return ERROR_CODE;
    if (!bindingPacket(&targets))
        return ERROR_CODE;
    if (!ping(&targets))
        return ERROR_CODE;
    clearPacket(&targets);
    return SUCCESS_CODE;
}
