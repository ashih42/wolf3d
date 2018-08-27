/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/03 09:40:16 by ashih             #+#    #+#             */
/*   Updated: 2018/08/25 17:57:53 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

t_asset			*find_asset(char c, t_master *m)
{
	int			i;

	i = -1;
	while (++i < m->asset_count)
	{
		if (m->assets[i].symbol == c)
			return (&m->assets[i]);
	}
	return (NULL);
}

static int		parse_asset_line(int fd, t_asset *asset, char *path)
{
	int			error;
	char		*line;
	char		**arr;

	error = 0;
	line = NULL;
	arr = NULL;
	get_next_line(fd, &line);
	if (!(line && (arr = ft_strsplit(line, ' ')) &&
		ft_char_array_length(arr) == 3 &&
		ft_strlen(arr[0]) == 1 &&
		!ft_hex_to_int_check(arr[1], &asset->mm_color) &&
		!load_sprite(&asset->tex, arr[2], path)))
		error = ft_puterror(ERROR_ASSET, line, 1);
	else
		asset->symbol = arr[0][0];
	ft_strdel(&line);
	ft_char_array_del(arr);
	return (error);
}

static int		parse_assets(int fd, t_master *m)
{
	int			i;

	if (!(m->assets = ft_memalloc(sizeof(t_asset) * m->asset_count)))
		return (ft_puterror(ERROR_MEMORY, 0, 1));
	i = -1;
	while (++i < m->asset_count)
	{
		if (parse_asset_line(fd, &m->assets[i], m->path))
			return (1);
	}
	return (0);
}

static int		parse_asset_count(int fd, t_master *m)
{
	int			error;
	char		*line;

	error = 0;
	line = NULL;
	get_next_line(fd, &line);
	if (!(line && !ft_atoi_check(line, &m->asset_count) &&
		m->asset_count > 0))
		error = 1;
	ft_strdel(&line);
	return (error ? ft_puterror(ERROR_ASSET_COUNT, 0, 1) : 0);
}

int				init_assets(t_master *m)
{
	int			error;
	int			fd;

	error = 0;
	if ((fd = ft_open(m->path, ASSETS_FILE, O_RDONLY)) < 0)
		return (ft_puterror(ERROR_OPEN_FILE, ASSETS_FILE, 1));
	error = parse_asset_count(fd, m) || parse_assets(fd, m) ||
		load_sprite(&m->ceil_tex, SPRITE_CEIL, m->path) ||
		load_sprite(&m->floor_tex[3], SPRITE_FLOOR, m->path) ||
		gen_floor_tex(m->floor_tex);
	get_next_line(fd, NULL);
	if (close(fd) < 0)
		return (ft_puterror(ERROR_CLOSE_FILE, ASSETS_FILE, 1));
	return (error);
}
