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

void merge(ppm& img1, ppm& img2, float p1)
{
	//Asumimos que las imagenes deben ser del mismo tamaño.
	float p2 = 1 - p1;

	for(int i = 0; i < img1.height; i++)//columna
	{
		for(int j = 0; j < img1.width; j++)//fila
		{	
			
					
			int R1 =img1.getPixel(i,j).r;
			int G1 =img1.getPixel(i,j).g;
			int B1 =img1.getPixel(i,j).b;
			
			int R2 =img2.getPixel(i,j).r;
			int G2 =img2.getPixel(i,j).g;
			int B2 =img2.getPixel(i,j).g;

			int rFinal = R1*p1 + R2*p2;
			int gFinal = G1*p1 + G2*p2;
			int bFinal = B1*p1 + B2*p2;

			truncate(rFinal);
			truncate(gFinal);
			truncate(bFinal);
			
			img1.setPixel(i,j, pixel(rFinal,gFinal,bFinal));
		}
				
				
			
	}

}

void zoom(ppm &img, ppm &img_out, int n)
{
	//Recorro la foto original y saco los pixeles
	pixel pixelNuevo;
	for(int i = 0; i < img.height; i++)//columna
	{
		for(int j = 0; j < img.width; j++)//fila
		{	
			
			pixelNuevo = img.getPixel(i, j);

			int resultadoAltura = (j + (j * (n - 1)));
			int resultadoAncho = (i + (i * (n - 1)));
			
			//Recorro la nueva foto img_out y le aplico el zoom
			for(int k = 0; k < n; k++)
			{	
				for(int h = 0; h < n; h++)
				{				
					img_out.setPixel(resultadoAncho + k, resultadoAltura + h, pixelNuevo);
				}

			}
		}		
	}

	img = img_out;

}

void blackWhite(ppm& img, unsigned int start, unsigned int end)
{
	
	for(int i = start; i < end; i++)
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

void blackWhiteMultiThread(ppm& img, int n_threads)
{

	int rows_for_thread = (int)(img.height / n_threads);
	vector<thread> threads_result;

	for (int i = 0; i < n_threads; i++)
	{
		// Calculo el principio y el final
		int start = i * rows_for_thread;
		int end = (i + 1) * rows_for_thread;

		threads_result.push_back(thread(blackWhite, ref(img), start, end));
	}

	for (int i = 0; i < n_threads; i++)
	{
        threads_result[i].join();
	}
}

void contrast(ppm& img, float contrast, unsigned int start, unsigned int end )
{
	
	for(int i = start; i < end; i++)
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

void contrastMultiThread(ppm& img, int n_threads, float contrasts)
{

	int rows_for_thread = (int)(img.height / n_threads);
	vector<thread> threads_result;

	for (int i = 0; i < n_threads; i++)
	{
		// Calculo el principio y el final
		int start = i * rows_for_thread;
		int end = (i + 1) * rows_for_thread;

		threads_result.push_back(thread(contrast, ref(img), contrasts, start, end));
	}

	for (int i = 0; i < n_threads; i++)
	{
        threads_result[i].join();
	}
}

void boxBlur(ppm &img, unsigned int start, unsigned int end)
{
    double promedioRojo = 0, promedioVerde = 0, promedioAzul = 0;
    int contador = 0;
    int box = 1;

    for(int i = start; i < end; i++)//columna
    {
        for(int j = 0; j < img.width; j++)//fila
        {
            promedioRojo = 0;
            promedioVerde = 0;
            promedioAzul = 0;
            contador = 0;

            for(int li = i - box; li <=    i + box; li++) //Creo la caja 
            {
                if(li < 0)
                {
                    li = 0;
                }

                if(li == img.height)
                {
                    break;
                }

                for(int col = j - box; col <= j + box; col++)
                {
                    if(col == img.width) break;
                    if (col < 0)
                    {
                        col = 0;
                    }
                    contador++;

                    promedioRojo = promedioRojo + img.getPixel(li,col).r;
                    promedioVerde = promedioVerde + img.getPixel(li,col).g;
                    promedioAzul = promedioAzul + img.getPixel(li,col).b;

					

                }
            }

            int rFinally = promedioRojo/contador;
			int gFinally = promedioVerde/contador;
			int bFinally = promedioAzul/contador; 

			truncate(rFinally);
			truncate(gFinally);
			truncate(bFinally);

			img.setPixel(i,j, pixel(rFinally,gFinally,bFinally));


        }

    }

}

void boxBlurMultiThread(ppm& img, int n_threads)
{

	int rows_for_thread = (int)(img.height / n_threads);
	vector<thread> threads_result;

	for (int i = 0; i < n_threads; i++)
	{
		// Calculo el principio y el final
		int start = i * rows_for_thread;
		int end = (i + 1) * rows_for_thread;

		threads_result.push_back(thread(boxBlur, ref(img), start, end));
	}

	for (int i = 0; i < n_threads; i++)
	{
        threads_result[i].join();
	}
}

void frame(ppm& img, int x, unsigned int start, unsigned int end)
{
	int fAnteultimas = img.width - x;
	int cAnteultimas = img.height - x;

	for(int i = start; i < end; i++)//columna
	{
		for(int j = 0; j < img.width; j++)//fila
		{	
			if(j<=x || j >= fAnteultimas ) //agarra las primeras x filas y las ultimas x filas
			{
				img.setPixel(i,j,pixel(0,0,0));
			}

			if(i<=x || i >= cAnteultimas ) //agarra las primeras x columnas y las ultimas x columnas
			{
				img.setPixel(i,j,pixel(0,0,0));
			}
		}
					
			
	}
}

void frameMultiThread(ppm& img, int n_threads, int x)
{

	int rows_for_thread = (int)(img.height / n_threads);
	vector<thread> threads_result;

	for (int i = 0; i < n_threads; i++)
	{
		// Calculo el principio y el final
		int start = i * rows_for_thread;
		int end = (i + 1) * rows_for_thread;

		threads_result.push_back(thread(frame, ref(img), x, start, end));
	}

	for (int i = 0; i < n_threads; i++)
	{
        threads_result[i].join();
	}
}