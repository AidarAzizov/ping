#include "../includes/ft_ping.h"

bool        printAndExit(const char* message)
{
    printf("ping: %s\n", message);
    return false;
}

int         printAndExit(const char* message, int code)
{
    printf("ping: %s\n", message);
    return code;
}

uint16_t    GetCSum(const uint8_t* Buf, uint32_t Len, uint16_t StartVal)
{
    uint32_t        CSum;
    const uint16_t* Ptr;

    CSum = StartVal;
    if (Len & 0x1)
        CSum += Buf[Len - 1];
    Ptr = (const uint16_t*)Buf;
    Len >>= 1;
    for (uint32_t i = 0; i < Len; i++)
        CSum += Ptr[i];
    while (CSum >> 16)
        CSum = (CSum & 0xFFFF) + (CSum >> 16);
    return (uint16_t)CSum;
}

bool        initTime(timeval* timeout)
{
    if (gettimeofday(timeout, NULL) != SUCCESS_CODE)
        return false;
    return true;
}

int         count_not_of(const char* str, char symb)
{
    int count;
    int i;

    count = 0;
    i = 0;
    while (str[i] != '\0')
    {
        if (str[i++] != symb)
            count++;
    }
    return count;
}
