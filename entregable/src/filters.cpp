#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>       /* sqrt */
#include <vector>
#include "filters.h"
#include <thread>  
#include <atomic>

  

#define BLACK 0

using namespace std;

// COMPLETAR :)

// Filtro plano como ejemplo

void plain(ppm& img, unsigned char c)
{

	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++)			
			img.setPixel(i, j, pixel(c,c,c));

}

void blackWhite(ppm& img)
{
	
	for(int i = 0; i < img.height; i++)
	{
		for(int j = 0; j < img.width; j++)
		{
			int r = img.getPixel(i,j).r;
			int g = img.getPixel(i,j).g;
			int b = img.getPixel(i,j).b;
			int rgb = (r + g + b)/3;
			if(rgb > 255)
			{
				rgb = 255;
			}
			img.setPixel(i, j, pixel(rgb,rgb,rgb));
		}			
			
	}	
}
void contrast(ppm& img, float contrast)
{
	float x = (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));
	for(int i = 0; i < img.height; i++)
	{
		for(int j = 0; j < img.width; j++)
		{	
			int r   = ((x * (img.getPixel(i,j).r - 128)) + 128);
			int g = ((x * (img.getPixel(i,j).g - 128)) + 128);
			int b  = ((x * (img.getPixel(i,j).b - 128)) + 128);
			if(r > 255)
			{ r=255;}
			if(g > 255)
			{ g=255;}
			if(b > 255)
			{ b=255;}

			if(r < 0)
			{ r=0;}
			if(g < 0)
			{ g=0;}
			if(b < 0)
			{ b=0;}

			img.setPixel(i,j,pixel(r,g,b));
		}			
			
	}
}

