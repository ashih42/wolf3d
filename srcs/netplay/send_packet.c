/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_packet.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuel <apuel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 16:21:59 by apuel             #+#    #+#             */
/*   Updated: 2018/08/05 20:45:55 by apuel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "netplay.h"

int	send_map_packet(int socket, char *name)
{
	t_packet_hdr	*hdr;
	t_map_pckt		*packet;
	uint32_t		length;
	int				result;

	length = ft_strlen(name);
	hdr = malloc(sizeof(t_packet_hdr) + sizeof(t_map_pckt) + length + 1);
	hdr->id = MAP_LOAD;
	hdr->size = sizeof(t_map_pckt) + length + 1;
	packet = (t_map_pckt *)(hdr + 1);
	packet->length = length;
	ft_memcpy(packet->name, name, length + 1);
	result = send(socket, hdr, sizeof(t_packet_hdr) + hdr->size, 0);
	free(hdr);
	return (result);
}

int	send_player_packet(int socket, uint32_t id, t_player *p)
{
	t_packet_hdr	*hdr;
	t_player_pckt	*packet;
	int				result;

	hdr = malloc(sizeof(t_packet_hdr) + sizeof(t_player_pckt));
	hdr->id = PLAYER_UPDATE;
	hdr->size = sizeof(t_player_pckt);
	packet = (t_player_pckt *)(hdr + 1);
	packet->id = id;
	packet->data = *p;
	result = send(socket, hdr, sizeof(t_packet_hdr) + sizeof(t_player_pckt), 0);
	free(hdr);
	return (result);
}

int	send_entity_packet(int socket, uint32_t id, t_entity *e)
{
	t_packet_hdr	*hdr;
	t_entity_pckt	*packet;
	int				result;

	hdr = malloc(sizeof(t_packet_hdr) + sizeof(t_entity_pckt));
	hdr->id = ENTITY_UPDATE;
	hdr->size = sizeof(t_entity_pckt);
	packet = (t_entity_pckt *)(hdr + 1);
	packet->id = id;
	packet->data = *e;
	result = send(socket, hdr, sizeof(t_packet_hdr) + sizeof(t_entity_pckt), 0);
	free(hdr);
	return (result);
}

int	send_bullet_packet(int socket, t_vect2 *pos, t_vect2 *dir)
{
	t_packet_hdr	*hdr;
	t_bullet_pckt	*packet;
	int				result;

	hdr = malloc(sizeof(t_packet_hdr) + sizeof(t_bullet_pckt));
	hdr->id = FIRE_BULLET;
	hdr->size = sizeof(t_bullet_pckt);
	packet = (t_bullet_pckt *)(hdr + 1);
	packet->pos = *pos;
	packet->dir = *dir;
	result = send(socket, hdr, sizeof(t_packet_hdr) + sizeof(t_bullet_pckt), 0);
	free(hdr);
	return (result);
}

int	send_disconnect_packet(int socket)
{
	t_packet_hdr	*hdr;
	int				result;

	hdr = malloc(sizeof(t_packet_hdr));
	hdr->id = DISCONNECT;
	hdr->size = 0;
	result = send(socket, hdr, sizeof(t_packet_hdr), 0);
	free(hdr);
	return (result);
}
