/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/03 10:34:43 by ashih             #+#    #+#             */
/*   Updated: 2018/08/03 10:48:48 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			enforce_range(int *a, int limit)
{
	int		t;

	t = *a % limit;
	if (t < 0)
		t += limit;
	*a = t;
}

static void		shift_color(t_wall *wall, int hit_side)
{
	int			i;

	i = -1;
	while (++i < hit_side)
		wall->color = (wall->color >> 1) & 0b011111110111111101111111;
}

static void		draw_wall(t_dda *dda, t_wall *wall, t_master *m, int x)
{
	int			y;
	int			d;

	y = wall->start;
	while (y <= wall->end)
	{
		d = y * 256 - (WIN_HEIGHT + wall->length) * 128;
		wall->tex_y = ((d * wall->tex->height) / wall->length) / 256;
		enforce_range(&wall->tex_x, wall->tex->width);
		enforce_range(&wall->tex_y, wall->tex->height);
		if (wall->tex->color[wall->tex->height * wall->tex_x
			+ wall->tex_y].alpha > ALPHA_CUTOFF)
			wall->color = wall->tex->color[wall->tex->height * wall->tex_x +
				wall->tex_y].rgb;
		else
			wall->color = 0xFFFFFF;
		shift_color(wall, dda->hit_side);
		draw_dot(x, y, wall->color, m);
		y++;
	}
}

static void		build_wall_2(t_dda *dda, t_wall *wall, t_master *m)
{
	wall->tex = find_asset(m->map.cell[dda->map.y][dda->map.x], m)->tex;
	if (dda->hit_side == WEST || dda->hit_side == EAST)
		wall->hit_x = m->players[m->pid].pos.y + wall->dist_from_ray *
			dda->ray_dir.y;
	else
		wall->hit_x = m->players[m->pid].pos.x + wall->dist_from_ray *
			dda->ray_dir.x;
	wall->hit_x -= floor((wall->hit_x));
	wall->tex_x = (int)(wall->hit_x * wall->tex->width);
	if ((dda->hit_side == WEST || dda->hit_side == EAST) && dda->ray_dir.x > 0)
		wall->tex_x = wall->tex->width - wall->tex_x - 1;
	if (!(dda->hit_side == WEST || dda->hit_side == EAST) && dda->ray_dir.y < 0)
		wall->tex_x = wall->tex->width - wall->tex_x - 1;
}

void			build_wall(t_dda *dda, t_wall *wall, t_master *m, int x)
{
	if (dda->hit_side == WEST || dda->hit_side == EAST)
		wall->dist_from_ray = (dda->map.x - m->players[m->pid].pos.x +
			(1 - dda->step.x) / 2) / dda->ray_dir.x;
	else
		wall->dist_from_ray = (dda->map.y - m->players[m->pid].pos.y +
			(1 - dda->step.y) / 2) / dda->ray_dir.y;
	m->z_buffer[x] = wall->dist_from_ray;
	wall->length = (int)(WIN_HEIGHT / wall->dist_from_ray);
	wall->start = (-wall->length + WIN_HEIGHT) / 2;
	if (wall->start < 0)
		wall->start = 0;
	wall->end = (wall->length + WIN_HEIGHT) / 2;
	if (wall->end >= WIN_HEIGHT)
		wall->end = WIN_HEIGHT - 1;
	build_wall_2(dda, wall, m);
	draw_wall(dda, wall, m, x);
}
