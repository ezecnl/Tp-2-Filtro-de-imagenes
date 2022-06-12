#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "ppm.h"
#include <atomic>


// SINGLE-THREAD FILTERS

void blackWhite(ppm& img, unsigned int start, unsigned int end);
void contrast(ppm& img, float contrast, unsigned int start, unsigned int end);
void brightness(ppm& img, float b);
void shades(ppm& img, unsigned char shades);
void merge(ppm& img1, ppm& img2, float p1);
void frame(ppm& img, int x, unsigned int start, unsigned int end);
void boxBlur(ppm &img, unsigned int start, unsigned int end);
void zoom(ppm &img_out, ppm &img, int n);
void edgeDetection(ppm &img, ppm &img_target);
void plain(ppm &img, unsigned char c);

// MULTI-THREAD FILTERS

void blackWhiteMultiThread(ppm& img, int n_threads);
void contrastMultiThread(ppm& img, int n_threads, float contrast);
void boxBlurMultiThread(ppm& img, int n_threads);
void frameMultiThread(ppm& img, int n_threads, int x);

#endif