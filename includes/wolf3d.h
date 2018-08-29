/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apuel <apuel@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 18:39:39 by ashih             #+#    #+#             */
/*   Updated: 2018/08/28 15:17:51 by apuel            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include "libft.h"
# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <math.h>
# include <pthread.h>
# include <mach/thread_act.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# include <netinet/in.h>
# include <errno.h>

# define WIN_WIDTH				800
# define WIN_HEIGHT				600
# define WIN_NAME				"Wolf3D"

# define MM_WIDTH				200
# define MM_HALF_DIST			8
# define MM_PLAYER_SIZE			10
# define MM_WIDTH_F				(0.3f)
# define MM_BOTTOM_X			((1.0f - MM_WIDTH_F) * 2.0f - 1.0f)
# define MM_BOTTOM_Y			(-1.0f)
# define MM_TOP_X				(1.0f)

/*
**	One thread per core.
*/
# define THREAD_COUNT			4

# define ERROR_GL				"Some kind of OpenGL error"
# define ERROR_SHADER			"Shader error"

# define ERROR_USAGE			"usage: ./wolf3d <input_file>\n" FLAG_USAGE
# define FLAG_USAGE				FLAG_S_USAGE FLAG_C_USAGE FLAG_H_USAGE
# define FLAG_S_USAGE			"-s <port>        Spawn server.\n"
# define FLAG_C_USAGE			"-c <ip> <port>   Spawn client.\n"
# define FLAG_H_USAGE			"-h               Display usage."
# define ERROR_MEMORY			"Out of memory"
# define ERROR_OPEN_FILE		"Cannot open file"
# define ERROR_CLOSE_FILE		"Cannot close file"
# define ERROR_MAP_DIMEN		"Invalid map dimensions"
# define ERROR_MAP_TOO_SMALL	"Invalid map size: must be at least 3 x 3"
# define ERROR_MAP_HEIGHT		"Invalid map: does not match height"
# define ERROR_MAP_WIDTH		"Invalid map: does not match width"
# define ERROR_MAP_SYMBOL		"Invalid map: symbol undefined in asset"
# define ERROR_MAP_BORDER		"Invalid map: border not closed"
# define ERROR_PLAYER			"Invalid player position"
# define ERROR_PLAYER_IL		"Illegal player position"
# define ERROR_SPRITE			"Sprite file not found"
# define ERROR_ASSET_COUNT		"Invalid asset count"
# define ERROR_ASSET			"Invalid asset"

# define SWAP(a, b, T)			{T temp = a; a = b; b = temp;}
# define DEG_TO_RAD(a)			((a) * M_PI / 180.0f)
# define SQ(x)					((x) * (x))
# define ABS(x)					((x >= 0) ? (x) : (-(x)))
# define MAX(a, b)				((a) > (b) ? (a) : (b))
# define MIN(a, b)				((a) < (b) ? (a) : (b))
# define CLAMP(x, min, max)		(x = MIN(MAX(min, x), max))

/*
** Transform x in [min, max] to some new value in [a, b]
*/
# define CHANGE(x, min, max, a, b) (((b)-(a))*((x)-(min))/((max)-(min)))+(a)

# define MOV_LOOKAHEAD			0.5f
# define MOV_INC				0.05f
# define ROT_INC				2.5f
# define WOB_RATE				2

# define EMPTY_CELL				'0'

# define TEX_W					256
# define ALPHA_CUTOFF			10

# define ASSETS_FILE			"assets/assets.wolf"

# define SPRITE_CEIL			"textures/twinkles.sprite"
# define SPRITE_FLOOR			"textures/neptunia.sprite"

# define FLOOR_TYPES			4

# define SOCKADDR_IN			struct sockaddr_in
# define PLAYER_COUNT			256
# define PLAYER_SPRITE_COUNT	2
# define TEXTURE_COUNT			2
# define BULLET_TEXTURE			(TEXTURE_COUNT + PLAYER_SPRITE_COUNT)
# define ENTITY_COUNT			256

# define MINI_SPRITE			"textures/mini_knuckles.sprite"
# define UGANDA_SPRITE			"textures/ugandan_knuckles.sprite"
# define SONIC_SPRITE			"textures/sonic.sprite"
# define BULLET_SPRITE			"textures/burger.sprite"

# define P1_SPRITE				"textures/BlobFearSweat.sprite"
# define P2_SPRITE				"textures/blobwizard.sprite"

# define KEY_CALLBACK_ARGS int key, int scancode, int action, int

typedef	struct		s_vect2
{
	float			x;
	float			y;
}					t_vect2;

typedef	struct		s_vect2i
{
	int				x;
	int				y;
}					t_vect2i;

typedef struct		s_color
{
	int				rgb;
	int				alpha;
}					t_color;

typedef struct		s_sprite
{
	int				width;
	int				height;
	t_color			color[];
}					t_sprite;

typedef struct		s_rgb
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	region;
	unsigned char	remainder;
	unsigned char	p;
	unsigned char	q;
	unsigned char	t;
}					t_rgb;

typedef	struct		s_map
{
	int				width;
	int				height;
	char			**cell;
}					t_map;

typedef struct		s_dda
{
	float			camera_x;
	t_vect2			ray_dir;
	t_vect2i		map;
	t_vect2			side_dist;
	t_vect2			delta_dist;
	t_vect2i		step;
	int				hit;
	int				hit_side;
}					t_dda;

typedef struct		s_wall
{
	float			dist_from_ray;
	int				length;
	int				start;
	int				end;
	int				color;
	float			hit_x;
	int				tex_x;
	int				tex_y;
	t_sprite		*tex;
}					t_wall;

typedef struct		s_floor
{
	t_vect2			wall_pos;
	float			dist_wall;
	float			dist_player;
	float			curr_dist;
	float			weight;
	t_vect2			curr;
	int				tex_x;
	int				tex_y;
	t_sprite		*tex;
	int				x;
	int				y;
}					t_floor;

typedef struct		s_dre
{
	t_vect2			spr;
	float			inv_det;
	t_vect2			trans;
	int				screen_x;
	int				spr_height;
	int				spr_width;
	t_vect2i		start;
	t_vect2i		end;
	t_sprite		*tex;
}					t_dre;

typedef struct		s_asset
{
	char			symbol;
	int				mm_color;
	t_sprite		*tex;
}					t_asset;

typedef struct		s_player
{
	int				connected;
	t_vect2			pos;
	t_vect2			default_dir;
	t_vect2			dir;
	t_vect2			default_plane;
	t_vect2			plane;
	float			angle;
}					t_player;

typedef struct		s_entity
{
	int				alive;
	t_vect2			pos;
	t_vect2			dir;
	int				is_moving;
	float			dist_player;
	int				tex;
}					t_entity;

typedef struct		s_master
{
	char			*ip;
	int				port;
	char			mode;

	GLFWwindow		*window;
	unsigned int	shader_prog;
	unsigned int	vao;
	unsigned int	vbo;
	unsigned int	ebo;
	unsigned int	frame_tex;

	int				*frame;
	int				asset_count;
	t_asset			*assets;
	t_sprite		*ceil_tex;
	t_sprite		*floor_tex[FLOOR_TYPES];

	t_map			map;
	char			*map_name;
	int				map_x;
	int				map_y;

	uint32_t		pid;
	t_player		players[PLAYER_COUNT];
	t_entity		renderers[PLAYER_COUNT];
	int				connections[PLAYER_COUNT];
	uint32_t		connected;

	float			forward;
	float			rightward;
	int				rotation;

	int				files_saved;

	int				minimap;
	int				floor_mode;
	int				curve_mode;
	int				wobble_mode;
	int				cursor_enabled;

	float			z_buffer[WIN_WIDTH];

	t_sprite		*textures[TEXTURE_COUNT + PLAYER_SPRITE_COUNT + 1];
	t_entity		blob;
	t_entity		entities[ENTITY_COUNT];

	t_dre			dre;
	volatile int	draw_state[THREAD_COUNT];
	pthread_mutex_t	draw_mutexes[THREAD_COUNT];
	pthread_t		draw_threads[THREAD_COUNT];
	mach_port_t		draw_ports[THREAD_COUNT];

	double			prev_time;
	double			curr_time;
	float			mov_inc;

	uint8_t			client;
	int				socket;

	SOCKADDR_IN		addr;

	pthread_mutex_t	mutex;
	pthread_t		thread;
	volatile int	terminate;

	char			*path;
}					t_master;

enum				e_direction
{
	NORTH = 0,
	WEST,
	SOUTH,
	EAST
};

/*
** asset.c
*/
t_asset				*find_asset(char c, t_master *m);
int					init_assets(t_master *m);

/*
** dda.c
*/
void				init_dda(t_dda *dda, t_master *m, int x);
void				run_dda(t_dda *dda, t_master *m);

/*
** debug.c
*/
void				print_map(t_map *map);
int					move_entity_northward(t_entity *e, t_master *m);
int					move_entity_southward(t_entity *e, t_master *m);
int					move_entity_westward(t_entity *e, t_master *m);
int					move_entity_eastward(t_entity *e, t_master *m);

/*
** draw.c
*/
void				draw_dot(int x, int y, int color, t_master *m);

/*
** draw_mm.c
*/
int					get_mm_color(int x, int y, t_master *m);
void				draw_dot_mm(int x, int y, int color, t_master *m);
void				draw_mm(int x, int y, t_master *m);
void				draw_mm_border(t_master *m);
void				draw_mm_player(t_master *m);

/*
** entity.c
*/
void				update_entities(t_master *m);
void				destroy_entity(int id, t_master *m);
t_entity			*spawn_entity(int tex, t_vect2 *pos, t_master *m);
int					init_entities(t_master *m);

/*
** floor.c
*/
void				floor_cast(t_dda *dda, t_wall *wall, t_master *m, int x);

/*
** freedom.c
*/
int					free_all(t_master *m, int ret);

/*
** gl_init.c
*/
int					gl_init(t_master *m);

/*
** gl_init_shader.c
*/
int					gl_init_shader(t_master *m);

/*
** gl_callback.c
*/
void				key_callback(GLFWwindow *window, int key, int scancode,
	int action, int mods);
void				window_resize_callback(GLFWwindow *window,
	int width, int height);
void				error_callback(int error, const char *description);

/*
** keys_0.c
*/
void				terminate(t_master *m, int action);
void				toggle_mouse_cursor(t_master *m, int action);
void				save_ppm_hotkey(t_master *m, int action);
void				fire_bullet(t_master *m, int action);

/*
** keys_1.c
*/
void				move_forward(t_master *m, int action);
void				move_backward(t_master *m, int action);
void				move_leftward(t_master *m, int action);
void				move_rightward(t_master *m, int action);

/*
** keys_2.c
*/
void				rotate_left(t_master *m, float rot);
void				rotate_right(t_master *m, float rot);
void				rotate_left_hotkey(t_master *m, int action);
void				rotate_right_hotkey(t_master *m, int action);

/*
** keys_3.c
*/
void				toggle_minimap(t_master *m, int action);
void				switch_floor_mode(t_master *m, int action);
void				toggle_curve_mode(t_master *m, int action);
void				toggle_wobble_mode(t_master *m, int action);

/*
** map.c
*/
int					is_valid_pos(int x, int y, t_map *map);
int					is_valid_move(int x, int y, t_map *map);

/*
** minimap.c
*/
void				render_mm_th(t_master *m, int i);
void				render_mm_th_master(t_master *m);

/*
** mouse.c
*/
void				mouse_move_callback(GLFWwindow *window,
	double xpos, double ypos);

/*
** parse_map.c
*/
int					read_map_file(char *filename, t_master *m);

/*
** parse_player.c
*/
int					parse_player(int fd, t_master *m);
int					validate_player(t_master *m);

/*
** ppm.c
*/
int					save_ppm(t_master *m);

/*
** render.c
*/
void				render_vert(int x, t_master *m);
void				render(t_master *m);

/*
** render_entities.c
*/
void				render_entities(t_master *m);

/*
** render_th.c
*/
void				render_th(t_master *m, int i);
void				render_th_master(t_master *m);

/*
** sprite.c
*/
void				play_sound(char *file, char *path);
void				init_bgm(char *path);

/*
** sprite.c
*/
int					load_sprite(t_sprite **sprite, char *file, char *path);

/*
** texture.c
*/
int					gen_floor_tex(t_sprite **sprite);

/*
** update_entities.c
*/
void				update_entities(t_master *m);

/*
** update_player.c
*/
void				update_player_position(t_master *m);
void				update_player_rotation(t_master *m);

/*
** vect2.c
*/
void				v_normalize(t_vect2 *v);
void				v_rotate(t_vect2 *source, t_vect2 *dest, float angle);
void				rand_dir(t_vect2 *dir);

/*
** wall.c
*/
void				enforce_range(int *a, int limit);
void				build_wall(t_dda *dda, t_wall *wall, t_master *m, int x);

/*
** dre_th.c
*/
void				init_dre(t_dre *dre, t_entity *e, t_player *p, t_master *m);
void				dre_th(t_master *m, int i);
void				dre_th_master(t_master *m);

/*
** draw_thread.c
*/
# define DRAW_TH_IDLE 0
# define DRAW_TH_DRE 1
# define DRAW_TH_RENDER 2
# define DRAW_TH_MM 3
# define DRAW_TH_EXIT 4

typedef	struct		s_thread_args
{
	t_master		*m;
	int				i;
}					t_thread_args;

void				draw_thread_init(t_master *m);
void				draw_thread_deinit(t_master *m);

#endif
