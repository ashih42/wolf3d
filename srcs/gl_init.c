/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/05 23:27:25 by ashih             #+#    #+#             */
/*   Updated: 2018/08/18 05:16:39 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static int		gl_init_objs(t_master *m)
{
	const float			vertices[] = {
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f };
	const unsigned int	indices[] = {
		0, 1, 3,
		1, 2, 3 };

	glGenVertexArrays(1, &m->vao);
	glBindVertexArray(m->vao);
	glGenBuffers(1, &m->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &m->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
		(void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	return (0);
}

static int		init_texture(t_master *m)
{
	glGenTextures(1, &m->frame_tex);
	glBindTexture(GL_TEXTURE_2D, m->frame_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (!(m->frame = malloc(sizeof(int) * (WIN_WIDTH) * (WIN_HEIGHT))))
		return (ft_puterror(ERROR_MEMORY, 0, 1));
	return (0);
}

static int		gl_kickstart(t_master *m)
{
	glfwSetErrorCallback(error_callback);
	if (glfwInit() == GLFW_FALSE)
		return (ft_puterror(ERROR_GL, 0, 1));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	m->window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_NAME, NULL, NULL);
	if (!m->window)
		return (ft_puterror(ERROR_GL, 0, 1));
	glfwMakeContextCurrent(m->window);
	glfwSetWindowUserPointer(m->window, m);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return (ft_puterror(ERROR_GL, 0, 1));
	return (0);
}

int				gl_init(t_master *m)
{
	double		xpos;
	double		ypos;
	int			width;
	int			height;

	if (gl_kickstart(m) || gl_init_objs(m) || gl_init_shader(m) ||
		init_texture(m))
		return (1);
	glfwGetFramebufferSize(m->window, &width, &height);
	glViewport(0, 0, width, height);
	glfwSetWindowAspectRatio(m->window, width, height);
	glfwGetCursorPos(m->window, &xpos, &ypos);
	mouse_move_callback(NULL, xpos, ypos);
	glfwSetFramebufferSizeCallback(m->window, window_resize_callback);
	glfwSetKeyCallback(m->window, key_callback);
	glfwSetCursorPosCallback(m->window, mouse_move_callback);
	glfwSetInputMode(m->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	return (0);
}
