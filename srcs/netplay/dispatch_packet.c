/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch_packet.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuel <apuel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 12:53:40 by apuel             #+#    #+#             */
/*   Updated: 2018/08/28 14:20:47 by apuel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netplay.h"

static void	*craft_entity_packet(uint32_t id, t_entity *e, uint32_t *size)
{
	t_packet_hdr	*hdr;
	t_entity_pckt	*packet;

	hdr = malloc(sizeof(t_packet_hdr) + sizeof(t_entity_pckt));
	hdr->id = ENTITY_UPDATE;
	hdr->size = sizeof(t_entity_pckt);
	packet = (t_entity_pckt *)(hdr + 1);
	packet->id = id;
	packet->data = *e;
	*size = sizeof(t_packet_hdr) + sizeof(t_entity_pckt);
	return (hdr);
}

void		dispatch_entity_packet(uint32_t id, t_entity *e, t_master *m)
{
	uint32_t	i;
	uint32_t	sent;
	void		*packet;
	uint32_t	size;

	packet = craft_entity_packet(id, e, &size);
	if (m->client)
	{
		if (send(m->socket, packet, size, 0) < 0)
			send_disconnect_packet(m->socket);
	}
	else if (m->connected > 0)
	{
		i = 0;
		sent = 0;
		while (sent < m->connected && ++i < PLAYER_COUNT)
		{
			if (!(m->players[i].connected))
				continue ;
			if (send(m->connections[i], packet, size, 0) < 0)
				disconnect_player(i, m);
			sent++;
		}
	}
	free(packet);
}

static void	*craft_player_packet(uint32_t id, t_player *p, uint32_t *size)
{
	t_packet_hdr	*hdr;
	t_player_pckt	*packet;

	hdr = malloc(sizeof(t_packet_hdr) + sizeof(t_player_pckt));
	hdr->id = PLAYER_UPDATE;
	hdr->size = sizeof(t_player_pckt);
	packet = (t_player_pckt *)(hdr + 1);
	packet->id = id;
	packet->data = *p;
	*size = sizeof(t_packet_hdr) + sizeof(t_player_pckt);
	return (hdr);
}

void		dispatch_player_packet(
	uint32_t id, t_player *p, t_master *m, int send_to_self)
{
	uint32_t	i;
	uint32_t	sent;
	void		*packet;
	uint32_t	size;

	i = 0;
	sent = 0;
	packet = craft_player_packet(id, p, &size);
	if (m->client)
	{
		if (send(m->socket, packet, size, 0) < 0)
			send_disconnect_packet(m->socket);
	}
	else if (m->connected > 0)
		while (sent < m->connected && ++i < PLAYER_COUNT)
		{
			if (!(m->players[i].connected))
				continue ;
			if ((send_to_self || i != id) &&
				send(m->connections[i], packet, size, 0) < 0)
				disconnect_player(i, m);
			sent++;
		}
	free(packet);
}
