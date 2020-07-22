#include <string>
#include "wtypes.h"

class Window {

private:
	int height;
	int width;

public:
	Window() {
		RECT desktop;
		// Get a handle to the desktop window
		const HWND hDesktop = GetDesktopWindow();
		// Get the size of screen to the variable desktop
		GetWindowRect(hDesktop, &desktop);
		// The top left corner will have coordinates (0,0)
		// and the bottom right corner will have coordinates
		// (horizontal, vertical)
		width = desktop.right;
		height = desktop.bottom;
	}

	int getHeight() {
		return height;
	}
	int getWidth() {
		return width;
	}

};