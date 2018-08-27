/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuel <apuel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 22:09:27 by apuel             #+#    #+#             */
/*   Updated: 2018/08/25 14:55:47 by apuel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netplay.h"

static int	wait_for_packets(t_master *m)
{
	void			*packet;
	t_packet_hdr	hdr;

	while (!(m->terminate))
	{
		if (!(packet = recv_packet(m->socket, &hdr)))
			continue ;
		if (packet == (void *)-1)
			return (-1);
		if (hdr.id <= MAX_PACKET_TYPE)
			packet_handler(&hdr, packet, 0, m);
		free(packet);
		if (m->map_name)
			break ;
	}
	return (0);
}

int			start_client(t_master *m, char *ip, int port)
{
	int					rval;
	socklen_t			len;

	ft_printf("[Client] Connecting to %s:%d...\n", ip, port);
	ft_bzero(&(m->addr), sizeof(struct sockaddr_in));
	m->addr.sin_family = AF_INET;
	m->addr.sin_port = htons(port);
	rval = inet_pton(AF_INET, ip, &(m->addr.sin_addr));
	if (rval == 0 || rval < 0)
		return (-1);
	m->socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m->socket < 0)
		return (-1);
	len = sizeof(struct sockaddr_in);
	if (connect(m->socket, (struct sockaddr *)&(m->addr), len))
		return (-1);
	ft_putstr("[Client] Waiting for packets...\n");
	if (wait_for_packets(m))
	{
		shutdown(m->socket, 2);
		return (-1);
	}
	if (!(m->terminate))
		ft_putstr("[Client] Ready.\n");
	return (0);
}
