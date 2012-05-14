/*
 * main.cpp
 *
 *  Created on: 2012-05-04
 *      Author: asimonov
 */

#include "window.h"

int main(int argc, char *argv[])
{
	int currentMode = 0;
	Window *w = WindowFactory::createWindow();
	w->init();
	w->setVideoMode(1024, 600);
	if (!w->hasScreen()) {
		w->quit();
		return 1;
	}

	WinEvent event;
	bool done = false;
	while (!done) {
		while (w->pollEvent(&event)) {
			switch (event.type) {
			case QUIT:
				done = true;
				break;
			case MOUSEBUTTONDOWN:
				switch (currentMode) {
				case 0:
					w->setVideoMode(800, 600);
					currentMode = 1;
					break;
				case 1:
					w->setVideoMode(640, 480);
					currentMode = 2;
					break;
				case 2:
					w->setVideoMode(1024, 600);
					currentMode = 0;
					break;
				}

				break;
			}
		}

		unsigned int color = w->mapRGB(0, 0, 0);
		switch (currentMode) {
		case 0:
			color = w->mapRGB(0x80, 0, 0);
			break;
		case 1:
			color = w->mapRGB(0, 0x80, 0);
			break;
		case 2:
			color = w->mapRGB(0, 0, 0x80);
			break;
		}
		w->fillRect(0, color);

		Rect rect;
		rect.x = 100;
		rect.y = 100;
		rect.w = 540;
		rect.h = 380;
		w->fillRect(&rect, w->mapRGB(0xff, 0x0, 0x0));

		w->flip();
	}

	w->quit();
	return 0;
}
