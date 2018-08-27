/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 18:35:18 by ashih             #+#    #+#             */
/*   Updated: 2018/08/25 18:44:16 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"
#include "netplay.h"

/*
** 0 := no minimap
** 1 := minimap, fixed
** 2 := minimap, rotating
*/

void		toggle_minimap(t_master *m, int action)
{
	if (action != GLFW_PRESS)
		return ;
	m->minimap = (m->minimap + 1) % 3;
}

void		switch_floor_mode(t_master *m, int action)
{
	if (action != GLFW_PRESS)
		return ;
	m->floor_mode = (m->floor_mode + 1) % FLOOR_TYPES;
}

void		toggle_curve_mode(t_master *m, int action)
{
	if (action != GLFW_PRESS)
		return ;
	m->curve_mode = !m->curve_mode;
}

void		toggle_wobble_mode(t_master *m, int action)
{
	if (action != GLFW_PRESS)
		return ;
	m->wobble_mode = (m->wobble_mode + 1) % 3;
	m->players[m->pid].default_plane.x = 0.0f;
	m->players[m->pid].default_plane.y = -0.66f;
}
