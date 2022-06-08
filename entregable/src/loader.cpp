#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>      // std::ofstream
#include <filesystem>
#include "filters.h"
#define ONE_OVER_BILLION 1E-9
using namespace std;
using namespace filesystem;

int main(int argc , char* argv[]){

	if(string(argv[1]) == "--help"){
		cout << "Uso: ./loader <filter> <n_threads> <p1> <root_dir> " << endl;
		return 0; 
	}

	string filter = string(argv[1]);
	float p1 = atof(argv[3]);
	unsigned int n = atoi(argv[2]);
	string dir = string(argv[4]); //direccion de la carpeta
	path out_dir = path(dir);
	bool single_thread = n == 1;

	cout << "Aplicando filtros"<< endl;
	struct timespec start, stop;    	
	clock_gettime(CLOCK_REALTIME, &start);

    for (const auto & image : directory_iterator(dir))
    {
		auto img_path = image.path();
		if (img_path.extension() != ".ppm") continue;

		

        ppm img(img_path);
		if (filter == "plain")
		plain(img, (unsigned char)p1);
		
		else if(filter == "blackWhite")
		{
			if (single_thread)
				blackWhite(img, 0, img.height);
			else
				blackWhiteMultiThread(img, n);
		}

		else if(filter == "contrast")
		{	
			if (single_thread)
				contrast(img,(unsigned char)p1, 0, img.height);
			else
				contrastMultiThread(img, n, (unsigned char)p1);
		}

		else if (filter == "frame")
		{
			if (single_thread)
				frame(img,(unsigned char)p1, 0, img.height);
			else
				frameMultiThread(img, n, (unsigned char)p1);
		}

		else if (filter == "merge")
		{
			string img2(argv[5]);
			ppm img3(img2);
			merge(img,img3,p1);
		}

		else if (filter == "boxBlur")
		{
			if (single_thread)
				boxBlur(img, 0, img.height);
			else
				boxBlurMultiThread(img, n);
		}

		else if (filter == "zoom")
		{
			ppm img_orig(img_path);
			zoom(img,img_orig,p1);
		}	

		path out_path = out_dir; 

		//Le agregas al final del nombre del archivo el nombre del filtro.
		out_path /= string(img_path.stem()) + "_" + filter + ".ppm";
		img.write(out_path);	
	}	
	clock_gettime(CLOCK_REALTIME, &stop);

	double accum;
	accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
	printf("%lf s\n", accum);

	cout << "listo" << endl;
	return 0;
}