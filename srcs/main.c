/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/03 16:54:46 by ashih             #+#    #+#             */
/*   Updated: 2018/08/25 19:26:46 by ashih            ###   ########.fr       */
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

void			main_loop(t_master *m)
{
	while(!(m->terminate) && !glfwWindowShouldClose(m->window))
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

int				main(int argc, char **argv)
{
	t_master	m;

	if (argc < 2)
		return (ft_puterror(ERROR_USAGE, 0, EXIT_FAILURE));

	srand(time(NULL));
	ft_bzero(&m, sizeof(t_master));

	m.socket = -1;

	init_argv0_path(argv[0], &m.path);	// check for failure on this too

	load_sprite(&(m.textures[0]), SONIC_SPRITE, m.path);
	load_sprite(&(m.textures[1]), UGANDA_SPRITE, m.path);
	load_sprite(&(m.textures[TEXTURE_COUNT + 0]), P1_SPRITE, m.path);
	load_sprite(&(m.textures[TEXTURE_COUNT + 1]), P2_SPRITE, m.path);
	load_sprite(&(m.textures[BULLET_TEXTURE]), BULLET_SPRITE, m.path);

	if (init_assets(&m))
		return (free_all(&m, 1));

	int server = 0;
	if (argc > 2 && !ft_strcmp(argv[2], "-s"))
		server = 1;
	else if (argc > 2)
		m.client = 1;

	if (!m.client)
	{
		if (read_map_file(argv[1], &m) || init_entities(&m))
			return (free_all(&m, EXIT_FAILURE));
	}
	else
	{
		if (start_client(&m, argv[2], 4242))
		{
			ft_printf("[Client] %s\n", strerror(errno));
			return (free_all(&m, EXIT_FAILURE));
		}
	}

	if (server)
	{
		m.players[0].connected = 1;
		if (start_server(&m, 4242))
		{
			ft_printf("[Server] %s\n", strerror(errno));
			return (free_all(&m, EXIT_FAILURE));
		}
	}

	pthread_mutex_init(&(m.mutex), NULL);
	if (m.socket >= 0 && pthread_create(&(m.thread), NULL, packet_thread, &m))
		return (free_all(&m, EXIT_FAILURE));

	if (gl_init(&m))
		return (free_all(&m, EXIT_FAILURE));
	m.curr_time = glfwGetTime();

//	print_map(&m.map);
	init_bgm(m.path);
	draw_thread_init(&m);

	main_loop(&m);

	if (m.socket >= 0)
	{
		m.terminate = 1;
		pthread_join(m.thread, 0);
		if (m.client)
			send_disconnect_packet(m.socket);
		else
		{
			for (uint32_t i = 1; i < PLAYER_COUNT; i++)
				if (m.players[i].connected)
					disconnect_player(i, &m);
		}
		shutdown(m.socket, 2);
		ft_putstr("Disconnected.\n");
	}

	draw_thread_deinit(&m);
	return (free_all(&m, EXIT_SUCCESS));
}
