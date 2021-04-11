#include "../includes/ft_ping.h"

bool    HostToIP(IcmpTargetType *targets)
{
    struct sockaddr_in *in;
    struct addrinfo hints;
    struct addrinfo *res;
    int family;

    res = NULL;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    memset(targets->dst_addr, 0, INET_ADDRSTRLEN);
    if (getaddrinfo(targets->addr_from_arg, NULL, &hints, &res) != 0)
    {
        if (res) freeaddrinfo(res);
        return printAndExit((std::string(targets->addr_from_arg) +
            ": Name or service not known").c_str());
    }
    family = res->ai_family;
    in = (sockaddr_in*)res->ai_addr;
    free(res);
    if (!inet_ntop(family, &(in->sin_addr), targets->dst_addr, sizeof(targets->dst_addr)))
        return printAndExit(("Error inet_ntop() for " + std::string(targets->addr_from_arg)).c_str());
    return true;
}

bool    init_count(uint64_t* res, int *index, int argc, char* argv[])
{
    char flag;
    char symb;
    int64_t checker = 0;

    if (sscanf(argv[*index], " -%c %ld %c ", &flag, res, &symb) == 2)
    {
        sscanf(argv[*index], " -%c %ld %c ", &flag, &checker, &symb);
        if (*res != checker)
            return printAndExit(("invalid argument: " + std::to_string(checker)).c_str());
        return true;
    }
    (*index)++;
    if (*index >= argc)
        return printAndExit(("option requires an argument -- " + std::string(argv[(*index) - 1])).c_str());
    if (sscanf(argv[*index], " %ld %c ", res, &symb) != 1)
        return printAndExit(("invalid argument: " + std::string(argv[*index])).c_str());
    sscanf(argv[*index], " %ld %c ", &checker, &symb);
    if (*res != checker)
        return printAndExit(("invalid argument: " + std::to_string(checker)).c_str());
    return true;
}
bool    init_delaytime(double* res, int *index, int argc, char* argv[])
{
    char symb;

    if (count_not_of(argv[*index], ' ') > 3 &&
        sscanf(argv[*index], " -dt %lf %c ", res, &symb) == 1)
        return true;
    (*index)++;
    if (*index >= argc)
        return printAndExit(("option requires an argument -- '" + std::string(argv[(*index) - 1]) + "'").c_str());
    if (sscanf(argv[*index], " %lf %c ", res, &symb) != 1)
        return printAndExit(("invalid argument: '" + std::string(argv[*index]) + "'").c_str());
    return true;
}

bool    init_addr(IcmpTargetType *targets, const char* arg)
{
    char    symb;
    ip_addr IPcheck;

    targets->addr_from_arg = (char*)malloc(sizeof(char) * NI_MAXHOST);
    if (!targets->addr_from_arg)
        return printAndExit("Bad with allocate memory");
    targets->addr_from_arg_host = false;
    memset(targets->addr_from_arg, 0, NI_MAXHOST);
    if (sscanf(arg, "%hhu.%hhu.%hhu.%hhu%c",
               &IPcheck.i1, &IPcheck.i2, &IPcheck.i3, &IPcheck.i4, &symb) == 4)
    {
        strcpy(targets->addr_from_arg, arg);
        strcpy(targets->dst_addr, arg);
        return true;
    }
    else if (sscanf(arg, " %s %c", targets->addr_from_arg, &symb) == 1)
        return (targets->addr_from_arg_host = HostToIP(targets));
    free(targets->addr_from_arg);
    return printAndExit((std::string(arg) + ": Name or service not known").c_str());
}

void    parseArgs(IcmpTargetType *targets, int argc, char *argv[], bool* result)
{
    int i;

    i = 0;
    while (!targets->currentFlags.print_info && result && ++i < argc)
        if (strncmp(argv[i], "-h", 2) == 0)
            targets->currentFlags.print_info = true;
        else if (strncmp(argv[i], "-v", 2) == 0)
            targets->currentFlags.more_info = true;
        else if (strncmp(argv[i], "-w", 2) == 0)
            targets->currentFlags.infinity_w = !(*result =
                    init_count(&targets->currentFlags.count_w, &i, argc, argv));
        else if (strncmp(argv[i], "-s", 2) == 0)
            *result = init_count(&targets->currentFlags.count_s, &i, argc, argv);
        else if (strncmp(argv[i], "-t", 2) == 0)
            *result = init_count(&targets->currentFlags.count_t, &i, argc, argv);
        else if (strncmp(argv[i], "-c", 2) == 0)
            targets->currentFlags.infinity_c = !(*result =
                    init_count(&targets->currentFlags.count_c, &i, argc, argv));
        else if (strncmp(argv[i], "-dt", 3) == 0)
            *result = init_delaytime(&targets->currentFlags.delay_time, &i, argc, argv);
        else
            *result = printAndExit(("invalid argument: "
                    + std::string(argv[i])).c_str(), 0);
}
