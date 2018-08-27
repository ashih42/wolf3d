/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/03 09:52:40 by ashih             #+#    #+#             */
/*   Updated: 2018/08/07 02:04:25 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void			draw_floor_ceil(t_floor *floor, t_master *m)
{
	floor->tex = m->floor_tex[m->floor_mode];
	floor->tex_x = (int)(floor->curr.x * floor->tex->width) % floor->tex->width;
	floor->tex_y = (int)(floor->curr.y * floor->tex->height) %
		floor->tex->height;
	draw_dot(floor->x, floor->y, floor->tex->color[floor->tex->height *
		floor->tex_x + floor->tex_y].rgb, m);
	floor->tex_x = (int)(floor->curr.x * m->ceil_tex->width / 4) %
		m->ceil_tex->width;
	floor->tex_y = (int)(floor->curr.y * m->ceil_tex->height / 4) %
		m->ceil_tex->height;
	draw_dot(floor->x, WIN_HEIGHT - floor->y, m->ceil_tex->color[
		m->ceil_tex->height * floor->tex_x + floor->tex_y].rgb, m);
}

static void			draw_floor(t_floor *floor, t_master *m)
{
	while (floor->y < WIN_HEIGHT)
	{
		floor->curr_dist = WIN_HEIGHT / (2.0f * floor->y - WIN_HEIGHT);
		floor->weight = (floor->curr_dist - floor->dist_player) /
			(floor->dist_wall - floor->dist_player);
		floor->curr.x = floor->weight * floor->wall_pos.x +
			(1.0f - floor->weight) * m->players[m->pid].pos.x;
		floor->curr.y = floor->weight * floor->wall_pos.y +
			(1.0f - floor->weight) * m->players[m->pid].pos.y;
		draw_floor_ceil(floor, m);
		floor->y++;
	}
}

static void			init_floor(t_floor *floor, t_dda *dda, t_wall *wall)
{
	if (dda->hit_side == WEST)
	{
		floor->wall_pos.x = (float)dda->map.x;
		floor->wall_pos.y = (float)dda->map.y + wall->hit_x;
	}
	else if (dda->hit_side == EAST)
	{
		floor->wall_pos.x = (float)dda->map.x + 1.0f;
		floor->wall_pos.y = (float)dda->map.y + wall->hit_x;
	}
	else if (dda->hit_side == NORTH)
	{
		floor->wall_pos.x = (float)dda->map.x + wall->hit_x;
		floor->wall_pos.y = (float)dda->map.y;
	}
	else
	{
		floor->wall_pos.x = (float)dda->map.x + wall->hit_x;
		floor->wall_pos.y = (float)dda->map.y + 1.0f;
	}
	floor->dist_wall = wall->dist_from_ray;
	floor->dist_player = 0.0f;
	if (wall->end < 0)
		wall->end = WIN_HEIGHT;
}

void				floor_cast(t_dda *dda, t_wall *wall, t_master *m, int x)
{
	t_floor			floor;

	ft_bzero(&floor, sizeof(t_floor));
	init_floor(&floor, dda, wall);
	floor.x = x;
	floor.y = wall->end + 1.0f;
	draw_floor(&floor, m);
}
