/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuel <apuel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/03 16:54:46 by ashih             #+#    #+#             */
/*   Updated: 2018/08/28 14:14:47 by apuel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"
#include "netplay.h"

static void		update_time(t_master *m)
{
	double		diff;
	char		*str;
	char		title[256];

	m->prev_time = m->curr_time;
	m->curr_time = glfwGetTime();
	diff = m->curr_time - m->prev_time;
	m->mov_inc = ((MOV_INC) * 60.0f) * (float)diff;
	if (diff <= 0.0)
		str = ft_strdup("Infinity");
	else
		str = ft_itoa((int)(1.0 / diff));
	ft_strcpy(title, WIN_NAME " - FPS: ");
	if (str)
	{
		ft_strcat(title, str);
		glfwSetWindowTitle(m->window, title);
		free(str);
	}
}

static void		main_loop(t_master *m)
{
	while (!(m->terminate) && !glfwWindowShouldClose(m->window))
	{
		if (!(m->client) && m->socket >= 0)
			listener_task(m);
		update_time(m);
		pthread_mutex_lock(&(m->mutex));
		if (!(m->client))
			update_entities(m);
		update_player_position(m);
		update_player_rotation(m);
		render(m);
		pthread_mutex_unlock(&(m->mutex));
		glClear(GL_COLOR_BUFFER_BIT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIN_WIDTH, WIN_HEIGHT, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, m->frame);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(m->window);
		glfwPollEvents();
	}
}

static int		data_init(char *path, t_master *m)
{
	return (init_argv0_path(path, &(m->path)) ||
		load_sprite(&(m->textures[0]), SONIC_SPRITE, m->path) ||
		load_sprite(&(m->textures[1]), UGANDA_SPRITE, m->path) ||
		load_sprite(&(m->textures[TEXTURE_COUNT + 0]), P1_SPRITE, m->path) ||
		load_sprite(&(m->textures[TEXTURE_COUNT + 1]), P2_SPRITE, m->path) ||
		load_sprite(&(m->textures[BULLET_TEXTURE]), BULLET_SPRITE, m->path) ||
		init_assets(m));
}

static int		process_flags(int argc, char **argv, t_master *m)
{
	int	i;

	i = 0;
	while (++i < argc)
	{
		if (argv[i][0] != '-' || !(argv[i][1]) || argv[i][2])
			m->map_name = argv[i];
		else if (!(m->mode) && i + 1 < argc && argv[i][1] == 's')
		{
			m->mode = 's';
			m->port = ft_atoi(argv[++i]);
		}
		else if (!(m->mode) && i + 2 < argc && argv[i][1] == 'c')
		{
			m->mode = 'c';
			m->client = 1;
			m->ip = argv[++i];
			m->port = ft_atoi(argv[++i]);
		}
		else
			return (ft_puterror(ERROR_USAGE, 0, 1));
	}
	if (!(m->client) && !(m->map_name))
		return (ft_puterror(ERROR_USAGE, 0, 1));
	return (0);
}

int				main(int argc, char **argv)
{
	t_master	m;

	srand(time(NULL));
	ft_bzero(&m, sizeof(t_master));
	m.socket = -1;
	m.players[0].connected = 1;
	if (data_init(argv[0], &m) || process_flags(argc, argv, &m))
		return (free_all(&m, EXIT_FAILURE));
	if (server_init(&m))
		return (free_all(&m, EXIT_FAILURE));
	if (gl_init(&m))
		return (free_all(&m, EXIT_FAILURE));
	m.curr_time = glfwGetTime();
	init_bgm(m.path);
	draw_thread_init(&m);
	main_loop(&m);
	draw_thread_deinit(&m);
	server_deinit(&m);
	return (free_all(&m, EXIT_SUCCESS));
}
