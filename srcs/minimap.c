/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/24 03:52:56 by ashih             #+#    #+#             */
/*   Updated: 2018/08/08 19:18:48 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			render_mm_th(t_master *m, int i)
{
	t_vect2i	pos;
	int			limit;

	pos.y = MM_WIDTH * i / THREAD_COUNT;
	limit = MM_WIDTH * (i + 1) / THREAD_COUNT;
	while (pos.y < limit)
	{
		pos.x = -1;
		while (++pos.x < MM_WIDTH)
			draw_mm(pos.x, pos.y, m);
		pos.y++;
	}
}

void			render_mm_th_master(t_master *m)
{
	int			i;

	if (!m->minimap)
		return ;
	i = -1;
	while (++i < THREAD_COUNT)
	{
		m->draw_state[i] = DRAW_TH_MM;
		pthread_mutex_unlock(&(m->draw_mutexes[i]));
	}
	i = -1;
	while (++i < THREAD_COUNT)
	{
		while (m->draw_state[i] != DRAW_TH_IDLE)
			;
		pthread_mutex_lock(&(m->draw_mutexes[i]));
	}
	draw_mm_border(m);
	draw_mm_player(m);
}
