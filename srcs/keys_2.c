/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 18:35:18 by ashih             #+#    #+#             */
/*   Updated: 2018/08/25 18:42:10 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"
#include "netplay.h"

void		rotate_left(t_master *m, float rot)
{
	m->players[m->pid].angle -= rot;
	if (m->players[m->pid].angle < 0.0f)
		m->players[m->pid].angle += 360.0f;
}

void		rotate_right(t_master *m, float rot)
{
	m->players[m->pid].angle += rot;
	if (m->players[m->pid].angle >= 360.0f)
		m->players[m->pid].angle -= 360.0f;
}

void		rotate_left_hotkey(t_master *m, int action)
{
	if (action == GLFW_PRESS)
		m->rotation++;
	else if (action == GLFW_RELEASE)
		m->rotation--;
}

void		rotate_right_hotkey(t_master *m, int action)
{
	if (action == GLFW_PRESS)
		m->rotation--;
	else if (action == GLFW_RELEASE)
		m->rotation++;
}
