/*
 * SDLWindow.cpp
 *
 *  Created on: 2012-05-04
 *      Author: asimonov
 */

#include "SDLWindow.h"
#include "SDL.h"

SDLWindow::SDLWindow() {
	m_screen = 0;
}

SDLWindow::~SDLWindow() {
	if (m_screen) {
		quit();
	}
}

void SDLWindow::init() {
	SDL_Init(SDL_INIT_VIDEO);
}

void SDLWindow::quit() {
	SDL_Quit();
}

bool SDLWindow::hasScreen() {
	return (m_screen != 0);
}

void SDLWindow::setVideoMode(int width, int height) {
	m_screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE);
}

bool SDLWindow::pollEvent(WinEvent *event) {
	SDL_Event sdlEvent;
	if (SDL_PollEvent(&sdlEvent)) {
		event->type = sdlEvent.type;
		return true;
	} else {
		return false;
	}
}

unsigned int SDLWindow::mapRGB(unsigned char r, unsigned char g,
		unsigned char b) {
	return SDL_MapRGB(m_screen->format, r, g, b);
}

void SDLWindow::fillRect(Rect *rect, unsigned int color) {
	SDL_FillRect(m_screen, (SDL_Rect *)rect, color);
}

void SDLWindow::flip()
{
	SDL_Flip(m_screen);
}
