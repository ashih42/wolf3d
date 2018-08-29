CC := gcc

TARGET := wolf3d

INCLUDES := includes/

GLFW_LOC := $(shell brew --prefix glfw)
GLFW_INC := $(GLFW_LOC)/include/
GLFW_LINK := -L $(GLFW_LOC)/lib/ -lglfw

LIBFT := libft/
LIBFT_INC := $(LIBFT)includes/
LIBFT_LIB := $(LIBFT)libft.a

GLAD_OBJ := objs/glad.o
GLAD_INC := glad/include/

CFLAGS := -Wall -Werror -Wextra #-g -fsanitize=address 
HEADERS := -I $(INCLUDES) -I $(LIBFT_INC) -I $(GLFW_INC) -I $(GLAD_INC)

SRCS := main.c \
asset.c \
dda.c \
debug.c \
draw.c \
draw_mm.c \
draw_thread.c \
dre_th.c \
entity.c \
floor.c \
freedom.c \
gl_init.c \
gl_init_shader.c \
gl_callback.c \
keys_0.c \
keys_1.c \
keys_2.c \
keys_3.c \
map.c \
minimap.c \
mouse.c \
parse_map.c \
parse_player.c \
ppm.c \
render.c \
render_entities.c \
render_th.c \
sound.c \
sprite.c \
texture.c \
update_entities.c \
update_player.c \
vect2.c \
wall.c \
netplay/client.c \
netplay/dispatch_packet.c \
netplay/netplay.c \
netplay/recv_packet.c \
netplay/send_packet.c \
netplay/server.c \
netplay/server_init.c

SRCDIR := srcs/
OBJDIR := objs/
OBJS := $(addprefix $(OBJDIR), $(SRCS:.c=.o))

all: glfw $(LIBFT_LIB) $(GLAD_OBJ) $(TARGET)

glfw:
	@echo "\x1b[1mInstalling glfw...\x1b[0m"
	@HOMEBREW_NO_AUTO_UPDATE=1 brew install glfw
	@echo

$(LIBFT_LIB):
	@echo "\x1b[1mBuilding $(LIBFT) library...\x1b[0m"
	@make -C $(LIBFT)
	@echo

$(GLAD_OBJ):
	@echo "\x1b[1mBuilding glad.o...\x1b[0m"
	@gcc glad/src/glad.c -c -I $(GLAD_INC)
	@mkdir -p $(OBJDIR)
	@mv glad.o $(OBJDIR)
	@echo

$(OBJDIR)%.o: $(SRCDIR)%.c
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)netplay/
	$(CC) -c $(CFLAGS) $(HEADERS) $< -o $@

$(TARGET): $(OBJS)
	@echo "\x1b[1mBuilding $(TARGET)...\x1b[0m"
	$(CC) -o $(TARGET) $(OBJS) $(GLAD_OBJ) -L$(LIBFT) -lft \
		-lpthread \
		$(GLFW_LINK) #\
#		-fsanitize=address
	@echo "\x1b[1mBuild finished!!\x1b[0m"

clean:
	@echo "\x1b[1mCleaning...\x1b[0m"
	#make -C $(LIBFT) clean
	/bin/rm -rf $(OBJDIR)
	@echo

fclean: clean
	@echo "\x1b[1mFcleaning...\x1b[0m"
	#/bin/rm -f $(LIBFT_LIB)
	/bin/rm -f $(TARGET)
	@echo

re: fclean all

.PHONY: all glfw clean fclean re
