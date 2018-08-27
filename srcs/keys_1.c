/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 18:35:18 by ashih             #+#    #+#             */
/*   Updated: 2018/08/25 19:24:04 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"
#include "netplay.h"

void		move_forward(t_master *m, int action)
{
	if (action == GLFW_PRESS)
		m->forward += 1.0f;
	else if (action == GLFW_RELEASE)
		m->forward -= 1.0f;
}

void		move_backward(t_master *m, int action)
{
	if (action == GLFW_PRESS)
		m->forward -= 1.0f;
	else if (action == GLFW_RELEASE)
		m->forward += 1.0f;
}

void		move_leftward(t_master *m, int action)
{
	if (action == GLFW_PRESS)
		m->rightward -= 1.0f;
	else if (action == GLFW_RELEASE)
		m->rightward += 1.0f;
}

void		move_rightward(t_master *m, int action)
{
	if (action == GLFW_PRESS)
		m->rightward += 1.0f;
	else if (action == GLFW_RELEASE)
		m->rightward -= 1.0f;
}
