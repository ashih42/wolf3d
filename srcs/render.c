/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 18:45:48 by ashih             #+#    #+#             */
/*   Updated: 2018/08/08 19:19:05 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void			wobble(t_master *m)
{
	static float	angle = 0;
	static float	inc = 1;
	t_vect2			def_plane;

	if (m->wobble_mode == 0)
		return ;
	else if (m->wobble_mode == 1)
	{
		def_plane.x = 0.0f;
		def_plane.y = -0.66f;
		angle += (inc) ? WOB_RATE : -WOB_RATE;
		if (angle >= 80.0f || angle <= -80.0f)
			inc = !inc;
		v_rotate(&def_plane, &m->players[m->pid].default_plane, angle);
	}
	else
		v_rotate(&m->players[m->pid].default_plane,
			&m->players[m->pid].default_plane, WOB_RATE);
}

static void			update_player_view(t_master *m)
{
	wobble(m);
	v_rotate(&m->players[m->pid].default_dir, &m->players[m->pid].dir,
		m->players[m->pid].angle);
	v_rotate(&m->players[m->pid].default_plane, &m->players[m->pid].plane,
		m->players[m->pid].angle);
}

void				render_vert(int x, t_master *m)
{
	t_dda			dda;
	t_wall			wall;

	init_dda(&dda, m, x);
	run_dda(&dda, m);
	if (dda.hit)
	{
		build_wall(&dda, &wall, m, x);
		floor_cast(&dda, &wall, m, x);
	}
}

void				render(t_master *m)
{
	ft_bzero(m->frame, sizeof(int) * (WIN_WIDTH) * (WIN_HEIGHT));
	ft_bzero(m->z_buffer, sizeof(m->z_buffer));
	update_player_view(m);
	render_th_master(m);
	render_entities(m);
	render_mm_th_master(m);
}
