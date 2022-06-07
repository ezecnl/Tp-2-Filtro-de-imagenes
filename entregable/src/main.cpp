#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include <thread>
#include <unistd.h>
#include <fstream>      // std::ofstream
#include "ppm.h"


#define ONE_OVER_BILLION 1E-9

using namespace std;

// El siguiente es un template basico que pueden usar como base

int main(int argc , char* argv[]){
	
	// Asumimos que los filtros sin p1 se escriben primero (por lo tanto, el primer p1 es no nulo)
	// Asumimos que Zoom no se puede encadenar

	if(string(argv[1]) == "-help"){
		cout << "Uso: ./main <filtro> <nthreads> <[p1]> <img1> <custom_output> <[p2]> <img2>" << endl;
		return 0; 
	}
	
	string filter = string(argv[1]);
	unsigned int n = atoi(argv[2]);
	bool single_thread = n == 1;
	float p1 = atof(argv[3]);
	string img1(argv[4]);
	string out = string(argv[5]);
	
	ppm img(img1);
	
	
	cout << "Aplicando filtros"<< endl;
	struct timespec start, stop;    	
	clock_gettime(CLOCK_REALTIME, &start);

	if (filter == "plain")
		plain(img, (unsigned char)p1);
	else if(filter == "blackWhite")
		if (single_thread)
			blackWhite(img, 0, img.height);
		else
			blackWhiteMultiThread(img, n);
	else if(filter == "contrast")
		if (single_thread)
			contrast(img,(unsigned char)p1, 0, img.height);
		else
			contrastMultiThread(img, n);
	else if (filter == "frame")
		if (single_thread)
			frame(img,(unsigned char)p1, 0, img.height);
		else
			frameMultiThread(img, n);
	else if (filter == "merge")
	{
		string img2(argv[6]);
		ppm img3(img2);
		merge(img,img3,p1);
	}
	else if (filter == "boxBlur")
		if (single_thread)
			boxBlur(img, 0, img.height);
		else
			boxBlurMultiThread(img, n);
	else if (filter == "zoom")
	{
		ppm img_orig(img1);
		zoom(img,img_orig,p1);
	}
		
		
   	clock_gettime(CLOCK_REALTIME, &stop);

	double accum;
	accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
	printf("%lf s\n", accum);

	cout << "Escribiendo imagen" << endl;
	img.write(out);	
	    
	cout << "Listo" << endl;
	return 0;
}