/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_mm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 18:45:48 by ashih             #+#    #+#             */
/*   Updated: 2018/08/08 19:14:54 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int				get_mm_color(int x, int y, t_master *m)
{
	char		c;

	c = m->map.cell[y][x];
	return (find_asset(c, m)->mm_color);
}

/*
** No boundary checking
** (x, y) MUST already be within valid range
*/

void			draw_dot_mm(int x, int y, int color, t_master *m)
{
	x += WIN_WIDTH - MM_WIDTH;
	m->frame[x + y * WIN_WIDTH] = color;
}

void			draw_mm(int x, int y, t_master *m)
{
	t_vect2		dir;
	t_vect2i	pos;

	dir.x = CHANGE((float)x, 0.0f, (float)(MM_WIDTH - 1),
		-MM_HALF_DIST, MM_HALF_DIST);
	dir.y = CHANGE((float)y, 0.0f, (float)(MM_WIDTH - 1),
		-MM_HALF_DIST, MM_HALF_DIST);
	if (m->minimap == 2)
		v_rotate(&dir, &dir, m->players[m->pid].angle - 90.0f);
	pos.x = floor(dir.x + m->players[m->pid].pos.x);
	pos.y = floor(dir.y + m->players[m->pid].pos.y);
	if (0 <= pos.x && pos.x < m->map.width &&
		0 <= pos.y && pos.y < m->map.height &&
		m->map.cell[pos.y][pos.x] != EMPTY_CELL)
		draw_dot_mm(x, y, get_mm_color(pos.x, pos.y, m), m);
	else
		draw_dot_mm(x, y, 0, m);
}

void			draw_mm_border(t_master *m)
{
	int			i;

	i = 0;
	while (i < MM_WIDTH)
	{
		draw_dot_mm(i, 0, 0xFFFFFF, m);
		draw_dot_mm(i, MM_WIDTH - 1, 0xFFFFFF, m);
		draw_dot_mm(0, i, 0xFFFFFF, m);
		draw_dot_mm(MM_WIDTH - 1, i, 0xFFFFFF, m);
		i++;
	}
}

void			draw_mm_player(t_master *m)
{
	int			start_1;
	int			start_2;
	int			i;

	start_1 = (MM_WIDTH - MM_PLAYER_SIZE) / 2;
	start_2 = (MM_WIDTH + MM_PLAYER_SIZE) / 2;
	i = 0;
	while (i <= MM_PLAYER_SIZE)
	{
		draw_dot_mm(start_1, start_1 + i, 0xFFFFFF, m);
		draw_dot_mm(start_2, start_1 + i, 0xFFFFFF, m);
		draw_dot_mm(start_1 + i, start_1, 0xFFFFFF, m);
		draw_dot_mm(start_1 + i, start_2, 0xFFFFFF, m);
		i++;
	}
}
