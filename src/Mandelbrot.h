// Interactive Mandelbrot - 2014 - Conrad Henke
// small example of a interactive Mandelbrot in C++ with SDL (SDL-1.2.15) and OpenMP

#pragma once

void paint(int x, int y, unsigned int color, SDL_Surface *surface);
void paintCircle(int cx, int cy, int r, int thick, unsigned int color, SDL_Surface *surface);
unsigned int color(int r, int g, int b, SDL_Surface *surface);
unsigned int colorFromPalette(double value, SDL_Surface *surface);
unsigned int colorFromSpectrum(double value, SDL_Surface *surface);
void spectral_color(double &r, double &g, double &b, double l);
void paintMandel(int maxX, int maxY, SDL_Surface *surface, double zoom, double shiftx, double shifty, int maxIter, int minIter);
int mandelIterations(double x0, double y0, int maxIterations);
char Taste(int d);
void handleKey(SDL_Event env, int &vx, int &vy, int &vzoom, int &vUserIter, bool &quit, bool &debug);
int distance(int x1, int y1, int x2, int y2);