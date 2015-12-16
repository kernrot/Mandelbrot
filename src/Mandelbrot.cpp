// Interactive Mandelbrot - 2014 - Conrad Henke
// small example of a interactive Mandelbrot in C++ with SDL (SDL-1.2.15) and OpenMP

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <tchar.h>

#include "..\lib\include\SDL.h"

#include "math.h"
#include "omp.h"

#pragma comment(lib, "SDL.lib")

#include "Mandelbrot.h"

SDL_Surface *surface;
SDL_Event *evt;

int _tmain(int argc, _TCHAR* argv[])
{
	// default window resolution
	int resX = 800;
	int resY = 600;

	resX = (_ttoi(argv[1]) > 50) ? _ttoi(argv[1]) : resX;
	resY = (_ttoi(argv[2]) > 50) ? _ttoi(argv[2]) : resY;

	printf("Interactive Mandelbrot - 2014 - Conrad Henke (mandelbrot@conradhenke.de\n\n");
	printf("interact by using following keys in the graphics window:\n arrow-keys to navigate\n page-up/down to zoom\n num+/num- to change iterations\n d to ouptu debug information\n ESC to exit");
	printf("\n\nyou can set the resolution of the graphics window:\n run command-line arguments like: \"Mandelbrot.exe 1280 960\"\n (4:3 resolutions look best)");

	evt = new SDL_Event;
	SDL_Init(SDL_INIT_VIDEO);
	surface = SDL_SetVideoMode(resX, resY, 32, SDL_SWSURFACE);
	SDL_WM_SetCaption("Interactive Mandelbrot - 2014 - Conrad Henke", 0);

	SDL_LockSurface(surface);

	// paint

	for (int x = 0; x < resX; x++)
	{
		for (int y = 0; y < resY; y++)
		{
			paint(x, y, colorFromSpectrum((double)x / resX, surface), surface);
		}
	}

	SDL_UnlockSurface(surface);
	SDL_Flip(surface);

	while (evt->type != SDL_KEYDOWN) SDL_PollEvent(evt);

	double zoom = 1.0;
	int vZoom = 0;
	double shiftX = 0.0;
	int vX = 0;
	double shiftY = 0.0;
	int vY = 0;

	bool debug = false;

	int initIter = 50, currIter = 50;
	int userIter = 0, vUserIter = 0;

	bool needRefresh = true;
	bool quit = false;

	do {
		SDL_LockSurface(surface);

		if (needRefresh) {
			paintMandel(resX, resY, surface, zoom, shiftX, shiftY, currIter, (currIter - initIter));
			needRefresh = false;
		}

		SDL_UnlockSurface(surface);
		SDL_Flip(surface);

		if ((SDL_PollEvent(evt) == 1 && (evt->type == SDL_KEYDOWN || evt->type == SDL_KEYUP))
			|| (vX != 0 || vY != 0 || vZoom != 0 || vUserIter != 0)){
			handleKey(*evt, vX, vY, vZoom, vUserIter, quit, debug);

			shiftX += 0.02 * zoom * vX;
			shiftY += 0.02 * zoom * vY;
			zoom -= 0.02 * zoom * vZoom;
			userIter += vUserIter;

			currIter = initIter - ((int)log(zoom*zoom*zoom)) + userIter;
			needRefresh = true;

			if (debug)
			{
				// debug output
				printf("S: sx= %f sy= %f \t sz= %f i= %d (%d)\n", shiftX, shiftY, zoom, currIter, userIter);
				printf("V: vx= %d vy= %d \t vz= %d \n", vX, vY, vZoom);
			}
		}
	} while (!quit);

	return 0;
}

void paint(int x, int y, unsigned int color, SDL_Surface *surface)		// zeichnet punkt bei x,y in farbe color
{
	unsigned char *scr = (unsigned char *)(surface->pixels);
	scr += y * surface->pitch + x * 4;
	*((unsigned int *)scr) = color;
}

void paintCircle(int cx, int cy, int r, int thick, unsigned int color, SDL_Surface *surface) // zeichnet kreis mit mittelpunkt x,y in farbe color, radius r und dicke nach innen thick
{
	int x, y = cy;
	for (x = cx - r; x < cx + r; x++)
		for (y = cy - r; y < cy + r; y++)

			if (thick == 0){
				if (distance(cx, cy, x, y) < r)
					paint(x, y, color, surface);
			}
			else{
				if (distance(cx, cy, x, y) < r && distance(cx, cy, x, y) >= r - thick)
					paint(x, y, color, surface);
			}
}

int distance(int x1, int y1, int x2, int y2)	// gibt die entfernung zweier punkte zueinander zurück
{
	return sqrt((float)((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)));
}

unsigned int color(int r, int g, int b, SDL_Surface *surface)
{
	return SDL_MapRGB(surface->format, r, g, b);
}

unsigned int colorFromPalette(double value, SDL_Surface *surface)
{
	int pickedValue = 50 * (value);
	return SDL_MapRGB(surface->format, pickedValue, pickedValue, pickedValue);
}

unsigned int colorFromSpectrum(double value, SDL_Surface *surface)
{
	int pickedLambda = (650 - 400) * (value)+400;
	double r = 0, g = 0, b = 0;
	spectral_color(r, g, b, pickedLambda);
	return SDL_MapRGB(surface->format, r * 255, g * 255, b * 255);
}

void spectral_color(double &r, double &g, double &b, double l) // RGB <- lambda l = < 380,780 > [nm]
{
	// from http://stackoverflow.com/questions/22141206/how-do-i-draw-a-rainbow-in-freeglut

	double M_PI = 3.14159265358979324;
	if (l < 380.0) r = 0.00;
	else if (l < 400.0) r = 0.05 - 0.05*sin(M_PI*(l - 366.0) / 33.0);
	else if (l < 435.0) r = 0.31*sin(M_PI*(l - 395.0) / 81.0);
	else if (l < 460.0) r = 0.31*sin(M_PI*(l - 412.0) / 48.0);
	else if (l < 540.0) r = 0.00;
	else if (l < 590.0) r = 0.99*sin(M_PI*(l - 540.0) / 104.0);
	else if (l < 670.0) r = 1.00*sin(M_PI*(l - 507.0) / 182.0);
	else if (l < 730.0) r = 0.32 - 0.32*sin(M_PI*(l - 670.0) / 128.0);
	else              r = 0.00;
	if (l < 454.0) g = 0.00;
	else if (l < 617.0) g = 0.78*sin(M_PI*(l - 454.0) / 163.0);
	else              g = 0.00;
	if (l < 380.0) b = 0.00;
	else if (l < 400.0) b = 0.14 - 0.14*sin(M_PI*(l - 364.0) / 35.0);
	else if (l < 445.0) b = 0.96*sin(M_PI*(l - 395.0) / 104.0);
	else if (l < 510.0) b = 0.96*sin(M_PI*(l - 377.0) / 133.0);
	else              b = 0.00;
}

void paintMandel(int maxX, int maxY, SDL_Surface *surface, double zoom, double shiftx, double shifty, int maxIter, int minIter)
{
#pragma omp parallel for schedule(dynamic)
	for (int pX = 0; pX < maxX; pX++)
		for (int pY = 0; pY < maxY; pY++)
		{
			//default view
			double x = ((pX / (double)maxX) * 3) - 2;		// -2 .. 1
			double y = ((pY / (double)maxY) * 3) - 1.5;		// -1.5 .. 1.5

			//view modifiers
			x = x*zoom + shiftx;
			y = y*zoom + shifty;

			int iter = mandelIterations(x, y, maxIter);

			if (iter >= maxIter) paint(pX, pY, color(0, 0, 0, surface), surface);
			else paint(pX, pY, colorFromSpectrum(((iter) / (double)(maxIter)), surface), surface);

			//printf("P %d %d - Iter: %d \n", pX, pY, iter);
		}
}

int mandelIterations(double x0, double y0, int maxIterations)
{
	double x = 0;
	double y = 0;
	int iteration = 0;

	while (x*x + y*y < 2 * 2 && iteration < maxIterations)
	{
		double xtemp = x*x - y*y + x0;
		y = 2 * x*y + y0;
		x = xtemp;
		iteration++;
	}
	return iteration;
}

void handleKey(SDL_Event env, int &vx, int &vy, int &vzoom, int &vUserIter, bool &quit, bool &debug) {
	switch (env.type){
	case SDL_KEYDOWN:

		switch (env.key.keysym.sym){
		case SDLK_LEFT:			vx = -1;		break;
		case SDLK_RIGHT:		vx = 1;			break;
		case SDLK_UP:			vy = -1;		break;
		case SDLK_DOWN:			vy = 1;			break;
		case SDLK_PAGEUP:		vzoom = -1;		break;
		case SDLK_PAGEDOWN:		vzoom = 1;		break;
		case SDLK_KP_MINUS:	vUserIter = -1;		break;
		case SDLK_KP_PLUS:		vUserIter = 1;	break;
		case SDLK_d:			debug = !debug;	break;
		default:break;
		}		break;

	case SDL_KEYUP:
		switch (env.key.keysym.sym){
		case SDLK_LEFT:		if (vx != 0) 	vx = 0; 	break;
		case SDLK_RIGHT:	if (vx != 0)	vx = 0;		break;
		case SDLK_UP:		if (vy != 0)	vy = 0;		break;
		case SDLK_DOWN:		if (vy != 0)	vy = 0;		break;
		case SDLK_PAGEUP:	if (vzoom != 0) vzoom = 0;	break;
		case SDLK_PAGEDOWN: if (vzoom != 0) vzoom = 0;	break;
		case SDLK_KP_MINUS:	if (vUserIter != 0) vUserIter = 0;	break;
		case SDLK_KP_PLUS:  if (vUserIter != 0) vUserIter = 0;	break;
		default:break;
		}	break;

	default:break;
	}

	// Check for ESC
	if (env.key.keysym.sym == SDLK_ESCAPE) quit = true;
}