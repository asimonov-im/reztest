/*
 * SDLWindow.h
 *
 *  Created on: 2012-05-04
 *      Author: asimonov
 */

#ifndef SDLWINDOW_H_
#define SDLWINDOW_H_

#include "window.h"

struct SDL_Surface;

class SDLWindow : public Window
{
public:
	SDLWindow();
	virtual ~SDLWindow();

	virtual void init();
	virtual void quit();
	virtual bool hasScreen();

	virtual void setVideoMode(int width, int height);
	virtual bool pollEvent(WinEvent *event);

	virtual unsigned int mapRGB(unsigned char r, unsigned char g, unsigned char b);
	virtual void fillRect(Rect *rect, unsigned int color);
	virtual void flip();
private:
	SDL_Surface *m_screen;
};

#endif /* SDLWINDOW_H_ */
