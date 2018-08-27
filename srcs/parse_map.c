/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 18:45:48 by ashih             #+#    #+#             */
/*   Updated: 2018/08/25 18:04:40 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"
#include "netplay.h"

static int		parse_dimen(int fd, t_map *map)
{
	int			error;
	char		*line;
	char		**arr;

	error = 0;
	line = NULL;
	arr = NULL;
	get_next_line(fd, &line);
	if (!(line && (arr = ft_strsplit(line, ' ')) &&
		ft_char_array_length(arr) == 2 &&
		!ft_atoi_check(arr[0], &map->width) &&
		!ft_atoi_check(arr[1], &map->height)))
		error = 1;
	ft_strdel(&line);
	ft_char_array_del(arr);
	if (error)
		ft_printf("  parse_dimen() failed\n");
	return (error ? ft_puterror(ERROR_MAP_DIMEN, 0, 1) : 0);
}

static int		parse_lines(int fd, t_map *map)
{
	int			i;

	map->cell = ft_memalloc(sizeof(char *) * (map->height + 1));
	i = 0;
	while (i < map->height && get_next_line(fd, &map->cell[i]) >= 0)
		i++;
	map->cell[i] = NULL;
	return (i != map->height ? ft_puterror(ERROR_MAP_HEIGHT, 0, 1) : 0);
}

/*
** Checks:
** (1) Each row contains exactly map->width symbols
** (2) Each symbol is a defined symbol according to assets
** (3) All map boundary cells must be walls, not empty
*/

static int		validate_lines(t_map *map, t_master *m)
{
	int			i;
	int			j;

	i = -1;
	while (++i < map->height)
	{
		if ((int)ft_strlen(map->cell[i]) != map->width)
			return (ft_puterror(ERROR_MAP_WIDTH, 0, 1));
		j = -1;
		while (++j < map->width)
		{
			if (!(map->cell[i][j] == EMPTY_CELL ||
				find_asset(map->cell[i][j], m)))
				return (ft_puterror(ERROR_MAP_SYMBOL, 0, 1));
			if ((i == 0 || i == map->height - 1 || j == 0 ||
				j == map->width - 1) && map->cell[i][j] == EMPTY_CELL)
				return (ft_puterror(ERROR_MAP_BORDER, 0, 1));
		}
	}
	return (0);
}

int				read_map_file(char *filename, t_master *m)
{
	int			error;
	int			fd;

	error = 0;
	if ((fd = open(filename, O_RDONLY)) < 0)
		return (ft_puterror(ERROR_OPEN_FILE, filename, 1));
	error = parse_dimen(fd, &m->map) ||
		parse_lines(fd, &m->map) || validate_lines(&m->map, m) ||
		parse_player(fd, m) || validate_player(m);
	if (close(fd) < 0)
		return (ft_puterror(ERROR_CLOSE_FILE, filename, 1));
	if (!error)
		m->map_name = filename;
	return (error);
}
