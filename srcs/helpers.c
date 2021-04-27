#include "../includes/ft_ping.h"

int	printAndExit(const char *message, int code)
{
	printf("ping: %s\n", message);
	return (code);
}

BOOL	printExitWStr(const char *message, const char *body)
{
	printf("ping: %s %s\n", message, body);
	return (FALSE);
}

BOOL	printExitWInt(const char *message, int num)
{
	printf("ping: %s %d\n", message, num);
	return (FALSE);
}

uint16_t	GetCSum(const uint8_t *Buf, uint32_t Len, uint16_t StartVal)
{
	uint32_t		CSum;
	const uint16_t	*Ptr;
	uint32_t		i;

	i = 0;
	CSum = StartVal;
	if (Len & 0x1)
		CSum += Buf[Len - 1];
	Ptr = (const uint16_t *)Buf;
	Len >>= 1;
	while (i < Len)
		CSum += Ptr[i++];
	while (CSum >> 16)
		CSum = (CSum & 0xFFFF) + (CSum >> 16);
	return ((uint16_t)CSum);
}

int	count_not_of(const char *str, char symb)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i++] != symb)
			count++;
	}
	return (count);
}
