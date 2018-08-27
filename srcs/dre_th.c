/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dre_th.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/05 16:07:02 by apuel             #+#    #+#             */
/*   Updated: 2018/08/25 18:22:01 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			init_dre(t_dre *dre, t_entity *e, t_player *p, t_master *m)
{
	ft_bzero(dre, sizeof(t_dre));
	dre->tex = m->textures[e->tex];
	dre->spr.x = e->pos.x - p->pos.x;
	dre->spr.y = e->pos.y - p->pos.y;
	dre->inv_det = 1.0 / (p->plane.x * p->dir.y - p->dir.x * p->plane.y);
	dre->trans.x = dre->inv_det *
		(p->dir.y * dre->spr.x - p->dir.x * dre->spr.y);
	dre->trans.y = dre->inv_det *
		(-p->plane.y * dre->spr.x + p->plane.x * dre->spr.y);
	dre->screen_x = (int)((WIN_WIDTH / 2) * (1 + dre->trans.x / dre->trans.y));
	dre->spr_height = abs((int)(WIN_HEIGHT / (dre->trans.y)));
	dre->start.y = -dre->spr_height / 2 + WIN_HEIGHT / 2;
	CLAMP(dre->start.y, 0, WIN_HEIGHT - 1);
	dre->end.y = dre->spr_height / 2 + WIN_HEIGHT / 2;
	CLAMP(dre->end.y, 0, WIN_HEIGHT - 1);
	dre->spr_width = abs((int)(WIN_HEIGHT / (dre->trans.y)));
	dre->start.x = -dre->spr_width / 2 + dre->screen_x;
	CLAMP(dre->start.x, 0, WIN_WIDTH - 1);
	dre->end.x = dre->spr_width / 2 + dre->screen_x;
	CLAMP(dre->end.x, 0, WIN_WIDTH - 1);
}

static void		dre_vert(int x, t_dre *dre, t_master *m)
{
	int			y;
	int			d;
	int			tex_x;
	int			tex_y;

	if (dre->spr_width == 0 || dre->spr_height == 0)
		return ;
	tex_x = (int)(256 * (x - (-dre->spr_width / 2 + dre->screen_x)) *
		dre->tex->width / dre->spr_width) / 256;
	if (dre->trans.y > 0 && x > 0 && x < WIN_WIDTH &&
		dre->trans.y < m->z_buffer[x])
	{
		y = dre->start.y - 1;
		while (++y < dre->end.y)
		{
			d = y * 256 - WIN_HEIGHT * 128 + dre->spr_height * 128;
			tex_y = ((d * dre->tex->height) / dre->spr_height) / 256;
			enforce_range(&tex_x, dre->tex->width);
			enforce_range(&tex_y, dre->tex->height);
			if (dre->tex->color[dre->tex->height * tex_x + tex_y].alpha >
				ALPHA_CUTOFF)
				draw_dot(x, y,
					dre->tex->color[dre->tex->height * tex_x + tex_y].rgb, m);
		}
	}
}

void			dre_th(t_master *m, int i)
{
	int			x;
	int			limit;
	t_dre		*dre;

	dre = &m->dre;
	x = dre->start.x + (dre->end.x - dre->start.x) * i / THREAD_COUNT;
	limit = dre->start.x + (dre->end.x - dre->start.x) * (i + 1) / THREAD_COUNT;
	while (x < limit)
		dre_vert(x++, dre, m);
}

void			dre_th_master(t_master *m)
{
	int			i;

	i = -1;
	while (++i < THREAD_COUNT)
	{
		m->draw_state[i] = DRAW_TH_DRE;
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
