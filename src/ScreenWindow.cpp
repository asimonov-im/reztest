/*
 * ScreenWindow.cpp
 *
 *  Created on: 2012-05-04
 *      Author: asimonov
 */

#include <bps/bps.h>
#include <bps/navigator.h>
#include <bps/screen.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include <screen/screen.h>
#include <stdlib.h>

#include "ScreenWindow.h"

ScreenWindow::ScreenWindow()
	: m_context(0)
	, m_window(0)
	, m_appWindow(0)
{
	bps_initialize();
}

ScreenWindow::~ScreenWindow()
{
	bps_shutdown();
}

void ScreenWindow::init()
{
	int rc;
	rc = screen_create_context(&m_context, 0);
//	if (rc) fprintf(stderr, "Fail at %d: %s\n", __LINE__, strerror(errno));

	rc = screen_create_window(&m_appWindow, m_context);

	char groupName[256];
	snprintf(groupName, 256, "screen-%d", getpid());
	rc = screen_create_window_group(m_appWindow, groupName);

	int sizeOfWindow[2] = {1024, 600};
	rc = screen_set_window_property_iv(m_appWindow, SCREEN_PROPERTY_SIZE, sizeOfWindow);


	int sizeOfBuffer[2] = {1024, 600};
	rc = screen_set_window_property_iv(m_appWindow, SCREEN_PROPERTY_BUFFER_SIZE, sizeOfBuffer);

	int format = SCREEN_FORMAT_RGBX8888;
	rc = screen_set_window_property_iv(m_appWindow, SCREEN_PROPERTY_FORMAT, &format);


	int usage = SCREEN_USAGE_NATIVE | SCREEN_USAGE_READ | SCREEN_USAGE_WRITE;
	rc = screen_set_window_property_iv(m_appWindow, SCREEN_PROPERTY_USAGE, &usage);


	int angle = 0;
	char *orientation = getenv("ORIENTATION");
	if (orientation) {
		 angle = atoi(orientation);
	}
	rc = screen_set_window_property_iv(m_appWindow, SCREEN_PROPERTY_ROTATION, &angle);


	rc = screen_create_window_buffers(m_appWindow, 1);


	screen_buffer_t windowBuffer[1];
	rc = screen_get_window_property_pv(m_appWindow,
				SCREEN_PROPERTY_RENDER_BUFFERS, (void**)&windowBuffer);

	int attribs[] = {SCREEN_BLIT_DESTINATION_X, 0,
					SCREEN_BLIT_DESTINATION_Y, 0,
					SCREEN_BLIT_DESTINATION_WIDTH, 1024,
					SCREEN_BLIT_DESTINATION_HEIGHT, 600,
					SCREEN_BLIT_COLOR, 0xff30ffff,
					SCREEN_BLIT_END};
	rc = screen_fill(m_context, windowBuffer[0], attribs);


	rc = screen_get_window_property_pv(m_appWindow,
					SCREEN_PROPERTY_RENDER_BUFFERS, (void**)&windowBuffer);


	int rect[4] = {0, 0, 1024, 600};
	rc = screen_post_window(m_appWindow, windowBuffer[0], 1, rect, 0);


	screen_request_events(m_context);
	navigator_request_events(0);
}

void ScreenWindow::quit()
{
	screen_stop_events(m_context);

	if (m_window)
		screen_destroy_window(m_window);
	if (m_appWindow)
		screen_destroy_window(m_appWindow);
	if (m_context)
		screen_destroy_context(m_context);
}

bool ScreenWindow::hasScreen()
{
	return (m_window != 0);
}

void ScreenWindow::setVideoMode(int width, int height)
{
	if (m_window) {
		screen_destroy_window(m_window);
		m_window = 0;
	}
	if (!m_window) {
		screen_create_window_type(&m_window, m_context, SCREEN_CHILD_WINDOW);
		char groupName[256];
		snprintf(groupName, 256, "screen-%d", getpid());
		screen_join_window_group(m_window, groupName);
	} else {
		screen_destroy_window_buffers(m_window);
	}

	m_width = width;
	m_height = height;

//#define NOSTRETCH
#define STRETCH_PRESERVE_ASPECT
#ifdef NOSTRETCH
	int position[2] = {ceil((1024-width)/2), ceil((600-height)/2)};
	screen_set_window_property_iv(m_window, SCREEN_PROPERTY_POSITION, position);

	int sizeOfWindow[2] = {width, height};
	screen_set_window_property_iv(m_window, SCREEN_PROPERTY_SIZE, sizeOfWindow);

	int sizeOfBuffer[2] = {width, height};
	screen_set_window_property_iv(m_window, SCREEN_PROPERTY_BUFFER_SIZE, sizeOfBuffer);
#elif defined(STRETCH_PRESERVE_ASPECT)
	width = ceil(width * 600 / height);
	height = 600;

	int position[2] = {ceil((1024-width)/2), ceil((600-height)/2)};
	screen_set_window_property_iv(m_window, SCREEN_PROPERTY_POSITION, position);

	int sizeOfWindow[2] = {width, height};
	screen_set_window_property_iv(m_window, SCREEN_PROPERTY_SIZE, sizeOfWindow);

	int sizeOfBuffer[2] = {m_width, m_height}; // OLD DIMS
	screen_set_window_property_iv(m_window, SCREEN_PROPERTY_BUFFER_SIZE, sizeOfBuffer);
#else
	int sizeOfWindow[2] = {1024, 600};
	screen_set_window_property_iv(m_window, SCREEN_PROPERTY_SIZE, sizeOfWindow);

	int sizeOfBuffer[2] = {width, height};
	screen_set_window_property_iv(m_window, SCREEN_PROPERTY_BUFFER_SIZE, sizeOfBuffer);
#endif

	int zOrder = 1;
	screen_set_window_property_iv(m_window, SCREEN_PROPERTY_ZORDER, &zOrder);

	int format = SCREEN_FORMAT_RGBX8888;
	screen_set_window_property_iv(m_window, SCREEN_PROPERTY_FORMAT, &format);

	int usage = SCREEN_USAGE_NATIVE | SCREEN_USAGE_READ | SCREEN_USAGE_WRITE;
	screen_set_window_property_iv(m_window, SCREEN_PROPERTY_USAGE, &usage);

	int angle = 0;
	char *orientation = getenv("ORIENTATION");
	if (orientation) {
		 angle = atoi(orientation);
	}
	screen_set_window_property_iv(m_window, SCREEN_PROPERTY_ROTATION, &angle);

	int visible = 1;
	screen_set_window_property_iv(m_window, SCREEN_PROPERTY_VISIBLE, &visible);

	screen_create_window_buffers(m_window, 1);
}

bool ScreenWindow::pollEvent(WinEvent *event)
{
	bps_event_t *bpsEvent;
	bps_get_event(&bpsEvent, 0);
	if (bpsEvent) {
		int domain = bps_event_get_domain(bpsEvent);
		if (domain == navigator_get_domain()) {
			if (bps_event_get_code(bpsEvent) == NAVIGATOR_EXIT) {
				event->type = QUIT;
				return true;
			}
		} else if (domain == screen_get_domain()) {
			screen_event_t se = screen_event_get_event(bpsEvent);
			int type;
			screen_get_event_property_iv(se, SCREEN_PROPERTY_TYPE, &type);
			if (type == SCREEN_EVENT_CLOSE) {
				event->type = QUIT;
				return true;
			} else if (type == SCREEN_EVENT_MTOUCH_TOUCH) {
				event->type = MOUSEBUTTONDOWN;
				return true;
			}
		}
	}
	return false;
}

unsigned int ScreenWindow::mapRGB(unsigned char r, unsigned char g, unsigned char b)
{
	return (0xff << 24) | (r << 16) | (g << 8) | b;
}

void ScreenWindow::fillRect(Rect *rect, unsigned int color)
{
	screen_buffer_t windowBuffer[1];
	screen_get_window_property_pv(m_window,
				SCREEN_PROPERTY_RENDER_BUFFERS, (void**)&windowBuffer);
	if (rect) {
		int attribs[] = {SCREEN_BLIT_DESTINATION_X, rect->x,
						SCREEN_BLIT_DESTINATION_Y, rect->y,
						SCREEN_BLIT_DESTINATION_WIDTH, rect->w,
						SCREEN_BLIT_DESTINATION_HEIGHT, rect->h,
						SCREEN_BLIT_COLOR, color,
						SCREEN_BLIT_END};
		screen_fill(m_context, windowBuffer[0], attribs);
	} else {
		int attribs[] = {SCREEN_BLIT_DESTINATION_X, 0,
						SCREEN_BLIT_DESTINATION_Y, 0,
						SCREEN_BLIT_DESTINATION_WIDTH, m_width,
						SCREEN_BLIT_DESTINATION_HEIGHT, m_height,
						SCREEN_BLIT_COLOR, color,
						SCREEN_BLIT_END};
		screen_fill(m_context, windowBuffer[0], attribs);
	}
}

void ScreenWindow::flip()
{
	int rect[4] = {0, 0, 1024, 600};
	screen_buffer_t windowBuffer[1];
	screen_get_window_property_pv(m_window,
				SCREEN_PROPERTY_RENDER_BUFFERS, (void**)&windowBuffer);
	screen_post_window(m_window, windowBuffer[0], 1, rect, 0);
}
