/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 19:25:34 by ashih             #+#    #+#             */
/*   Updated: 2018/08/25 19:25:55 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"
#include "netplay.h"

static void		move_along_dir(t_master *m, t_vect2 dir, float dist)
{
	t_vect2		target;

	target.x = m->players[m->pid].pos.x + dir.x * dist;
	if (is_valid_move((int)target.x, (int)m->players[m->pid].pos.y, &m->map))
		m->players[m->pid].pos.x = target.x;
	target.y = m->players[m->pid].pos.y + dir.y * dist;
	if (is_valid_move((int)m->players[m->pid].pos.x, (int)target.y, &m->map))
		m->players[m->pid].pos.y = target.y;
}

void			update_player_position(t_master *m)
{
	t_vect2		dir;
	float		dist;

	if (m->forward)
	{
		dist = ((m->rightward) ? 0.7f : 1.0f) * m->forward * m->mov_inc;
		move_along_dir(m, m->players[m->pid].dir, dist);
	}
	if (m->rightward)
	{
		dist = ((m->rightward) ? 0.7f : 1.0f) * m->rightward * m->mov_inc;
		v_rotate(&m->players[m->pid].dir, &dir, 90.0f);
		move_along_dir(m, dir, dist);
	}
	if ((m->forward || m->rightward) && m->socket >= 0)
		dispatch_player_packet(m->pid, &(m->players[m->pid]), m, 0);
}

void			update_player_rotation(t_master *m)
{
	float		rot_inc;

	rot_inc = ((ROT_INC) * 60.0f) * (float)(m->curr_time - m->prev_time);
	if (m->rotation > 0)
		rotate_left(m, rot_inc);
	else if (m->rotation < 0)
		rotate_right(m, rot_inc);
}
