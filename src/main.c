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
	return (true);
}

bool	close_window(t_donto_man *donto_man)
{
	XFreeGC(donto_man->display, donto_man->gc);
	XDestroyWindow(donto_man->display, donto_man->window);
	XCloseDisplay(donto_man->display);
	return (true);
}

void	draw_scene(t_donto_man *donto_man)
{
	XSetForeground(donto_man->display, donto_man->gc, 0xFF0000);
	XFillRectangle(
			donto_man->display,
			donto_man->window,
			donto_man->gc,
			10, 10,
			50, 50);
}

int	main(void)
{
	ma_result result;
	ma_engine engine;

	t_donto_man	donto_man;
	bool		status;
	bool		window_should_close = false;

	status = init_window(&donto_man);
	if (!status) {
		trace_log(ERROR, "Cannot init window");
		exit(EXIT_FAILURE);
	}
	result = ma_engine_init(NULL, &engine);
	if (result != MA_SUCCESS) {
		trace_log(ERROR, "Failed to initialize audio engine.");
		exit(EXIT_FAILURE);
	}
	ma_engine_play_sound(&engine, "./resources/hurry.wav", NULL);
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
		draw_scene(&donto_man);
	}
	ma_engine_uninit(&engine);
	close_window(&donto_man);
	return (0);
}

#if 0
int main(int ac, char **av)
{
	ma_result result;
	ma_engine engine;

	result = ma_engine_init(NULL, &engine);
	if (result != MA_SUCCESS) {
		printf("Failed to initialize audio engine.");
		return -1;
	}

	ma_engine_play_sound(&engine, av[1], NULL);

	printf("Press Enter to quit...");
	getchar();

	ma_engine_uninit(&engine);

	return 0;
}
#endif
