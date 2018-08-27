/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/03 10:18:47 by ashih             #+#    #+#             */
/*   Updated: 2018/08/08 19:22:54 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
** Invariant: Parser will enforce that a valid map must be fully enclosed,
** such that all boundary cells must be walls (not empty cells)
*/

/*
** o-------------> x, width
** |
** |
** V
** y, height
*/

int			is_valid_pos(int x, int y, t_map *map)
{
	return (0 <= x && x < map->width &&
		0 <= y && y < map->height);
}

/*
** Check if cell (x, y) is a valid position for player or entity:
**
** (x, y) must be a valid position
** (x, y) must be empty (NOT a wall)
*/

int			is_valid_move(int x, int y, t_map *map)
{
	return (0 <= x && x < map->width &&
		0 <= y && y < map->height &&
		map->cell[y][x] == EMPTY_CELL);
}
