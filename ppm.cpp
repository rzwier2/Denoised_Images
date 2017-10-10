#include <cassert>
#include <string>
#include <iostream>
#include <fstream> //read and write from and to files
#include "ppm.hpp" //Prototypes
using namespace std;

// Default constructor
// This represents an empty image.
PpmImage::PpmImage() {
    width = 0;
    height = 0;
    data = NULL; // NULL means an invalid pointer. Because there's no data!
}

// Construct a blank image of a given size.
// Initialize the image to all zeros.
PpmImage::PpmImage(int _width, int _height) {
    width = _width;
    height = _height;
    data = new int[width * height * 3];

    for (int i = 0; i < width * height * 3; ++i)
        data[i] = 0;
}

// File-based Constructor. Load a PPM image from a file.
// If something goes wrong, give an error message, and return the same thing
// as if we called the default constructor.
PpmImage::PpmImage(const std::string& filename) {

    // These are default values, in case the header is invalid and we exit early.
    width = 0;
    height = 0;
    data = NULL;

    // Parse the header.
    // First: open the file
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cerr << "The image file could not be found:" << filename << std::endl;
        return;
    }
    // Next: the special letters "P3"
    // If those aren't the first thing in the file, then this isn't a version-3
    // PPM image file.
    std::string token;
    fin >> token;
    if (token != "P3") {
        std::cerr << "File not recognized as ppm format." << std::endl;
        return;
    }
    // Third: image metadata: width, height, maxval
    // (We'll assume that maxval is 255. If it's bigger, then abort.)
    fin >> token;
    width = std::stoi(token);
    fin >> token;
    height = std::stoi(token);
    fin >> token;
    int maxval = std::stoi(token);
    if (maxval > 255) {
        std::cerr << "Error: This ppm file can't be read with this code." << std::endl;
    }

    // Okay. We successfully read the header.
    // Allocate the memory we'll need to hold the image.
    data = new int[width * height * 3];

    // Now read in the image, value by value (this is the bulk of the file)
    for (int i = 0; i < width * height * 3; ++i) {
        fin >> data[i];
    }

    // All done!
}

// Destructor: release any dynamic memory
PpmImage::~PpmImage() {
    if (data != NULL)
        delete[] data;
}

// Save this image to a PPM file.
// Reminder: a version-3 PPM file has this format:
/*
P3
width height
255
red green blue
red green blue
red green blue
...
*/
// The pixels are in row-major order. That is, the first row of pixels comes
// first (from left to right), and then the second row, and so on.
void PpmImage::writeToFile(const std::string& filename) {

    std::ofstream wfilename(filename); // use ofstream
        wfilename << "P3\n";
        wfilename << width << " " << height << "\n";
        wfilename << "255\n";

        for (int i = 0; i < height; ++i) { // rows
	    for (int j = 0; j < width; ++j) { // coulumns
	        for (int k = 0; k < 3; ++k) { // each part of the row
		    wfilename << getPixel(i, j, k);
		        if (k == 2) 
			    wfilename << "\n"; // newline after blue
		        else 
			    wfilename << " "; // space between red-green and green-blue
		}
	    }
	}
	wfilename.close(); // close the file
}	

// Access the value of a particular pixel
int PpmImage::getPixel(int row, int col, int channel) const {
    return data[getIndex(row, col, channel)];
}

// Set the value of a particular pixel
void PpmImage::setPixel(int row, int col, int channel, int value) {
    data[getIndex(row, col, channel)] = value;
}

// Figure out where a particular pixel lives in memory.
// Images are of size (width, height, 3), but our memory is just a row of
// width * height * 3 buckets. This function expresses our data layout.
// For example:
//   data[0] is the red value of the upper-left pixel
//   data[width * 3 - 1] is the blue value of the upper-right pixel
//   data[width * 3 * (height - 1)] is the red value of the lower-right pixel.
int PpmImage::getIndex(int row, int col, int channel) const {
    int index = channel +
                getChannels() * col +
                getChannels() * getWidth() * row;
    return index;
}
