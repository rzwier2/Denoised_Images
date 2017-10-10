#include "ppm.hpp"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char * argv[]) {

    string noisyImageList(argv[1]); // Calls the 10 images -- first command line argument
    string outputFile(argv[2]); // output text file in .ppm format -- second command line argument

    ifstream fin(noisyImageList);
    if(!fin) {
        cout << "file not found" << endl;
    }
    string noisyName;
    fin >> noisyName;
	
    PpmImage denoised_image(noisyName); // The ppm image that we write to file. This is the base file
    int a = 1;	

    for (int i = 0; i < 10; ++i) {
       fin >> noisyName;
       cout << noisyName << endl; // print out the noisy image text file, and print out the names of the noisy images
       PpmImage ryans_image(noisyName); //Open up image
        for(int j = 0; j < ryans_image.getHeight(); ++j) { // add up all the images pixel by pixel
            for(int k = 0; k < ryans_image.getWidth(); ++k) {
                for(int z = 0; z < 3; ++z) {
                    int currentValue = denoised_image.getPixel(j, k, z); 
                    int nextValue = ryans_image.getPixel(j, k, z);
                    denoised_image.setPixel(j, k, z, currentValue + nextValue);
                }
            }
        }
    }

    for (int j = 0; j < denoised_image.getHeight(); ++j) { // divide by the number of noisy images
        for (int k = 0; k < denoised_image.getWidth(); ++k) {
            for (int z = 0; z < 3; ++z) {
                int currentValue = denoised_image.getPixel(j, k, z);
                denoised_image.setPixel(j, k, z, currentValue / 10);		
            }
        }
    }

    denoised_image.writeToFile(outputFile); // write out the denoised image

}
