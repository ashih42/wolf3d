/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/03 10:19:57 by ashih             #+#    #+#             */
/*   Updated: 2018/08/08 19:18:35 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void	mouse_move_callback(GLFWwindow *window, double xpos, double ypos)
{
	static double	x_ = 0;
	double			dx;
	t_master		*m;

	(void)ypos;
	if (window == NULL)
	{
		x_ = xpos;
		return ;
	}
	m = glfwGetWindowUserPointer(window);
	dx = x_ - xpos;
	pthread_mutex_lock(&(m->mutex));
	if (dx > 0.0)
		rotate_left(m, dx * (ROT_INC / 4.0f));
	else if (dx < 0.0)
		rotate_right(m, -dx * (ROT_INC / 4.0f));
	pthread_mutex_unlock(&(m->mutex));
	x_ = xpos;
}
