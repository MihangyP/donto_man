#ifndef DONTO_MAN_H
#define DONTO_MAN_H

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define W_WIDTH 800
#define W_HEIGHT 600
#define BACKGROUND_COLOR 0x663300

typedef enum e_log_level
{
	INFO = 0,
	DEBUG,
	WARNING,
	ERROR
}	t_log_level;

typedef	struct s_img
{
	XImage	*ximg;
	unsigned char *img_data;
	int width;
	int height;
}	t_img;

typedef	struct s_music
{
	ma_result result;
	ma_engine engine;
}	t_music;

typedef struct s_donto_man
{
	Display	*display;
	Window	window;
	XEvent	event;
	GC		gc;
	int		screen;
	Atom	wm_delete_window;
	t_music	music;
}	t_donto_man;

#endif // DONTO_MAN_H
