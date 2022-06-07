#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "ppm.h"
#include <atomic>


// SINGLE-THREAD FILTERS

void blackWhite(ppm& img, int start, int end);
void contrast(ppm& img, float contrast, int start, int end);
void brightness(ppm& img, float b, int start, int end);
void shades(ppm& img, unsigned char shades);
void merge(ppm& img1, ppm& img2, float p1);
void frame(ppm& img, int x);
void boxBlur(ppm &img, int start, int end);
void zoom(ppm &img_out, ppm &img, int n);
void edgeDetection(ppm &img, ppm &img_target);
void plain(ppm &img, unsigned char c);

// MULTI-THREAD FILTERS

void blackWhiteMultiThread(ppm& img, int n_threads);
void contrastMultiThread(ppm& img, int n_threads);
void boxBlurMultiThread(ppm& img, int n_threads);


#endif