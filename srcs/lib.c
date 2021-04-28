#include "../includes/ft_ping.h"

size_t	ft_strlen(char const *s)
{
	size_t	count;

	count = 0;
	while (s[count] != '\0')
		count++;
	return (count);
}

int	ft_strncmp(const char *s1, const char *s2, unsigned int n)
{
	unsigned int	i;
	int				r;

	i = 0;
	while (s1[i] && s2[i] && i < n)
	{
		r = s1[i] - s2[i];
		if (r)
			return (r);
		i++;
	}
	return (0);
}

void	*ft_memset(void *destination, int c, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (destination);
	while (i < n)
	{
		((unsigned char *)destination)[i] = (unsigned char)c;
		i++;
	}
	return (destination);
}

char	*ft_strcpy(char *s1, const char *s2)
{
	size_t	cur;

	cur = 0;
	while (s2[cur] != '\0')
	{
		s1[cur] = s2[cur];
		cur++;
	}
	s1[cur] = '\0';
	return (s1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 || *s2)
	{
		if (*s1 != *s2)
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		s1++;
		s2++;
	}
	return (0);
}
