/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_callback.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuel <apuel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/05 23:24:47 by ashih             #+#    #+#             */
/*   Updated: 2018/08/28 15:08:52 by apuel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"
#include "netplay.h"

/*
** Key						Function
**
** ESC						Terminate program
** TAB						Toggle mouse cursor on/off
** TILDE (~)				Save image to PPM file
**
** SPACEBAR					Shoot burger!
**
** W						Move forward
** S 						Move backward
** A						Move leftward
** D						Move rightward
**
** Q						Rotate left
** E						Rotate right
**
** M						Toggle minimap on/off
** F						Switch floor mode
** C						Toggle "Curve" Mode" on/off
** V						Toggle "Wobble Mode" on/off
*/

void		key_callback(GLFWwindow *window, KEY_CALLBACK_ARGS mods)
{
	static void	(*key_table[259])(t_master *, int) = {
		[GLFW_KEY_ESCAPE] = terminate,
		[GLFW_KEY_TAB] = toggle_mouse_cursor,
		[GLFW_KEY_GRAVE_ACCENT] = save_ppm_hotkey,
		[GLFW_KEY_W] = move_forward,
		[GLFW_KEY_S] = move_backward,
		[GLFW_KEY_A] = move_leftward,
		[GLFW_KEY_D] = move_rightward,
		[GLFW_KEY_Q] = rotate_left_hotkey,
		[GLFW_KEY_E] = rotate_right_hotkey,
		[GLFW_KEY_M] = toggle_minimap,
		[GLFW_KEY_F] = switch_floor_mode,
		[GLFW_KEY_C] = toggle_curve_mode,
		[GLFW_KEY_V] = toggle_wobble_mode,
		[GLFW_KEY_SPACE] = fire_bullet };
	t_master	*m;

	m = glfwGetWindowUserPointer(window);
	if (key <= 258 && key_table[key])
	{
		pthread_mutex_lock(&(m->mutex));
		key_table[key](m, action);
		pthread_mutex_unlock(&(m->mutex));
	}
	scancode = mods;
}

void		window_resize_callback(GLFWwindow *window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}

void		error_callback(int error, const char *description)
{
	ft_printf("error_callback(): Error %d: %s\n", error, description);
}
