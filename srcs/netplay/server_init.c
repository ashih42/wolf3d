/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuel <apuel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/28 13:51:47 by apuel             #+#    #+#             */
/*   Updated: 2018/08/28 14:08:53 by apuel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netplay.h"

int		server_init(t_master *m)
{
	if (!(m->client))
	{
		if (read_map_file(m->map_name, m) || init_entities(m))
			return (-1);
	}
	else if (start_client(m, m->ip, m->port))
	{
		ft_printf("[Client] %s\n", strerror(errno));
		return (-1);
	}
	if (m->mode == 's' && start_server(m, m->port))
	{
		ft_printf("[Server] %s\n", strerror(errno));
		return (-1);
	}
	pthread_mutex_init(&(m->mutex), NULL);
	if (m->socket >= 0 && pthread_create(&(m->thread), NULL, packet_thread, m))
		return (-1);
	return (0);
}

void	server_deinit(t_master *m)
{
	uint32_t	i;

	if (m->socket >= 0)
	{
		m->terminate = 1;
		pthread_join(m->thread, 0);
		if (m->client)
			send_disconnect_packet(m->socket);
		else
		{
			i = 1;
			while (i < PLAYER_COUNT)
			{
				if (m->players[i].connected)
					disconnect_player(i, m);
				i++;
			}
		}
		shutdown(m->socket, 2);
		ft_putstr("Disconnected.\n");
	}
}
