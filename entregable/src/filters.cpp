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
void truncate(int& num)
{
	if(num > 255)
	{num=255; }
	if (num < 0)
	{num = 0; }
}

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
	
	for(int i = 0; i < img.height; i++)
	{
		for(int j = 0; j < img.width; j++)
		{	
			float x = (259.0 * (contrast + 255.0)) / (255.0 * (259.0 - contrast));
			int r   = ((x * (img.getPixel(i,j).r - 128)) + 128);
			int g = ((x * (img.getPixel(i,j).g - 128)) + 128);
			int b  = ((x * (img.getPixel(i,j).b - 128)) + 128);
			truncate(r);
			truncate(g);
			truncate(b);
			

			img.setPixel(i,j,pixel(r,g,b));
		}			
			
	}
}
void frame(ppm& img, int x)
{
	int fAnteultimas = img.width - x;
	int cAnteultimas = img.height - x;

	for(int i = 0; i < img.height; i++)//columna
	{
		for(int j = 0; j < img.width; j++)//fila
		{	
			if(j<=x or j >= fAnteultimas ) //agarra las primeras x filas y las ultimas x filas
			{
				img.setPixel(i,j,pixel(0,0,0));
			}

			if(i<=x or i >= cAnteultimas ) //agarra las primeras x columnas y las ultimas x columnas
			{
				img.setPixel(i,j,pixel(0,0,0));
			}
		}
					
			
	}
}

