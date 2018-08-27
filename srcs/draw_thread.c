/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/05 16:06:23 by apuel             #+#    #+#             */
/*   Updated: 2018/08/25 17:44:16 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void		draw_something(t_master *m, int i, int *state)
{
	if (m->draw_state[i] == DRAW_TH_DRE)
	{
		m->draw_state[i] = DRAW_TH_IDLE;
		dre_th(m, i);
	}
	else if (m->draw_state[i] == DRAW_TH_RENDER)
	{
		m->draw_state[i] = DRAW_TH_IDLE;
		render_th(m, i);
	}
	else if (m->draw_state[i] == DRAW_TH_MM)
	{
		m->draw_state[i] = DRAW_TH_IDLE;
		render_mm_th(m, i);
	}
	*state = m->draw_state[i];
}

static void		*draw_f(void *args)
{
	t_master	*m;
	int			i;
	int			state;

	m = ((t_thread_args *)args)->m;
	i = ((t_thread_args *)args)->i;
	state = DRAW_TH_IDLE;
	while (state != DRAW_TH_EXIT)
	{
		pthread_mutex_lock(&(m->draw_mutexes[i]));
		draw_something(m, i, &state);
		pthread_mutex_unlock(&(m->draw_mutexes[i]));
	}
	return (NULL);
}

void			draw_thread_init(t_master *m)
{
	static t_thread_args	args[THREAD_COUNT];
	int						i;

	i = -1;
	while (++i < THREAD_COUNT)
	{
		args[i].m = m;
		args[i].i = i;
		m->draw_state[i] = DRAW_TH_IDLE;
		pthread_mutex_init(&(m->draw_mutexes[i]), NULL);
		pthread_mutex_lock(&(m->draw_mutexes[i]));
		pthread_create_suspended_np(&(m->draw_threads[i]), 0, draw_f, &args[i]);
		m->draw_ports[i] = pthread_mach_thread_np(m->draw_threads[i]);
		thread_policy_set(m->draw_ports[i], THREAD_AFFINITY_POLICY, &i, 1);
		thread_resume(m->draw_ports[i]);
	}
}

void			draw_thread_deinit(t_master *m)
{
	int			i;

	i = -1;
	while (++i < THREAD_COUNT)
	{
		m->draw_state[i] = DRAW_TH_EXIT;
		pthread_mutex_unlock(&(m->draw_mutexes[i]));
		pthread_join(m->draw_threads[i], 0);
	}
}
