/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freedom.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/03 10:16:05 by ashih             #+#    #+#             */
/*   Updated: 2018/08/08 19:16:34 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void			free_gl_stuff(t_master *m)
{
	ft_memdel((void **)&m->frame);
	if (m->window)
		glfwDestroyWindow(m->window);
	if (m->vao)
		glDeleteVertexArrays(1, &m->vao);
	if (m->vbo)
		glDeleteBuffers(1, &m->vbo);
	if (m->ebo)
		glDeleteBuffers(1, &m->ebo);
	glfwTerminate();
}

static void		free_assets(t_master *m)
{
	int			i;

	i = -1;
	while (++i < m->asset_count)
		ft_memdel((void **)&m->assets[i].tex);
	ft_memdel((void **)&m->assets);
	i = -1;
	while (++i < FLOOR_TYPES)
		ft_memdel((void **)&m->floor_tex[i]);
	ft_memdel((void **)&m->ceil_tex);
}

static void		free_map(t_master *m)
{
	ft_char_array_del(m->map.cell);
}

int				free_all(t_master *m, int ret)
{
	free_gl_stuff(m);
	free_assets(m);
	free_map(m);
	ft_strdel(&m->path);
	system("pkill -f afplay");
	return (ret);
}
