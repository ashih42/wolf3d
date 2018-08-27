/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/24 21:43:11 by ashih             #+#    #+#             */
/*   Updated: 2018/08/08 19:00:25 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int			load_sprite(t_sprite **sprite, char *file, char *path)
{
	int		fd;
	int		width;
	int		height;

	if ((fd = ft_open(path, file, O_RDONLY)) == -1)
		return (ft_puterror(ERROR_SPRITE, file, 1));
	read(fd, &height, 4);
	read(fd, &width, 4);
	*sprite = ft_memalloc(sizeof(t_sprite) + sizeof(t_color) * width * height);
	if (*sprite == 0)
		return (ft_puterror(ERROR_MEMORY, 0, 1));
	(*sprite)->width = width;
	(*sprite)->height = height;
	read(fd, (*sprite)->color, sizeof(t_color) * width * height);
	if (close(fd) == -1)
		return (ft_puterror(ERROR_CLOSE_FILE, 0, 1));
	return (0);
}
