/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recv_packet.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuel <apuel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 18:45:17 by apuel             #+#    #+#             */
/*   Updated: 2018/08/25 13:28:16 by apuel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netplay.h"

void	map_load(t_map_pckt *packet, uint32_t sender, t_master *m)
{
	if (sender == 0 && m->client)
	{
		if (read_map_file(packet->name, m))
			m->terminate = 1;
	}
}

void	player_update(t_player_pckt *packet, uint32_t sender, t_master *m)
{
	if (packet->id < PLAYER_COUNT)
	{
		if (packet->id == sender || sender == 0)
		{
			if (m->client && m->pid == 0)
				m->pid = packet->id;
			m->players[packet->id] = packet->data;
			m->players[packet->id].connected = 1;
			if (!(m->client))
				dispatch_player_packet(packet->id, &(packet->data), m, 0);
		}
	}
}

void	entity_update(t_entity_pckt *packet, uint32_t sender, t_master *m)
{
	if (sender == 0 && m->client && packet->id < ENTITY_COUNT)
		m->entities[packet->id] = packet->data;
}

void	bullet_request(t_bullet_pckt *packet, uint32_t sender, t_master *m)
{
	t_entity	*bullet;

	if (sender != 0 && !(m->client))
	{
		bullet = spawn_entity(BULLET_TEXTURE, &(packet->pos), m);
		if (bullet)
		{
			bullet->is_moving = 1;
			bullet->dir = packet->dir;
		}
	}
}

void	disconnect_player(uint32_t pid, t_master *m)
{
	if (m->players[pid].connected)
	{
		send_disconnect_packet(m->connections[pid]);
		m->players[pid].connected = 0;
		shutdown(m->connections[pid], 2);
		ft_printf("[Server] Player %u disconnected!\n", pid);
		m->connected--;
		dispatch_player_packet(pid, &(m->players[pid]), m, 0);
	}
}
