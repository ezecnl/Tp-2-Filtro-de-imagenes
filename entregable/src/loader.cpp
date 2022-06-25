#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <thread>
#include <unistd.h>
#include <fstream>      // std::ofstream
#include <filesystem>
#include "filters.h"
#define ONE_OVER_BILLION 1E-9
using namespace std;
using namespace filesystem;


//No aplicamos el filtro zoom Digital y el Merge
void aplicarFfiltros(path directorio_salida, string filter ,int p1, vector<path> imagenes )
{
	for (path img_path : imagenes) 
	{
		ppm img(img_path);
		if (filter == "plain")
		{
			plain(img, (unsigned char)p1);
		}
		
		else if(filter == "blackWhite")
		{
			blackWhite(img, 0, img.height);
		}

		else if(filter == "contrast")
		{	
			contrast(img,(unsigned char)p1, 0, img.height);	
		}

		else if (filter == "frame")
		{
			frame(img,(unsigned char)p1, 0, img.height);
		}
		else if (filter == "boxBlur")
		{
			boxBlur(img, 0, img.height);
		}

		
		path out_path = directorio_salida; 
		

		//Le agregas al final del nombre del archivo el nombre del filtro.
		out_path /= string(img_path.stem()) + "_" + filter + ".ppm";
		img.write(out_path);
	}
}


int main(int argc , char* argv[])
{
	if(string(argv[1]) == "--help"){
		cout << "Uso: ./loader <filter> <n_threads> <p1> <dir> <dir_out> " << endl;
		return 0; 
	}

	string filters = string(argv[1]);
	unsigned int n_readers = atoi(argv[2]);
	float p1 = atof(argv[3]);
	string dir = string(argv[4]); //direccion de la carpeta
	string dir_salida = string (argv[5]);
	path output = path(dir_salida);
	

	struct timespec start, stop;    	
	clock_gettime(CLOCK_REALTIME, &start);

	path out_dir = path(output); 
	// Crea un directorio si no esta 
	

	// Almacena en los vectores todas las direcciones de las imagenes
	vector<path> paths;
	for (const auto & image : directory_iterator(dir)){
		auto img_path = image.path();
		if (img_path.extension() != ".ppm") continue;
		paths.push_back(img_path);
	}
	// Ordena las imagenes de mayor al menor por tamaño
    for (int i = 0; i < paths.size() - 1; i++){
        for (int j = 0; j < paths.size() - i - 1; j++){
            if (file_size(paths[j]) < file_size(paths[j + 1])) {
            	swap(paths[j], paths[j + 1]);       
            }
        }
    }
    // Se destribuyen las imagenes en los threads
    int img_per_reader = paths.size() / n_readers;
    int remainder = paths.size() - (n_readers * img_per_reader);
    vector<vector<path>> threads_images;
    for (int i = 0; i < n_readers; i++)
	{
    	vector<path> thread_images;
    	for (int j = 0; j < img_per_reader; j++){
    		thread_images.push_back(paths[i + j*n_readers]);
    	}
    	threads_images.push_back(thread_images);
    }
    paths.erase(paths.begin(), paths.begin() + n_readers * img_per_reader);
    
	
    // Se añaden el resto de las imagenes al los threads 
    for (int i = n_readers-1; i > 0; i--)
	{
    	if (paths.empty()) break;
    	threads_images[i].push_back(paths[0]);
    	paths.erase(paths.begin());
    }
	
	
	thread threads[n_readers];
    for (int i = 0; i < n_readers; i++)
	{
		threads[i] = thread(aplicarFfiltros,out_dir,filters,p1, threads_images[i]);
	} 
    for (int i = 0; i < n_readers; i++)
	{
		threads[i].join();
	} 

    clock_gettime(CLOCK_REALTIME, &stop);
	double total_time;
	total_time = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
	
	printf("Total Time: %lf s\n", total_time);
	return total_time;
}






