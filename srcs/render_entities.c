/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_entities.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 20:50:39 by ashih             #+#    #+#             */
/*   Updated: 2018/08/25 18:29:22 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void			draw_entity(t_entity *e, t_master *m)
{
	if (!(e->alive))
		return ;
	init_dre(&m->dre, e, &m->players[m->pid], m);
	dre_th_master(m);
}

static void			count_entities(t_master *m, int *count,
	t_entity *entities[ENTITY_COUNT + PLAYER_COUNT])
{
	int				i;

	*count = 0;
	i = -1;
	while (++i < ENTITY_COUNT)
	{
		if (m->entities[i].alive)
			entities[(*count)++] = &(m->entities[i]);
	}
	i = -1;
	while (++i < PLAYER_COUNT)
	{
		if ((uint32_t)i != m->pid && m->players[i].connected)
		{
			m->renderers[i].alive = 1;
			m->renderers[i].pos = m->players[i].pos;
			m->renderers[i].tex = TEXTURE_COUNT + (i > 0);
			entities[(*count)++] = &(m->renderers[i]);
		}
	}
}

void				render_entities(t_master *m)
{
	int				i;
	int				j;
	int				count;
	t_entity		*entities[ENTITY_COUNT + PLAYER_COUNT];

	count_entities(m, &count, entities);
	i = -1;
	while (++i < count)
		entities[i]->dist_player =
			SQ(entities[i]->pos.x - m->players[m->pid].pos.x) +
			SQ(entities[i]->pos.y - m->players[m->pid].pos.y);
	i = -1;
	while (++i < count - 1)
	{
		j = i;
		while (++j < count)
			if (entities[i]->dist_player < entities[j]->dist_player)
				SWAP(entities[i], entities[j], t_entity *);
	}
	i = -1;
	while (++i < count)
		draw_entity(entities[i], m);
}
