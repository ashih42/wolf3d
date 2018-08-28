/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   netplay.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuel <apuel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/25 03:01:45 by apuel             #+#    #+#             */
/*   Updated: 2018/08/28 13:56:50 by apuel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETPLAY_H
# define NETPLAY_H

# include "wolf3d.h"
# include <unistd.h>
# include <stdint.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/ioctl.h>
# include <stdio.h>
# include <stdlib.h>
# include <arpa/inet.h>

# define MAP_LOAD 0
# define PLAYER_UPDATE 1
# define ENTITY_UPDATE 2
# define FIRE_BULLET 3
# define DISCONNECT 4
# define MAX_PACKET_TYPE 3

typedef struct	s_packet_hdr
{
	uint32_t	id;
	uint32_t	size;
}				t_packet_hdr;

/*
**	SERVER => CLIENT
*/
typedef struct	s_map_pckt
{
	uint32_t	length;
	char		name[];
}				t_map_pckt;

/*
**	SERVER <=> CLIENT
*/
typedef struct	s_player_pckt
{
	uint32_t	id;
	t_player	data;
}				t_player_pckt;

/*
**	SERVER => CLIENT
*/
typedef struct	s_entity_pckt
{
	uint32_t	id;
	t_entity	data;
}				t_entity_pckt;

/*
**	SERVER <= CLIENT
*/
typedef struct	s_bullet_pckt
{
	t_vect2		pos;
	t_vect2		dir;
}				t_bullet_pckt;

/*
**	netplay.c
*/
void			*recv_packet(int socket, t_packet_hdr *hdr);
void			packet_handler(
				t_packet_hdr *hdr, void *packet, uint32_t sender, t_master *m);
void			*packet_thread(void *arg);

/*
**	dispatch_packet.c
*/
void			dispatch_entity_packet(uint32_t id, t_entity *e, t_master *m);
void			dispatch_player_packet(
					uint32_t id, t_player *p, t_master *m, int send_to_self);

/*
**	server_init.c
*/
int				server_init(t_master *m);
void			server_deinit(t_master *m);

/*
**	server.c
*/
void			listener_task(t_master *m);
int				start_server(t_master *m, int port);

/*
**	client.c
*/
int				start_client(t_master *m, char *ip, int port);

/*
**	send_packet.c
*/
int				send_map_packet(int socket, char *name);
int				send_player_packet(int socket, uint32_t id, t_player *p);
int				send_entity_packet(int socket, uint32_t id, t_entity *e);
int				send_bullet_packet(int socket, t_vect2 *pos, t_vect2 *dir);
int				send_disconnect_packet(int socket);

/*
**	recv_packet.c
*/
void			map_load(
						t_map_pckt *packet, uint32_t sender, t_master *m);
void			player_update(
						t_player_pckt *packet, uint32_t sender, t_master *m);
void			entity_update(
						t_entity_pckt *packet, uint32_t sender, t_master *m);
void			bullet_request(
						t_bullet_pckt *packet, uint32_t sender, t_master *m);
void			disconnect_player(uint32_t pid, t_master *m);

#endif
