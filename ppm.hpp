#ifndef __PPM_HPP
#define __PPM_HPP

class PpmImage {
public:
    PpmImage(); // default constructor
    PpmImage(int width, int height); // Creates a blank image
    PpmImage(const std::string& filename); // opens a ppm file

    ~PpmImage(); // destructor: release all dynamic memory

    // write this image's data to a ppm file
    // assume that all image contents are between 0 and 255
    void writeToFile(const std::string& filename);

    // accessor methods for the image dimensions
    // ppm is a 3-channel color-only format. There are always exactly three
    // channels (red, green, blue).
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return 3; }

    // functions to access and change the pixel data of the image
    int getPixel(int row, int col, int channel) const;
    void setPixel(int row, int col, int channel, int value);

private:
    // private data: the dimensions of the image, and an array of
    // the raw pixel information. Storage order is channel first,
    // then by row, and finally by column.
    int width;
    int height;
    int * data;

    // convert from row/column/channel to an index in memory
    int getIndex(int row, int col, int channel) const;
};

#endif
