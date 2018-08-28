/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   netplay.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuel <apuel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/25 03:37:09 by apuel             #+#    #+#             */
/*   Updated: 2018/08/28 14:31:01 by apuel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netplay.h"

/*
**	Attempts to read a packet from the connected socket.
**	NULL result means there was no packet to be read.
**	On success, id and size are set and the packet is returned.
**	In the case of an error, (void *)-1 is returned.
*/

void			*recv_packet(int socket, t_packet_hdr *hdr)
{
	int				n_bytes;
	uint32_t		pos;
	void			*result;

	if (ioctl(socket, FIONREAD, &n_bytes) < 0)
		return ((void *)-1);
	if (n_bytes < (int)sizeof(t_packet_hdr))
		return (NULL);
	if (recv(socket, hdr, sizeof(t_packet_hdr), 0) < 0)
		return ((void *)-1);
	if (hdr->id <= MAX_PACKET_TYPE && hdr->size && (result = malloc(hdr->size)))
	{
		pos = 0;
		while ((n_bytes = recv(socket, &result[pos], hdr->size - pos, 0)) > 0)
		{
			pos += (uint32_t)n_bytes;
			if (pos == hdr->size)
				return (result);
		}
		free(result);
	}
	return ((void *)-1);
}

/*
**	Selects the next available client pid to attempt to read from.
*/

static uint32_t	select_client(t_master *m)
{
	static uint32_t	pid = 0;
	static uint32_t	selected = 0;

	if (selected >= m->connected)
	{
		pid = 0;
		selected = 0;
	}
	if (m->connected == 0)
		return (0);
	pid++;
	while (pid >= PLAYER_COUNT || !(m->players[pid].connected))
	{
		if (pid >= PLAYER_COUNT)
		{
			pid = 0;
			selected = 0;
		}
		pid++;
	}
	selected++;
	return (pid);
}

/*
**	Takes turns listening to incoming packets from all connections.
*/

static void		*query_packets(
	t_packet_hdr *hdr, uint32_t *sender, t_master *m)
{
	void			*result;
	uint32_t		pid;

	result = NULL;
	*sender = 0;
	if (m->client)
		result = recv_packet(m->socket, hdr);
	else if (m->connected > 0)
	{
		pid = select_client(m);
		result = recv_packet(m->connections[pid], hdr);
		*sender = pid;
	}
	if (result != (void *)-1)
		return (result);
	pthread_mutex_lock(&(m->mutex));
	if (m->client)
		m->terminate = 1;
	else
		disconnect_player(*sender, m);
	pthread_mutex_unlock(&(m->mutex));
	return (NULL);
}

/*
**	Dispatches packets based on id.
*/

void			packet_handler(
	t_packet_hdr *hdr, void *packet, uint32_t sender, t_master *m)
{
	static void		(*packet_f[MAX_PACKET_TYPE + 1])() = {
		map_load,
		player_update,
		entity_update,
		bullet_request
	};
	static uint32_t	packet_s[MAX_PACKET_TYPE + 1] = {
		sizeof(t_map_pckt),
		sizeof(t_player_pckt),
		sizeof(t_entity_pckt),
		sizeof(t_bullet_pckt)
	};

	if (hdr->size >= packet_s[hdr->id])
		packet_f[hdr->id](packet, sender, m);
}

/*
**	Listens for incoming packets and dispatches them based on their id.
**	On error, m->terminate will be set to a non-zero value and the thread
**	will exit.
*/

void			*packet_thread(void *arg)
{
	t_master		*m;
	void			*packet;
	t_packet_hdr	hdr;
	uint32_t		sender;

	m = arg;
	while (!(m->terminate))
	{
		if (!(packet = query_packets(&hdr, &sender, m)))
			continue ;
		pthread_mutex_lock(&(m->mutex));
		packet_handler(&hdr, packet, sender, m);
		pthread_mutex_unlock(&(m->mutex));
		free(packet);
	}
	return (NULL);
}
