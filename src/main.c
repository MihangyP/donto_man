#include "donto_man.h"

void	trace_log(t_log_level log_level, const char *message)
{
	switch (log_level) {
		case INFO:{
			dprintf(STDOUT_FILENO, "[INFO]: %s\n", message);
		} break;
		case DEBUG: {
			dprintf(STDOUT_FILENO, "[DEBUG]: %s\n", message);
		} break;
		case WARNING: {
			dprintf(STDERR_FILENO, "[WARNING]: %s\n", message);
		} break;
		case ERROR: {
			dprintf(STDERR_FILENO, "[ERROR]: %s\n", message);
		} break;
		default:
			dprintf(STDERR_FILENO, "Unknown log_level\n");
	}
}

bool	init_window(t_donto_man *donto_man)
{
	donto_man->display = XOpenDisplay(NULL);
	if (!donto_man->display) return (false);
	donto_man->screen = DefaultScreen(donto_man->display);
	donto_man->window = XCreateSimpleWindow(
						donto_man->display,
						DefaultRootWindow(donto_man->display),
						0, 0,
						W_WIDTH, W_HEIGHT,
						0,
						0, 0);
	donto_man->gc = XCreateGC(donto_man->display, donto_man->window, 0, NULL);
	donto_man->wm_delete_window = XInternAtom(donto_man->display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(donto_man->display, donto_man->window, &donto_man->wm_delete_window, 1);
	XSelectInput(donto_man->display, donto_man->window, KeyPressMask);
	XMapWindow(donto_man->display, donto_man->window);
	XFlush(donto_man->display);

	donto_man->music.result = ma_engine_init(NULL, &donto_man->music.engine);
	if (donto_man->music.result != MA_SUCCESS) return (false);
	return (true);
}

bool	close_window(t_donto_man *donto_man)
{
	XFreeGC(donto_man->display, donto_man->gc);
	XDestroyWindow(donto_man->display, donto_man->window);
	XCloseDisplay(donto_man->display);
	ma_engine_uninit(&donto_man->music.engine);
	return (true);
}

void	clear_background(t_donto_man *donto_man, int color) {
	XSetForeground(donto_man->display, donto_man->gc, color);
	for (size_t y = 0; y < W_HEIGHT; ++y) {
		for (size_t x = 0; x < W_WIDTH; ++x)
			XDrawPoint(donto_man->display, donto_man->window, donto_man->gc, x, y);
	}
}

void	draw_scene(t_donto_man *donto_man, t_img *img)
{
	clear_background(donto_man, BACKGROUND_COLOR);
	XPutImage(donto_man->display, donto_man->window, donto_man->gc, img->ximg, 0, 0, 0, 0, img->width, img->height);
}

// TODO: remove leaks when playing music
int	main(void)
{
	t_donto_man	donto_man;
	t_img		player;
	bool		status;
	bool		window_should_close = false;
	unsigned char *img_data;

	status = init_window(&donto_man);
	if (!status) {
		trace_log(ERROR, "Cannot init window");
		exit(EXIT_FAILURE);
	}
	// Play sound
	ma_engine_play_sound(&donto_man.music.engine, "./resources/hurry.wav", NULL);
	// Load img
	int	width, height;
	player.img_data = stbi_load("./resources/donto_man.png", &width, &height, NULL, 4);
	if (!player.img_data) {
		trace_log(ERROR, "Failed to load image");
		exit(EXIT_FAILURE);
	}
	player.ximg = XCreateImage(
				donto_man.display,
				DefaultVisual(donto_man.display, donto_man.screen),
				24,
				ZPixmap,
				0,
				(char *)player.img_data,
				width, height,
				32, 0);
	if (!player.ximg) {
		trace_log(ERROR, "Failed to create XImage");
		exit(EXIT_FAILURE);
	}
	while (!window_should_close) {
		while (XPending(donto_man.display) > 0) {
			XNextEvent(donto_man.display, &donto_man.event);
			switch (donto_man.event.type) {
				case KeyPress: {
				   KeySym keysym = XLookupKeysym(&donto_man.event.xkey, 0);
				   if (keysym == XK_Escape)
					   window_should_close = true;
				} break;
				case ClientMessage: {
					if ((Atom)donto_man.event.xclient.data.l[0] == donto_man.wm_delete_window)
						window_should_close = true;
				} break;
				default:
					trace_log(WARNING, "Unknown event type");
			}
		}
		draw_scene(&donto_man, &player);
	}
	stbi_image_free(player.img_data);
	close_window(&donto_man);
	return (0);
}
