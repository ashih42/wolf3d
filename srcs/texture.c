/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/30 10:33:43 by ashih             #+#    #+#             */
/*   Updated: 2018/08/03 11:55:52 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static int		hsv_to_rgb(unsigned char h, unsigned char s, unsigned char v)
{
	t_rgb		rgb;

	ft_bzero(&rgb, sizeof(t_rgb));
	if (s == 0)
		return ((v << 16) | (v << 8) | v);
	rgb.region = h / 43;
	rgb.remainder = (h - (rgb.region * 43)) * 6;
	rgb.p = (v * (255 - s)) >> 8;
	rgb.q = (v * (255 - ((s * rgb.remainder) >> 8))) >> 8;
	rgb.t = (v * (255 - ((s * (255 - rgb.remainder)) >> 8))) >> 8;
	if (rgb.region == 0)
		return ((v << 16) | (rgb.t << 8) | rgb.p);
	else if (rgb.region == 1)
		return ((rgb.q << 16) | (v << 8) | rgb.p);
	else if (rgb.region == 2)
		return ((rgb.p << 16) | (v << 8) | rgb.t);
	else if (rgb.region == 3)
		return ((rgb.p << 16) | (rgb.q << 8) | v);
	else if (rgb.region == 4)
		return ((rgb.t << 16) | (rgb.p << 8) | v);
	else
		return ((v << 16) | (rgb.p << 8) | rgb.q);
}

static void		assign_floor_colors(t_sprite **sprite)
{
	int			x;
	int			y;
	int			c;

	y = -1;
	while (++y < TEX_W)
	{
		x = -1;
		while (++x < TEX_W)
		{
			c = x ^ y;
			sprite[0]->color[y * TEX_W + x].rgb = (c << 16) | (c << 8) | c;
			sprite[1]->color[y * TEX_W + x].rgb = ((255 - c) << 16) |
				((c % 128) << 8) | (c);
			sprite[2]->color[y * TEX_W + x].rgb = hsv_to_rgb(c, 255, 255);
		}
	}
}

int				gen_floor_tex(t_sprite **sprite)
{
	int			i;

	i = -1;
	while (++i < 3)
	{
		sprite[i] = ft_memalloc(sizeof(t_sprite) + sizeof(t_color) * SQ(TEX_W));
		if (sprite[i] == NULL)
			return (ft_puterror(ERROR_MEMORY, 0, 1));
		sprite[i]->width = TEX_W;
		sprite[i]->height = TEX_W;
	}
	assign_floor_colors(sprite);
	return (0);
}
