/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_init_shader.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuel <apuel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/07 00:59:51 by ashih             #+#    #+#             */
/*   Updated: 2018/08/25 15:29:50 by apuel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static int			join_lines(int fd, char **str)
{
	char			*temp;
	char			buffer[4096 + 1];
	int				n_bytes;

	while ((n_bytes = read(fd, buffer, 4096)))
	{
		if (n_bytes < 0)
			return (-1);
		buffer[n_bytes] = '\0';
		temp = *str;
		*str = ft_strjoin(temp, buffer);
		ft_strdel(&temp);
		if (!*str)
			return (1);
	}
	return (0);
}

static char			*load_shader(char *filename, char *path)
{
	int				error;
	int				fd;
	char			*str;

	error = 0;
	if ((fd = ft_open(path, filename, O_RDONLY)) < 0)
	{
		ft_puterror(ERROR_OPEN_FILE, filename, 0);
		return (NULL);
	}
	if (!(str = ft_strnew(0)))
	{
		ft_puterror(ERROR_MEMORY, filename, 0);
		return (NULL);
	}
	join_lines(fd, &str);
	close(fd);
	return (str);
}

static int			compile_shader(unsigned int *id, GLenum type,
	char *file, char *path)
{
	char			*shader_str;
	int				success;
	char			info_log[512];

	*id = glCreateShader(type);
	if (!(shader_str = load_shader(file, path)))
		return (1);
	glShaderSource(*id, 1, (const char *const *)&shader_str, NULL);
	glCompileShader(*id);
	glGetShaderiv(*id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(*id, 512, NULL, info_log);
		return (ft_puterror(ERROR_SHADER, info_log, 1));
	}
	return (0);
}

static int			link_shader(unsigned int vs, unsigned int fs, t_master *m)
{
	int				success;
	char			info_log[512];

	m->shader_prog = glCreateProgram();
	glAttachShader(m->shader_prog, vs);
	glAttachShader(m->shader_prog, fs);
	glLinkProgram(m->shader_prog);
	glGetProgramiv(m->shader_prog, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m->shader_prog, 512, NULL, info_log);
		return (ft_puterror(ERROR_SHADER, info_log, 1));
	}
	glUseProgram(m->shader_prog);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return (0);
}

int					gl_init_shader(t_master *m)
{
	unsigned int	vs;
	unsigned int	fs;

	return (compile_shader(&vs, GL_VERTEX_SHADER,
			"shaders/vertex_shader.vs", m->path) ||
		compile_shader(&fs, GL_FRAGMENT_SHADER,
			"shaders/fragment_shader.fs", m->path) ||
		link_shader(vs, fs, m));
}
