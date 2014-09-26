// stdafx.h : Includedatei für Standardsystem-Includedateien
// oder häufig verwendete projektspezifische Includedateien,
// die nur in unregelmäßigen Abständen geändert werden.
//

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
void handleKey(SDL_Event env, int &vx, int &vy, int &vzoom, int &vUserIter, bool &quit);
int distance(int x1, int y1, int x2, int y2);


// TODO: Hier auf zusätzliche Header, die das Programm erfordert, verweisen.

