/*
 * ScreenWindow.h
 *
 *  Created on: 2012-05-04
 *      Author: asimonov
 */

#ifndef SCREENWINDOW_H_
#define SCREENWINDOW_H_

#include "window.h"

struct _screen_context;
typedef struct _screen_context *screen_context_t;

struct _screen_window;
typedef struct _screen_window *screen_window_t;

class ScreenWindow : public Window
{
public:
	ScreenWindow();
	virtual ~ScreenWindow();

	virtual void init();
	virtual void quit();
	virtual bool hasScreen();

	virtual void setVideoMode(int width, int height);
	virtual bool pollEvent(WinEvent *event);

	virtual unsigned int mapRGB(unsigned char r, unsigned char g, unsigned char b);
	virtual void fillRect(Rect *rect, unsigned int color);
	virtual void flip();

private:
	screen_context_t m_context;
	screen_window_t m_appWindow;
	screen_window_t m_window;
	int m_width;
	int m_height;
};

#endif /* SCREENWINDOW_H_ */
