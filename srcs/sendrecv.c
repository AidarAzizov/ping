#include "../includes/ft_ping.h"

extern t_mypair	g_work;

BOOL	sender(t_IcmpTargetType *targets)
{
	struct sockaddr_in	connection;
	int					howSended;
	int					len;

	connection.sin_port = targets->_addrinfo->ai_protocol;
	connection.sin_family = targets->_addrinfo->ai_family;
	connection.sin_addr.s_addr
		= ((struct sockaddr_in *)&targets->_addrinfo->ai_addr)->sin_addr.s_addr;
	len = targets->packlen;
	howSended = sendto(targets->_socket, targets->packToSend, len, 0,
			(struct sockaddr *)&connection, sizeof(struct sockaddr *));
	if (howSended == len)
		return (TRUE);
	if (howSended == -1)
		return (printAndExit(strerror(errno), FALSE));
	return (printAndExit("Bad try to send packet", FALSE));
}

BOOL	receiver(t_IcmpTargetType *target)
{
	ssize_t		howReceived;

	howReceived = -1;
	while (g_work.stillwait && !g_work.alreadyend && howReceived == -1)
		howReceived = recvmsg(target->_socket, &target->msg, MSG_DONTWAIT);
	alarm(0);
	if (g_work.alreadyend)
		return (FALSE);
	if (howReceived == target->packlen)
		return (TRUE);
	if (howReceived == -1)
		return (printAndExit(strerror(errno), FALSE));
	return (printAndExit("Bad try to receive packet", FALSE));
}
