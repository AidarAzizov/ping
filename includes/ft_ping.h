#ifndef FT_PING_H
# define FT_PING_H

# include <string.h>
# include <stdio.h>
# include <stdint.h>
# include <time.h>

# include <stdlib.h>
# include <errno.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <netinet/ip_icmp.h>
# include <netinet/ip.h>
# include <netinet/in.h>

# include <arpa/inet.h>
# include <sys/select.h>
# include <netdb.h>
# include <signal.h>
# include <math.h>
# include <ifaddrs.h>

# define SUCCESS_CODE 0
# define ERROR_CODE 1

# define BOOL int
# define TRUE 1
# define FALSE 0

# define LOCALHOST "0.0.0.0"

# ifndef DBL_MAX
#  define DBL_MAX 1.79769e+308
# endif

# ifndef DBL_MIN
#  define DBL_MIN 2.22507e-308
# endif

# define ROOT_UID 0
# define SOCKET int

typedef struct s_ip_addr
{
	uint8_t	i1;
	uint8_t	i2;
	uint8_t	i3;
	uint8_t	i4;
}	t_ip_addr;

typedef struct s_flags
{
	uint64_t	s;
	uint64_t	t;
	uint64_t	c;
	uint64_t	w;
	double		dt;
	BOOL		inf_c;
	BOOL		inf_w;
	BOOL		info;
	BOOL		more_info;
}	t_flags;

typedef struct s_mdev
{
	double	sum;
	double	sum2;
}	t_mdev;

typedef struct s_pingstat
{
	double			min;
	double			avg;
	double			max;
	t_mdev			mdev;
	uint32_t		received;
	struct timeval	times;
}	t_pingstat;

typedef struct s_mypair
{
	BOOL	stillwait;
	BOOL	alreadyend;
}	t_mypair;

typedef struct s_IcmpTargetType
{
	t_pingstat		pinfo;
	t_flags			currFl;
	struct iphdr	*_ip;
	struct icmphdr	*_icmp;
	struct addrinfo	*_addrinfo;
	struct addrinfo	hints;
	struct iovec	iov;
	struct msghdr	msg;
	struct timeval	timebef;
	struct timeval	timeaft;
	SOCKET			_socket;
	struct timeval	timebegin;
	struct timeval	timeend;
	uint16_t		uid;
	BOOL			addr_from_arg_host;
	char			*addr_from_arg;
	char			dst_addr[INET_ADDRSTRLEN];
	char			src_addr[INET_ADDRSTRLEN];
	char			*packToRecv;
	char			*packToSend;
	uint16_t		packlen;
}	t_IcmpTargetType;

BOOL		INITED(int x);
double		secondTernare(double tone, double ttwo);
double		firstTernare(double tone, double ttwo);

int			printExitWStr(const char *message, const char *body);
BOOL		printExitWInt(const char *message, int num);

void		printHead(t_IcmpTargetType *targets);
void		printBody(t_IcmpTargetType *target);
void		printTail(t_IcmpTargetType *target);

BOOL		createSocket(t_IcmpTargetType *target);

BOOL		init_addr(t_IcmpTargetType *targets, const char *arg);
void		parseArgs(t_IcmpTargetType *targets,
				int argc, char *argv[], BOOL *result);

int			count_not_of(const char *str, char symb);
BOOL		initTime(struct timeval *timeout);
int			printAndExit(const char *message, int code);
uint16_t	GetCSum(const uint8_t *Buf, uint32_t Len, uint16_t StartVal);

BOOL		prepareTargets(t_IcmpTargetType *target);
void		nullifyTargets(t_IcmpTargetType *target);

int			printInfo(void);
void		recyclePacket(t_IcmpTargetType *target);
void		clearPacket(t_IcmpTargetType *target);
BOOL		bindingPacket(t_IcmpTargetType *targets);

BOOL		sender(t_IcmpTargetType *targets);
BOOL		receiver(t_IcmpTargetType *target);

BOOL		ping(t_IcmpTargetType *targets);

#endif
