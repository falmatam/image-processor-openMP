#include <omp.h>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
 
/* Global variables, Look at their usage in main() */
int image_height;
int image_width;
int image_maxShades;
int inputImage[1000][1000];
int outputImage[1000][1000];
int chunkSize;

#define NUM_THREADS 2


/* ****************Change and add functions below ***************** */
void compute_sobel_static()
{

	//Static scheduling divides iterations evenly between threads
	//• Default chunk size is num_Iterations/num_Threads 

	omp_set_dynamic(0);
	omp_set_num_threads(NUM_THREADS);

	int curThread = omp_get_thread_num();


	int x, y, sum, sumx, sumy;
	int GX[3][3], GY[3][3];
	/* 3x3 Sobel masks. */
	GX[0][0] = -1; GX[0][1] = 0; GX[0][2] = 1;
	GX[1][0] = -2; GX[1][1] = 0; GX[1][2] = 2;
	GX[2][0] = -1; GX[2][1] = 0; GX[2][2] = 1;

	GY[0][0] = 1; GY[0][1] = 2; GY[0][2] = 1;
	GY[1][0] = 0; GY[1][1] = 0; GY[1][2] = 0;
	GY[2][0] = -1; GY[2][1] = -2; GY[2][2] = -1;


    #pragma omp parallel for schedule(static, chunkSize)
	for (x = 0; x < image_height; x++) {
		for (y = 0; y < image_width; y++) {
			sumx = 0;
			sumy = 0;
			
			
			//Change boundary cases as required
			if (x == 0 || x == (image_height - 1) || y == 0 || y == (image_width - 1)) {
				sum = 0;
			}

			else {
				for (int i = -1; i <= 1; i++) {

					for (int j = -1; j <= 1; j++) {
						sumx += (inputImage[x + i][y + j] * GX[i + 1][j + 1]);
					}
				}
				/* Gradient calculation in Y Dimension */
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						sumy += (inputImage[x + i][y + j] * GY[i + 1][j + 1]);
					}
				}
				/* Gradient magnitude */
				sum = (abs(sumx) + abs(sumy));

				if (sum < 0) {
					sum = 0;
				}
				else if (sum > 255) {
					sum = 255;
				}
			}

			outputImage[x][y] = sum;
			
		 }	
	}
#pragma omp parallel num_threads(NUM_THREADS)
{	for (int i = 0; i < image_height; i++) {
        std::cout << "Thread -> " << omp_get_thread_num() << " processing chunk starting at row " << i << "\n";
}
 }
   
}



void compute_sobel_dynamic()
{

//• Dynamic scheduling uses an internal work queue to
//dispatch chunkSized blocks to each thread. When a
//thread finishes a block, it gets the next block
//• Default chunk size is 1
//• Think Part B Lab 1

	omp_set_dynamic(0);
	omp_set_num_threads(NUM_THREADS);

	int curThread = omp_get_thread_num();


	int x, y, sum, sumx, sumy;
	int GX[3][3], GY[3][3];
	/* 3x3 Sobel masks. */
	GX[0][0] = -1; GX[0][1] = 0; GX[0][2] = 1;
	GX[1][0] = -2; GX[1][1] = 0; GX[1][2] = 2;
	GX[2][0] = -1; GX[2][1] = 0; GX[2][2] = 1;

	GY[0][0] = 1; GY[0][1] = 2; GY[0][2] = 1;
	GY[1][0] = 0; GY[1][1] = 0; GY[1][2] = 0;
	GY[2][0] = -1; GY[2][1] = -2; GY[2][2] = -1;

	
    #pragma omp for schedule(dynamic, chunkSize)
	for (x = 0; x < image_height; x++) {
		for (y = 0; y < image_width; y++) {
			sumx = 0;
			sumy = 0;
			
			if (x == 0 || x == (image_height - 1) || y == 0 || y == (image_width - 1)) {
				sum = 0;
			}

			else {
				for (int i = -1; i <= 1; i++) {

					for (int j = -1; j <= 1; j++) {
						sumx += (inputImage[x + i][y + j] * GX[i + 1][j + 1]);
					}
				}
				/* Gradient calculation in Y Dimension */
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						sumy += (inputImage[x + i][y + j] * GY[i + 1][j + 1]);
					}
				}
				/* Gradient magnitude */
				sum = (abs(sumx) + abs(sumy));

				if (sum < 0) {
					sum = 0;
				}
				else if (sum > 255) {
					sum = 255;
				}
			}

			outputImage[x][y] = sum;	
		 }
         		 
	}
#pragma omp parallel num_threads(NUM_THREADS)
{	for (int i = 0; i < image_height; i++) {
        std::cout << "Thread -> " << omp_get_thread_num() << " processing chunk starting at row " << i << "\n";
}
 }
}


int main(int argc, char* argv[])
{
    
    
    
    if(argc != 5)
    {
        std::cout << "ERROR: Incorrect number of arguments. Format is: <Input image filename> <Output image filename> <Chunk size> <a1/a2>" << std::endl;
        return 0;
    }
 
    std::ifstream file(argv[1]);
    if(!file.is_open())
    {
        std::cout << "ERROR: Could not open file " << argv[1] << std::endl;
        return 0;
    }
    chunkSize  = std::atoi(argv[3]);

    std::cout << "Detect edges in " << argv[1] << " using OpenMP threads\n" << std::endl;

    /* ******Reading image into 2-D array below******** */

    std::string workString;
    /* Remove comments '#' and check image format */ 
    while(std::getline(file,workString))
    {
        if( workString.at(0) != '#' ){
            if( workString.at(1) != '2' ){
                std::cout << "Input image is not a valid PGM image" << std::endl;
                return 0;
            } else {
                break;
            }       
        } else {
            continue;
        }
    }
    /* Check image size */ 
    while(std::getline(file,workString))
    {
        if( workString.at(0) != '#' ){
            std::stringstream stream(workString);
            int n;
            stream >> n;
            image_width = n;
            stream >> n;
            image_height = n;
            break;
        } else {
            continue;
        }
    }

    /* Check image max shades */ 
    while(std::getline(file,workString))
    {
        if( workString.at(0) != '#' ){
            std::stringstream stream(workString);
            stream >> image_maxShades;
            break;
        } else {
            continue;
        }
    }
    /* Fill input image matrix */ 
    int pixel_val;
    for( int i = 0; i < image_height; i++ )
    {
        if( std::getline(file,workString) && workString.at(0) != '#' ){
            std::stringstream stream(workString);
            for( int j = 0; j < image_width; j++ ){
                if( !stream )
                    break;
                stream >> pixel_val;
                inputImage[i][j] = pixel_val;
            }
        } else {
            continue;
        }
    }

    /************ Call functions to process image *********/
    std::string opt = argv[4];
    if( !opt.compare("a1") )
    {    
        double dtime_static = omp_get_wtime();
        compute_sobel_static();
        dtime_static = omp_get_wtime() - dtime_static;
    } else {
        double dtime_dyn = omp_get_wtime();
        compute_sobel_dynamic();
        dtime_dyn = omp_get_wtime() - dtime_dyn;
        std::cout << "dtime_dyn: " << dtime_dyn << std::endl;
        
    }

    /* ********Start writing output to your file************ */
    std::ofstream ofile(argv[2]);
    if( ofile.is_open() )
    {
        ofile << "P2" << "\n" << image_width << " " << image_height << "\n" << image_maxShades << "\n";
        for( int i = 0; i < image_height; i++ )
        {
            for( int j = 0; j < image_width; j++ ){
                ofile << outputImage[i][j] << " ";
            }
            ofile << "\n";
        }
    } else {
        std::cout << "ERROR: Could not open output file " << argv[2] << std::endl;
        return 0;
    }
    return 0;
}
