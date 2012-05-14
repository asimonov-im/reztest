/*
 * window.cpp
 *
 *  Created on: 2012-05-04
 *      Author: asimonov
 */

#include "SDLWindow.h"
#include "ScreenWindow.h"

//#define USE_SDL

Window::~Window()
{

}

Window *WindowFactory::createWindow()
{
#if defined(__QNXNTO__) && !defined(USE_SDL)
	return new ScreenWindow();
#else
	return new SDLWindow();
#endif
}
