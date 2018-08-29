/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_entities.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuel <apuel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 18:09:12 by ashih             #+#    #+#             */
/*   Updated: 2018/08/28 20:03:11 by apuel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"
#include "netplay.h"

/*
** Try to move one unit (MOV_INC) toward entity's current direction
*/

static int		move_entity(t_entity *e, t_master *m)
{
	t_vect2		target;
	float		mov_inc;

	mov_inc = m->mov_inc;
	if (e->tex == BULLET_TEXTURE)
		mov_inc *= 2.0f;
	target.x = e->pos.x + e->dir.x * mov_inc;
	if (is_valid_move(target.x + (e->dir.x > 0 ?
		MOV_LOOKAHEAD : -MOV_LOOKAHEAD), e->pos.y, &m->map))
		e->pos.x = target.x;
	else
		return (0);
	target.y = e->pos.y + e->dir.y * mov_inc;
	if (is_valid_move(e->pos.x, target.y + (e->dir.y > 0 ?
		MOV_LOOKAHEAD : -MOV_LOOKAHEAD), &m->map))
		e->pos.y = target.y;
	else
		return (0);
	return (1);
}

static void		try_collide(int id, t_master *m)
{
	uint32_t	i;
	float		dist_bullet;

	i = 0;
	while (i < ENTITY_COUNT)
	{
		if (m->entities[i].alive && m->entities[i].tex != BULLET_TEXTURE)
		{
			dist_bullet =
				SQ(m->entities[i].pos.x - m->entities[id].pos.x) +
				SQ(m->entities[i].pos.y - m->entities[id].pos.y);
			if (dist_bullet < 0.5)
			{
				destroy_entity(id, m);
				destroy_entity(i, m);
			}
		}
		i++;
	}
}

static void		update_entity(int id, t_master *m)
{
	if (!(m->entities[id].is_moving) && rand() % 30 == 0)
	{
		m->entities[id].is_moving = 1;
		rand_dir(&(m->entities[id].dir));
	}
	if (!(m->entities[id].is_moving))
		return ;
	if (!move_entity(&(m->entities[id]), m))
	{
		m->entities[id].is_moving = 0;
		if (m->entities[id].tex == BULLET_TEXTURE)
			destroy_entity(id, m);
	}
	else if (m->entities[id].tex == BULLET_TEXTURE)
		try_collide(id, m);
}

void			update_entities(t_master *m)
{
	uint32_t	i;
	t_vect2		pos;

	i = 0;
	while (i < ENTITY_COUNT)
	{
		if (m->entities[i].alive)
		{
			pos = m->entities[i].pos;
			update_entity(i, m);
			if (m->entities[i].alive && m->socket >= 0 && !(m->client) &&
				(pos.x != m->entities[i].pos.x ||
				pos.y != m->entities[i].pos.y))
				dispatch_entity_packet(i, &(m->entities[i]), m);
		}
		i++;
	}
}
