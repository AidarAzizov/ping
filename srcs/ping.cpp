#include "../includes/ft_ping.h"

extern mypair work;

bool    makeDelay(double delay_sec, bool *is_firstIter)
{
    struct timeval  beg;
    struct timeval  curr;
    double          deltatime;

    if (*is_firstIter)
    {
        *is_firstIter = false;
        return true;
    }
    initTime(&beg);
    while (1488 && !work.alreadyend)
    {
        initTime(&curr);
        deltatime = (double)(curr.tv_sec - beg.tv_sec) +
                (double)(curr.tv_usec - beg.tv_usec)/1000000.;
        if (deltatime >= delay_sec)
            break;
    }
    if (work.alreadyend)
        return false;
    return true;
}

bool    run(IcmpTargetType *targets)
{
    int delta;

    delta = targets->timeafter.tv_sec - targets->timebegin.tv_sec;
    if (work.alreadyend)
        return false;
    if ((targets->currentFlags.infinity_c || targets->currentFlags.count_c-- > 0) &&
        (targets->currentFlags.infinity_w || targets->currentFlags.count_w > delta))
        return true;
    return false;
}

bool pingPrepare(IcmpTargetType *targets)
{
    nullifyTargets(targets);
    if (!createSocket(targets))
        return false;
    if (!prepareTargets(targets))
        return false;
    if (!initTime(&targets->timebegin))
        return false;
    if (!initTime(&targets->timeafter))
        return false;
    printHead(targets);
    return true;
}

bool    ping(IcmpTargetType *targets)
{
    bool firstIter;

    firstIter = true;
    if (!pingPrepare(targets))
        return false;
    while (run(targets))
    {
        if (!makeDelay(targets->currentFlags.delay_time, &firstIter))
            continue;
        recyclePacket(targets);
        if (!sender(targets))
            continue;
        if (!initTime(&targets->timebefore))
            return false;
        alarm(targets->currentFlags.count_w);
        if (!receiver(targets))
            continue;
        if (!initTime(&targets->timeafter))
            return false;
        printBody(targets);
    }
    if (!initTime(&targets->timeend))
        return false;
    printTail(targets);
    return true;
}

