#ifndef imageProc_H__
#define imageProc_H__

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <cstring>

using namespace std;

// Pixel structure
struct Pixel
{
    // Red, green, blue color values
    int red;
    int green;
    int blue;
};

/**
 * Gets an integer from a binary stream.
 * Helper function for read_image()
 * @param stream the stream
 * @param offset the offset at which to read the integer
 * @param bytes  the number of bytes to read
 * @return the integer starting at the given offset
 */ 
int get_int(fstream& stream, int offset, int bytes);

/**
 * Reads the BMP image specified and returns the resulting image as a vector
 * @param filename BMP image filename
 * @return the image as a vector of vector of Pixels
 */
vector<vector<Pixel>> read_image(string filename);

/**
 * Sets a value to the char array starting at the offset using the size
 * specified by the bytes.
 * This is a helper function for write_image()
 * @param arr    Array to set values for
 * @param offset Starting index offset
 * @param bytes  Number of bytes to set
 * @param value  Value to set
 * @return nothing
 */
void set_bytes(unsigned char arr[], int offset, int bytes, int value);

/**
 * Write the input image to a BMP file name specified
 * @param filename The BMP file name to save the image to
 * @param image    The input image to save
 * @return True if successful and false otherwise
 */
bool write_image(string filename, const vector<vector<Pixel>>& image);

/**
 * Adds vignette effect to image (dark corners)
 * @param image    The input image to process
 * @return processed image
 */
vector<vector<Pixel>> process_1(const vector<vector<Pixel>>& image);

/**
 * Adds clarendon effect to image (darks darker and lights lighter) by a scaling factor
 * @param image    The input image to process
 * @param scaling factor
 * @return processed image
 */
vector<vector<Pixel>> process_2(const vector<vector<Pixel>>& image, double scaling_factor);

/**
 * Adds grayscale effect to image
 * @param image The input image to process
 * @param scaling factor
 * @return processed image
 */
vector<vector<Pixel>> process_3(const vector<vector<Pixel>>& image);

/**
 * Rotates image by 90 degrees clockwise
 * @param image The input image to process
 * @return processed image
 */
vector<vector<Pixel>> process_4(const vector<vector<Pixel>>& image);

/**
 * Helper function for process 5 that creates a new image and rotates image in 90 degree increments
 * @param image The input image to process
 * @return processed image
 */
vector<vector<Pixel>> rotate_by_90(const vector<vector<Pixel>>& image);

/**
 * Rotates image by 90 degree increments clockwise
 * @param image The input image to process
 * @param number of rotations
 * @return processed image
 */
vector<vector<Pixel>> process_5(const vector<vector<Pixel>>& image, int number);

/**
 * Enlarges the image in the x and y direction
 * @param image The input image to process
 * @param x scale
 * @param y scale
 * @return processed image
 */
vector<vector<Pixel>> process_6(const vector<vector<Pixel>>& image, int x_scale, int y_scale);

/**
 * Convert image to high contrast (black and white only)
 * @param image The input image to process
 * @return processed image
 */
vector<vector<Pixel>> process_7(const vector<vector<Pixel>>& image);


/**
 * Lightens image by a scaling factor
 * @param image The input image to process
 * @param scaling factor
 * @return processed image
 */
vector<vector<Pixel>> process_8(const vector<vector<Pixel>>& image, double scaling_factor);

/**
 * Darkens image by a scaling factor
 * @param image The input image to process
 * @param scaling factor
 * @return processed image
 */
vector<vector<Pixel>> process_9(const vector<vector<Pixel>>& image, double scaling_factor);

/**
 * Converts image to only black, white, red, blue, and green
 * @param image The input image to process
 * @return processed image
 */
vector<vector<Pixel>> process_10(const vector<vector<Pixel>>& image);

#endif // imageProc_H__