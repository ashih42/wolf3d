/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/03 12:02:49 by ashih             #+#    #+#             */
/*   Updated: 2018/08/25 18:11:09 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"
#include "netplay.h"

static void		rand_pos(t_entity *e, t_map *map)
{
	t_vect2	pos;

	ft_bzero(&pos, sizeof(t_vect2));
	while (!is_valid_move(pos.x, pos.y, map))
	{
		pos.x = rand() % (map->width - 2) + 1.5;
		pos.y = rand() % (map->height - 2) + 1.5;
	}
	e->pos.x = pos.x;
	e->pos.y = pos.y;
}

void			destroy_entity(int id, t_master *m)
{
	m->entities[id].alive = 0;
	if (m->socket >= 0 && !(m->client))
		dispatch_entity_packet(id, &(m->entities[id]), m);
}

t_entity		*spawn_entity(int tex, t_vect2 *pos, t_master *m)
{
	int	i;

	i = 0;
	while (i < ENTITY_COUNT)
	{
		if (!(m->entities[i].alive))
			break ;
		i++;
	}
	if (i >= ENTITY_COUNT)
		return (NULL);
	m->entities[i].alive = 1;
	m->entities[i].is_moving = 0;
	m->entities[i].tex = tex;
	if (pos)
		m->entities[i].pos = *pos;
	else
		rand_pos(&(m->entities[i]), &(m->map));
	if (m->socket >= 0 && !(m->client))
		dispatch_entity_packet(i, &(m->entities[i]), m);
	return (&(m->entities[i]));
}

int				init_entities(t_master *m)
{
	uint32_t	i;

	i = 0;
	while (i < 10)
	{
		spawn_entity(rand() % TEXTURE_COUNT, NULL, m);
		i++;
	}
	return (0);
}
