/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ppm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashih <ashih@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/03 10:22:22 by ashih             #+#    #+#             */
/*   Updated: 2018/08/08 19:24:09 by ashih            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static int			create_ppm_name(char *filename, t_master *m)
{
	char			*str;

	ft_bzero(filename, 20);
	ft_strcpy(filename, "out_");
	if (!(str = ft_itoa(++(m->files_saved))))
		return (ft_puterror(ERROR_MEMORY, 0, 1));
	ft_strcat(filename, str);
	ft_strdel(&str);
	ft_strcat(filename, ".ppm");
	return (0);
}

static int			write_ppm_header(int fd)
{
	char			*str;

	write(fd, "P6\n", 3);
	if (!(str = ft_itoa(WIN_WIDTH)))
		return (ft_puterror(ERROR_MEMORY, 0, 1));
	write(fd, str, ft_strlen(str));
	ft_strdel(&str);
	write(fd, " ", 1);
	if (!(str = ft_itoa(WIN_HEIGHT)))
		return (ft_puterror(ERROR_MEMORY, 0, 1));
	write(fd, str, ft_strlen(str));
	ft_strdel(&str);
	write(fd, "\n255\n", 5);
	return (0);
}

static int			write_ppm_content(int fd, t_master *m)
{
	unsigned char	*buffer;
	int				i;

	if (!(buffer = ft_memalloc(sizeof(unsigned char) *
		WIN_WIDTH * WIN_HEIGHT * 3)))
		return (ft_puterror(ERROR_MEMORY, 0, 1));
	i = -1;
	while (++i < WIN_WIDTH * WIN_HEIGHT)
	{
		buffer[3 * i + 0] = (m->frame[i] >> 16) & 0xFF;
		buffer[3 * i + 1] = (m->frame[i] >> 8) & 0xFF;
		buffer[3 * i + 2] = (m->frame[i] >> 0) & 0xFF;
	}
	write(fd, buffer, WIN_WIDTH * WIN_HEIGHT * 3);
	ft_memdel((void **)&buffer);
	return (0);
}

int					save_ppm(t_master *m)
{
	int				fd;
	char			filename[20];

	if (create_ppm_name(filename, m))
		return (1);
	if ((fd = ft_open(m->path, filename, O_WRONLY | O_CREAT)) == -1)
		return (ft_puterror(ERROR_OPEN_FILE, filename, 1));
	fchmod(fd, 0644);
	if (write_ppm_header(fd) || write_ppm_content(fd, m))
		return (close(fd));
	if (close(fd) == -1)
		return (ft_puterror(ERROR_CLOSE_FILE, filename, 1));
	ft_printf("Saved to %s\n", filename);
	return (0);
}
