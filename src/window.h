/*
 * window.h
 *
 *  Created on: 2012-05-04
 *      Author: asimonov
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <stdint.h>

typedef enum {
	MOUSEBUTTONDOWN = 5,
	QUIT = 12
} WinEventType;

typedef struct Rect {
	int16_t x, y;
	uint16_t w, h;
} Rect;

typedef struct Event {
	int type;
} WinEvent;

class Window
{
public:
	virtual ~Window();

	virtual void init() = 0;
	virtual void quit() = 0;
	virtual bool hasScreen() = 0;

	virtual void setVideoMode(int width, int height) = 0;
	virtual bool pollEvent(WinEvent *event) = 0;

	virtual unsigned int mapRGB(unsigned char r, unsigned char g, unsigned char b) = 0;
	virtual void fillRect(Rect *rect, unsigned int color) = 0;
	virtual void flip() = 0;
};

class WindowFactory {
public:
	static Window *createWindow();
};

#endif /* WINDOW_H_ */
