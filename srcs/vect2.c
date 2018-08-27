/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vect2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/03 10:49:47 by ashih             #+#    #+#             */
/*   Updated: 2018/08/05 16:53:49 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			v_normalize(t_vect2 *v)
{
	float		magnitude;

	if (v->x == 0.0f && v->y == 0.0f)
		return ;
	magnitude = v->x * v->x + v->y * v->y;
	v->x /= magnitude;
	v->y /= magnitude;
}

void			v_rotate(t_vect2 *source, t_vect2 *dest, float angle)
{
	t_vect2		dir_;

	angle = DEG_TO_RAD(angle);
	dir_.x = source->x * cos(angle) - source->y * sin(angle);
	dir_.y = source->x * sin(angle) + source->y * cos(angle);
	dest->x = dir_.x;
	dest->y = dir_.y;
}

void			rand_dir(t_vect2 *dir)
{
	t_vect2		dir_;
	float		angle;

	dir_.x = 1.0;
	dir_.y = 0.0;
	angle = rand() % 360;
	v_rotate(&dir_, dir, angle);
}
