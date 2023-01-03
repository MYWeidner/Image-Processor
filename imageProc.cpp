#include "imageProc.h"

//***************************************************************************************************//
//               Functions through write_image provided by Instructor (CU Boulder)                   //
//***************************************************************************************************//

int get_int(fstream& stream, int offset, int bytes)
{
    stream.seekg(offset);
    int result = 0;
    int base = 1;
    for (int i = 0; i < bytes; i++)
    {   
        result = result + stream.get() * base;
        base = base * 256;
    }
    return result;
}

vector<vector<Pixel>> read_image(string filename)
{
    // Open the binary file
    fstream stream;
    stream.open(filename, ios::in | ios::binary);

    // Get the image properties
    int file_size = get_int(stream, 2, 4);
    int start = get_int(stream, 10, 4);
    int width = get_int(stream, 18, 4);
    int height = get_int(stream, 22, 4);
    int bits_per_pixel = get_int(stream, 28, 2);

    // Scan lines must occupy multiples of four bytes
    int scanline_size = width * (bits_per_pixel / 8);
    int padding = 0;
    if (scanline_size % 4 != 0)
    {
        padding = 4 - scanline_size % 4;
    }

    // Return empty vector if this is not a valid image
    if (file_size != start + (scanline_size + padding) * height)
    {
        return {};
    }

    // Create a vector the size of the input image
    vector<vector<Pixel>> image(height, vector<Pixel> (width));

    int pos = start;
    // For each row, starting from the last row to the first
    // Note: BMP files store pixels from bottom to top
    for (int i = height - 1; i >= 0; i--)
    {
        // For each column
        for (int j = 0; j < width; j++)
        {
            // Go to the pixel position
            stream.seekg(pos);

            // Save the pixel values to the image vector
            // Note: BMP files store pixels in blue, green, red order
            image[i][j].blue = stream.get();
            image[i][j].green = stream.get();
            image[i][j].red = stream.get();

            // We are ignoring the alpha channel if there is one

            // Advance the position to the next pixel
            pos = pos + (bits_per_pixel / 8);
        }

        // Skip the padding at the end of each row
        stream.seekg(padding, ios::cur);
        pos = pos + padding;
    }

    // Close the stream and return the image vector
    stream.close();
    return image;
}

void set_bytes(unsigned char arr[], int offset, int bytes, int value)
{
    for (int i = 0; i < bytes; i++)
    {
        arr[offset+i] = (unsigned char)(value>>(i*8));
    }
}

bool write_image(string filename, const vector<vector<Pixel>>& image)
{
    // Get the image width and height in pixels
    int width_pixels = image[0].size();
    int height_pixels = image.size();

    // Calculate the width in bytes incorporating padding (4 byte alignment)
    int width_bytes = width_pixels * 3;
    int padding_bytes = 0;
    padding_bytes = (4 - width_bytes % 4) % 4;
    width_bytes = width_bytes + padding_bytes;

    // Pixel array size in bytes, including padding
    int array_bytes = width_bytes * height_pixels;

    // Open a file stream for writing to a binary file
    fstream stream;
    stream.open(filename, ios::out | ios::binary);

    // If there was a problem opening the file, return false
    if (!stream.is_open())
    {
        return false;
    }

    // Create the BMP and DIB Headers
    const int BMP_HEADER_SIZE = 14;
    const int DIB_HEADER_SIZE = 40;
    unsigned char bmp_header[BMP_HEADER_SIZE] = {0};
    unsigned char dib_header[DIB_HEADER_SIZE] = {0};

    // BMP Header
    set_bytes(bmp_header,  0, 1, 'B');              // ID field
    set_bytes(bmp_header,  1, 1, 'M');              // ID field
    set_bytes(bmp_header,  2, 4, BMP_HEADER_SIZE+DIB_HEADER_SIZE+array_bytes); // Size of BMP file
    set_bytes(bmp_header,  6, 2, 0);                // Reserved
    set_bytes(bmp_header,  8, 2, 0);                // Reserved
    set_bytes(bmp_header, 10, 4, BMP_HEADER_SIZE+DIB_HEADER_SIZE); // Pixel array offset

    // DIB Header
    set_bytes(dib_header,  0, 4, DIB_HEADER_SIZE);  // DIB header size
    set_bytes(dib_header,  4, 4, width_pixels);     // Width of bitmap in pixels
    set_bytes(dib_header,  8, 4, height_pixels);    // Height of bitmap in pixels
    set_bytes(dib_header, 12, 2, 1);                // Number of color planes
    set_bytes(dib_header, 14, 2, 24);               // Number of bits per pixel
    set_bytes(dib_header, 16, 4, 0);                // Compression method (0=BI_RGB)
    set_bytes(dib_header, 20, 4, array_bytes);      // Size of raw bitmap data (including padding)                     
    set_bytes(dib_header, 24, 4, 2835);             // Print resolution of image (2835 pixels/meter)
    set_bytes(dib_header, 28, 4, 2835);             // Print resolution of image (2835 pixels/meter)
    set_bytes(dib_header, 32, 4, 0);                // Number of colors in palette
    set_bytes(dib_header, 36, 4, 0);                // Number of important colors

    // Write the BMP and DIB Headers to the file
    stream.write((char*)bmp_header, sizeof(bmp_header));
    stream.write((char*)dib_header, sizeof(dib_header));

    // Initialize pixel and padding
    unsigned char pixel[3] = {0};
    unsigned char padding[3] = {0};

    // Pixel Array (Left to right, bottom to top, with padding)
    for (int h = height_pixels - 1; h >= 0; h--)
    {
        for (int w = 0; w < width_pixels; w++)
        {
            // Write the pixel (Blue, Green, Red)
            pixel[0] = image[h][w].blue;
            pixel[1] = image[h][w].green;
            pixel[2] = image[h][w].red;
            stream.write((char*)pixel, 3);
        }
        // Write the padding bytes
        stream.write((char *)padding, padding_bytes);
    }

    // Close the stream and return true
    stream.close();
    return true;
}

//***************************************************************************************************//
//                               Image Processing Functions                                          //
//***************************************************************************************************//

vector<vector<Pixel>> process_1(const vector<vector<Pixel>>& image)
{
    int num_rows = image.size(); // Gets the number of rows (height) in a 2D vector named image
    int num_columns = image[0].size(); // // Gets the number of columns (width) in a 2D vector named image
    vector<vector<Pixel>> result(num_rows, vector<Pixel> (num_columns)); // define new 2D vector of Pixel values and prepopulate it with a specified number of rows and columns
    for(int rows = 0; rows < num_rows; rows++)
    {
        for(int columns = 0; columns < num_columns; columns++)
        {
            // get pixel for color located at index row, col in the input 2D vector
            int red = image[rows][columns].red;
            int green = image[rows][columns].green;
            int blue = image[rows][columns].blue;            
            // find the distance to the center
            double distance = sqrt(((pow(((columns - num_columns)/2), 2))) + ((pow(((rows - num_rows)/2), 2))));
            double scaling_factor = ((num_rows - distance)/num_rows); 
            // set pixel to new color value
            result[rows][columns].red = red * scaling_factor;
            result[rows][columns].green = green * scaling_factor;
            result[rows][columns].blue = blue * scaling_factor;
        }
    }
    return result;
}

vector<vector<Pixel>> process_2(const vector<vector<Pixel>>& image, double scaling_factor)
{
    int num_rows = image.size(); // Gets the number of rows (height) in a 2D vector named image
    int num_columns = image[0].size(); // // Gets the number of columns (width) in a 2D vector named image
    vector<vector<Pixel>> result(num_rows, vector<Pixel> (num_columns)); // define new 2D vector of Pixel values and prepopulate it with a specified number of rows and columns
    for(int rows = 0; rows < num_rows; rows++)
    {
        for(int columns = 0; columns < num_columns; columns++)
        {
            // get pixel for color located at index row, col in the input 2D vector
            int red = image[rows][columns].red;
            int green = image[rows][columns].green;
            int blue = image[rows][columns].blue;
            const double COUNT_COLORS = 3; // constant variable for RGB
            double avg_color_value = (red + green + blue)/COUNT_COLORS; // average color values
            if (avg_color_value >= 170)
            {
                // set pixel to new color value
                result[rows][columns].red = (255 - (255 - red) * scaling_factor);
                result[rows][columns].green = (255 - (255 - green) * scaling_factor);
                result[rows][columns].blue = (255 - (255 - blue) * scaling_factor);
            }
            else if (avg_color_value < 90)
            {
                // set pixel to new color value
                result[rows][columns].red = red * scaling_factor;
                result[rows][columns].green = green * scaling_factor;
                result[rows][columns].blue = blue * scaling_factor; 
            }
            else
            {
                // set pixel to new color value
                result[rows][columns].red = red;
                result[rows][columns].green = green;
                result[rows][columns].blue = blue;
            }
        }
    }
    return result;
}

vector<vector<Pixel>> process_3(const vector<vector<Pixel>>& image)
{
    int num_rows = image.size(); // Gets the number of rows (height) in a 2D vector named image
    int num_columns = image[0].size(); // // Gets the number of columns (width) in a 2D vector named image
    vector<vector<Pixel>> result(num_rows, vector<Pixel> (num_columns)); // define new 2D vector of Pixel values and prepopulate it with a specified number of rows and columns
    for(int rows = 0; rows < num_rows; rows++)
    {
        for(int columns = 0; columns < num_columns; columns++)
        {
            // get pixel for color located at index row, col in the input 2D vector
            int red = image[rows][columns].red;
            int green = image[rows][columns].green;
            int blue = image[rows][columns].blue;            
            // find average of color values to get gray value
            const double COUNT_COLORS = 3; // constant variable for RGB
            double gray_value = (red + green + blue)/COUNT_COLORS; // average color values 
            // set pixel to grey color value
            result[rows][columns].red = gray_value;
            result[rows][columns].green = gray_value;
            result[rows][columns].blue = gray_value;
        }
    }
    return result;
}

vector<vector<Pixel>> process_4(const vector<vector<Pixel>>& image)
{
    int num_rows = image.size(); // Gets the number of rows (height) in a 2D vector named image
    int num_columns = image[0].size(); // // Gets the number of columns (width) in a 2D vector named image
    vector<vector<Pixel>> result(num_columns, vector<Pixel> (num_rows)); // define new 2D vector of Pixel values and prepopulate it with a specified number of rows and columns; note position of columns and rows has been switched to adjust for 90 degree rotaton
    for(int rows = 0; rows < num_rows; rows++)
    {
        for(int columns = 0; columns < num_columns; columns++)
        {
            // get pixel for color located at index row, col in the input 2D vector
            int red = image[rows][columns].red;
            int green = image[rows][columns].green;
            int blue = image[rows][columns].blue;            
            // rotate image 90 degrees (clockwise)
            result[columns][(num_rows - 1)-rows].red = red;
            result[columns][(num_rows - 1)-rows].green = green;
            result[columns][(num_rows - 1)-rows].blue = blue;
        }
    }
    return result;
}

// Helper function for process 5
vector<vector<Pixel>> rotate_by_90(const vector<vector<Pixel>>& image)
{
    int num_rows = image.size(); // Gets the number of rows (height) in a 2D vector named image
    int num_columns = image[0].size(); // // Gets the number of columns (width) in a 2D vector named image
    vector<vector<Pixel>> result(num_columns, vector<Pixel> (num_rows)); // define new 2D vector of Pixel values and prepopulate it with a specified number of rows and columns; note position of columns and rows has been switched to adjust for 90 degree rotaton
    for(int rows = 0; rows < num_rows; rows++)
    {
        for(int columns = 0; columns < num_columns; columns++)
        {
            // get pixel for color located at index row, col in the input 2D vector
            int red = image[rows][columns].red;
            int green = image[rows][columns].green;
            int blue = image[rows][columns].blue;            
            // rotate image 90 degrees (clockwise)
            result[columns][(num_rows - 1)-rows].red = red;
            result[columns][(num_rows - 1)-rows].green = green;
            result[columns][(num_rows - 1)-rows].blue = blue;
        }
    }
    return result;
}

vector<vector<Pixel>> process_5(const vector<vector<Pixel>>& image, int number)
{
    int const DEGREE = 90; // constant variable for degree of rotation
    int angle = number * DEGREE; // compute angle
    if (angle % DEGREE != 0)
    {
        cout << "Angle must be a multiple of 90 degrees." << endl;
    }
    else if (angle % 360 == 0)
    {
        return image;
    }
    else if (angle % 360 == 90)
    {
        return rotate_by_90(image);
    }
    else if (angle % 360 == 180)
    {
        return rotate_by_90(rotate_by_90(image));
    }
    else
    {
        return rotate_by_90(rotate_by_90(rotate_by_90(image)));
    }
    return image;
}

vector<vector<Pixel>> process_6(const vector<vector<Pixel>>& image, int x_scale, int y_scale)
{
    int num_rows = image.size(); // Gets the number of rows (height) in a 2D vector named image
    int num_columns = image[0].size(); // // Gets the number of columns (width) in a 2D vector named image
    vector<vector<Pixel>> result(num_rows * y_scale, vector<Pixel> (num_columns * x_scale)); // define new 2D vector of Pixel values and prepopulate it with a specified number of rows and columns
    for(int rows = 0; rows < (num_rows * y_scale); rows++)
    {
        for(int columns = 0; columns < (num_columns * x_scale); columns++)
        {
            // get pixel for color located at index row, col in the input 2D vector
            int red = image[rows/y_scale][columns/x_scale].red;
            int green = image[rows/y_scale][columns/x_scale].green;
            int blue = image[rows/y_scale][columns/x_scale].blue; 
            // set pixel to new color value
            result[rows][columns].red = red;
            result[rows][columns].green = green;
            result[rows][columns].blue = blue;
        }
    }
    return result;
}

vector<vector<Pixel>> process_7(const vector<vector<Pixel>>& image)
{
    int num_rows = image.size(); // Gets the number of rows (height) in a 2D vector named image
    int num_columns = image[0].size(); // // Gets the number of columns (width) in a 2D vector named image
    double change_value = 255/2;
    vector<vector<Pixel>> result(num_rows, vector<Pixel> (num_columns)); // define new 2D vector of Pixel values and prepopulate it with a specified number of rows and columns
    for(int rows = 0; rows < num_rows; rows++)
    {
        for(int columns = 0; columns < num_columns; columns++)
        {
            // get pixel for color located at index row, col in the input 2D vector
            int red = image[rows][columns].red;
            int green = image[rows][columns].green;
            int blue = image[rows][columns].blue;            
            // find average of color values to get gray value
            const double COUNT_COLORS = 3; // constant variable for RGB
            double gray_value = (red + green + blue)/COUNT_COLORS; // average color values 
            if (gray_value >= change_value)
            {
                // set pixel to max value for each color
                result[rows][columns].red = 255;
                result[rows][columns].green = 255;
                result[rows][columns].blue = 255;
            }
            else
            {
                // set pixel to min value for each color
                result[rows][columns].red = 0;
                result[rows][columns].green = 0;
                result[rows][columns].blue = 0;
            }
        }
    }
    return result;
}

vector<vector<Pixel>> process_8(const vector<vector<Pixel>>& image, double scaling_factor)
{
    int num_rows = image.size(); // Gets the number of rows (height) in a 2D vector named image
    int num_columns = image[0].size(); // // Gets the number of columns (width) in a 2D vector named image
    vector<vector<Pixel>> result(num_rows, vector<Pixel> (num_columns)); // define new 2D vector of Pixel values and prepopulate it with a specified number of rows and columns
    for(int rows = 0; rows < num_rows; rows++)
    {
        for(int columns = 0; columns < num_columns; columns++)
        {
            // get pixel for color located at index row, col in the input 2D vector
            int red = image[rows][columns].red;
            int green = image[rows][columns].green;
            int blue = image[rows][columns].blue;
            // set pixel to new color value
            result[rows][columns].red = (255 - (255 - red) * scaling_factor);
            result[rows][columns].green = (255 - (255 - green) * scaling_factor);
            result[rows][columns].blue = (255 - (255 - blue) * scaling_factor);
        }
    }
    return result;
}

vector<vector<Pixel>> process_9(const vector<vector<Pixel>>& image, double scaling_factor)
{
    int num_rows = image.size(); // Gets the number of rows (height) in a 2D vector named image
    int num_columns = image[0].size(); // // Gets the number of columns (width) in a 2D vector named image
    vector<vector<Pixel>> result(num_rows, vector<Pixel> (num_columns)); // define new 2D vector of Pixel values and prepopulate it with a specified number of rows and columns
    for(int rows = 0; rows < num_rows; rows++)
    {
        for(int columns = 0; columns < num_columns; columns++)
        {
            // get pixel for color located at index row, col in the input 2D vector
            int red = image[rows][columns].red;
            int green = image[rows][columns].green;
            int blue = image[rows][columns].blue;
            // set pixel to new color value
            result[rows][columns].red = red * scaling_factor;
            result[rows][columns].green = green * scaling_factor;
            result[rows][columns].blue = blue * scaling_factor;
        }
    }
    return result;
}

vector<vector<Pixel>> process_10(const vector<vector<Pixel>>& image)
{
    int num_rows = image.size(); // Gets the number of rows (height) in a 2D vector named image
    int num_columns = image[0].size(); // // Gets the number of columns (width) in a 2D vector named image
    vector<vector<Pixel>> result(num_rows, vector<Pixel> (num_columns)); // define new 2D vector of Pixel values and prepopulate it with a specified number of rows and columns
    for(int rows = 0; rows < num_rows; rows++)
    {
        for(int columns = 0; columns < num_columns; columns++)
        {
            // get pixel for color located at index row, col in the input 2D vector
            int red = image[rows][columns].red;
            int green = image[rows][columns].green;
            int blue = image[rows][columns].blue;
            int max_color = max({red, green, blue}); // calculate max color value
            int tot_color = red + green + blue; // calculate total color value
            if (tot_color >= 550)
            {
                // set pixel to new color value at max value per color
                result[rows][columns].red = 255;
                result[rows][columns].green = 255;
                result[rows][columns].blue = 255;
            }
            else if (tot_color <= 150)
            {
                // set pixel to new color value at min value per color
                result[rows][columns].red = 0;
                result[rows][columns].green = 0;
                result[rows][columns].blue = 0; 
            }
            else if (max_color == red)
            {
                // set pixel to new color value at max value for red
                result[rows][columns].red = 255;
                result[rows][columns].green = 0;
                result[rows][columns].blue = 0; 
            }
            else if (max_color == green)
            {
                // set pixel to new color value at max value for green
                result[rows][columns].red = 0;
                result[rows][columns].green = 255;
                result[rows][columns].blue = 0; 
            }   
            else
            {
                // set pixel to new color value at max value for blue
                result[rows][columns].red = 0;
                result[rows][columns].green = 0;
                result[rows][columns].blue = 255; 
            }
        }
    }
    return result;
}
