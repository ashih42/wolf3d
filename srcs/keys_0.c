/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_0.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 18:35:18 by ashih             #+#    #+#             */
/*   Updated: 2018/08/25 18:41:33 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"
#include "netplay.h"

void		terminate(t_master *m, int action)
{
	if (action == GLFW_PRESS)
		glfwSetWindowShouldClose(m->window, GLFW_TRUE);
}

void		toggle_mouse_cursor(t_master *m, int action)
{
	if (action == GLFW_PRESS)
	{
		m->cursor_enabled = !m->cursor_enabled;
		if (m->cursor_enabled)
			glfwSetInputMode(m->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(m->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void		save_ppm_hotkey(t_master *m, int action)
{
	if (action != GLFW_PRESS)
		return ;
	save_ppm(m);
}

void		fire_bullet(t_master *m, int action)
{
	t_vect2		pos;
	t_entity	*bullet;

	if (action != GLFW_PRESS)
		return ;
	pos.x = m->players[m->pid].pos.x + m->players[m->pid].dir.x * MOV_INC;
	pos.y = m->players[m->pid].pos.y + m->players[m->pid].dir.y * MOV_INC;
	if (m->client)
		send_bullet_packet(m->socket, &pos, &(m->players[m->pid].dir));
	else
	{
		bullet = spawn_entity(BULLET_TEXTURE, &pos, m);
		if (bullet)
		{
			bullet->is_moving = 1;
			bullet->dir = m->players[m->pid].dir;
			play_sound("sounds/laser_1b.wav", m->path);
		}
	}
}
