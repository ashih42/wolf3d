/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_th.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/26 04:49:01 by ashih             #+#    #+#             */
/*   Updated: 2018/08/08 19:19:24 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	render_th(t_master *m, int i)
{
	int	x;
	int	limit;

	x = WIN_WIDTH * i / THREAD_COUNT;
	limit = WIN_WIDTH * (i + 1) / THREAD_COUNT;
	while (x < limit)
		render_vert(x++, m);
}

void	render_th_master(t_master *m)
{
	int	i;

	i = -1;
	while (++i < THREAD_COUNT)
	{
		m->draw_state[i] = DRAW_TH_RENDER;
		pthread_mutex_unlock(&(m->draw_mutexes[i]));
	}
	i = -1;
	while (++i < THREAD_COUNT)
	{
		while (m->draw_state[i] != DRAW_TH_IDLE)
			;
		pthread_mutex_lock(&(m->draw_mutexes[i]));
	}
}
