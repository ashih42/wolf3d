/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/01 22:46:02 by ashih             #+#    #+#             */
/*   Updated: 2018/08/06 15:00:52 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
** No boundary checking
** (x, y) MUST already be within valid range
*/

void		draw_dot(int x, int y, int color, t_master *m)
{
	m->frame[x + y * WIN_WIDTH] = color;
}
