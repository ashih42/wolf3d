/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuel <apuel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/05 17:34:14 by apuel             #+#    #+#             */
/*   Updated: 2018/08/28 14:16:52 by apuel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netplay.h"

#define SOCKOPT_NAME (SO_REUSEADDR | SO_REUSEPORT)

static uint32_t	create_new_player(t_master *m)
{
	uint32_t	i;

	i = 1;
	while (i < PLAYER_COUNT)
	{
		if (!(m->players[i].connected))
			break ;
		i++;
	}
	if (i == PLAYER_COUNT)
		return (0);
	m->players[i].pos.x = (float)(m->map_x) + 0.5f;
	m->players[i].pos.y = (float)(m->map_y) + 0.5f;
	m->players[i].default_dir.x = -1.0f;
	m->players[i].default_dir.y = 0.0f;
	m->players[i].default_plane.x = 0.0f;
	m->players[i].default_plane.y = -0.66f;
	m->players[i].angle = 0.0f;
	m->players[i].connected = 1;
	dispatch_player_packet(i, &(m->players[i]), m, 0);
	return (i);
}

static int		send_initial_packets(int socket, uint32_t id, t_master *m)
{
	uint32_t	i;
	int			result;

	result = 0;
	if (send_player_packet(socket, id, &(m->players[id])) < 0)
		return (-1);
	i = 0;
	while (result >= 0 && i < ENTITY_COUNT)
	{
		if (m->entities[i].alive)
			result = send_entity_packet(socket, i, &(m->entities[i]));
		i++;
	}
	i = 0;
	while (result >= 0 && i < PLAYER_COUNT)
	{
		if (i != id && m->players[i].connected)
			result = send_player_packet(socket, i, &(m->players[i]));
		i++;
	}
	if (result < 0)
		return (result);
	if (send_map_packet(socket, m->map_name) < 0)
		return (-1);
	return (0);
}

void			listener_task(t_master *m)
{
	int			socket;
	socklen_t	len;
	uint32_t	id;

	len = sizeof(struct sockaddr_in);
	socket = accept(m->socket, (struct sockaddr *)&(m->addr), &len);
	if (socket >= 0)
	{
		ft_putstr("[Server] Established connection...\n");
		pthread_mutex_lock(&(m->mutex));
		if ((id = create_new_player(m)))
		{
			m->connections[id] = socket;
			m->connected++;
			ft_printf("[Server] Player %u connected!\n", id + 1);
			if (send_initial_packets(socket, id, m) < 0)
				disconnect_player(id, m);
		}
		else
			ft_putstr("[Server] Player limit exceeded!\n");
		pthread_mutex_unlock(&(m->mutex));
	}
}

int				start_server(t_master *m, int port)
{
	int			opt;
	socklen_t	len;

	ft_putstr("[Server] Starting listener...\n");
	opt = 1;
	ft_bzero(&(m->addr), sizeof(struct sockaddr_in));
	m->socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m->socket < 0)
		return (-1);
	if (setsockopt(m->socket, SOL_SOCKET, SOCKOPT_NAME, &opt, sizeof(int)))
	{
		m->addr.sin_family = AF_INET;
		m->addr.sin_addr.s_addr = INADDR_ANY;
		m->addr.sin_port = htons(port);
		len = sizeof(struct sockaddr_in);
		if (bind(m->socket, (struct sockaddr *)&(m->addr), len) < 0)
			return (-1);
		if (listen(m->socket, 3) < 0)
			return (-1);
		fcntl(m->socket, F_SETFL, O_NONBLOCK);
		ft_printf("[Server] Listening on port %d.\n", port);
		return (0);
	}
	return (-1);
}
