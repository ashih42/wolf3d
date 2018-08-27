/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 18:45:48 by ashih             #+#    #+#             */
/*   Updated: 2018/08/06 15:49:51 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			init_dda(t_dda *dda, t_master *m, int x)
{
	dda->camera_x = 2 * x / (float)WIN_WIDTH - 1;
	dda->ray_dir.x = m->players[m->pid].dir.x +
		m->players[m->pid].plane.x * dda->camera_x;
	dda->ray_dir.y = m->players[m->pid].dir.y +
		m->players[m->pid].plane.y * dda->camera_x;
	if (m->curve_mode)
		v_normalize(&dda->ray_dir);
	dda->map.x = (int)m->players[m->pid].pos.x;
	dda->map.y = (int)m->players[m->pid].pos.y;
	dda->delta_dist.x = ABS(1 / dda->ray_dir.x);
	dda->delta_dist.y = ABS(1 / dda->ray_dir.y);
	dda->step.x = (dda->ray_dir.x < 0) ? -1 : 1;
	dda->step.y = (dda->ray_dir.y < 0) ? -1 : 1;
	dda->side_dist.x = (dda->ray_dir.x < 0) ?
		(m->players[m->pid].pos.x - dda->map.x) * dda->delta_dist.x :
		(dda->map.x + 1 - m->players[m->pid].pos.x) * dda->delta_dist.x;
	dda->side_dist.y = (dda->ray_dir.y < 0) ?
		(m->players[m->pid].pos.y - dda->map.y) * dda->delta_dist.y :
		(dda->map.y + 1 - m->players[m->pid].pos.y) * dda->delta_dist.y;
}

void			run_dda(t_dda *dda, t_master *m)
{
	while (is_valid_pos(dda->map.x, dda->map.y, &m->map))
	{
		if (m->map.cell[dda->map.y][dda->map.x] != EMPTY_CELL)
		{
			dda->hit = 1;
			return ;
		}
		if (dda->side_dist.x < dda->side_dist.y)
		{
			dda->side_dist.x += dda->delta_dist.x;
			dda->map.x += dda->step.x;
			dda->hit_side = (dda->step.x < 0) ? EAST : WEST;
		}
		else
		{
			dda->side_dist.y += dda->delta_dist.y;
			dda->map.y += dda->step.y;
			dda->hit_side = (dda->step.y < 0) ? SOUTH : NORTH;
		}
	}
}
