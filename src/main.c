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
	XSelectInput(donto_man->display, donto_man->window, KeyPressMask);
	XMapWindow(donto_man->display, donto_man->window);
	XFlush(donto_man->display);
	return (true);
}

bool	close_window(t_donto_man *donto_man)
{
	XCloseDisplay(donto_man->display);
	return (true);
}

int	main(void)
{
	t_donto_man	donto_man;
	bool		status;
	bool		window_should_close = false;

	status = init_window(&donto_man);
	if (!status) {
		trace_log(ERROR, "Cannot init window");
		return (1);	
	}
	while (!window_should_close) {
		while (XPending(donto_man.display)) {
			XNextEvent(donto_man.display, &donto_man.event);
			if (donto_man.event.type == KeyPress) {
				KeySym keysym = XLookupKeysym(&donto_man.event.xkey, 0);
				if (keysym == XK_Escape) {
					window_should_close = true;
				}
			}
		}
	}
	close_window(&donto_man);
	return (0);
}
