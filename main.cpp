#include "imageProc.cpp"

int main()
{
    string input_image;
    int selection;
    double scaling_factor;
    int num_rotations;
    int x_scale;
    int y_scale;
    string output_image;
    int user_continue = 1;
    cout << "************************************************" << endl;
    cout << " CSPB 1300 Image Processing Application" << endl;
    cout << "        Coded by Mayra Weidner        " << endl;
    cout << "************************************************" << endl;
    cout << " " << endl;  
    cout << "Enter input BMP file name including file extension ";
    cin >> input_image;
    while (user_continue == 1) // loop will run through options until user quits or inputs invalid option
    { 
        cout << "You are currently working with " << input_image << endl;
        cout << " " << endl;
        cout << "-------------------------------------------" << endl;
        cout << "          IMAGE PROCESSING MENU" << endl;
        cout << "-------------------------------------------" << endl;
        cout << "1) Vignette" << endl;
        cout << "2) Clarendon" << endl;
        cout << "3) Grayscale" << endl;
        cout << "4) Rotate 90 degrees" << endl;
        cout << "5) Rotate in multiples of 90 degrees" << endl;
        cout << "6) Enlarge" << endl;
        cout << "7) High contrast" << endl;
        cout << "8) Lighten" << endl;
        cout << "9) Darken" << endl;
        cout << "10) Black, white, red, green, and blue" << endl;
        cout << "11) Change image" << endl;
        cout << " " << endl;
        cout << "Enter menu selection or Q to quit: " << endl;
        cin >> selection;    
        if (selection == 1)
        {
            cout << "You selected vignette" << endl; 
            cout << "Enter out BMP file name including bmp file extension " << endl;
            cin >> output_image;
            vector<vector<Pixel>> image = read_image(input_image);  // Read bmp file and store in 2D vector
            vector<vector<Pixel>> new_image = process_1(image); // call using input vector and save to new vector
            bool success = write_image(output_image, new_image);  // Write 2D vector to file
            cout << " " << endl;
            cout << "***Successfully applied vignette!***" << endl;
            cout << " " << endl;
        }
        else if (selection == 2)
        {
            cout << "You selected clarendon" << endl; 
            cout << "Enter out BMP file name including bmp file extension " << endl;
            cin >> output_image;
            cout << "Enter positive decimal for scaling factor (i.e. 0.3): " << endl;
            cin >> scaling_factor;
            vector<vector<Pixel>> image = read_image(input_image);  
            vector<vector<Pixel>> new_image = process_2(image, scaling_factor); 
            bool success = write_image(output_image, new_image); 
            cout << " " << endl;
            cout << "***Successfully applied clarendon!***" << endl;
            cout << " " << endl;
        }
        else if (selection == 3)
        {
            cout << "You selected grayscale" << endl; 
            cout << "Enter out BMP file name including bmp file extension " << endl;
            cin >> output_image;
            vector<vector<Pixel>> image = read_image(input_image);  
            vector<vector<Pixel>> new_image = process_3(image); 
            bool success = write_image(output_image, new_image);  
            cout << " " << endl;
            cout << "***Successfully applied grayscale!***" << endl;
            cout << " " << endl;
        }
        else if (selection == 4)
        {
            cout << "You selected rotate 90 degrees" << endl; 
            cout << "Enter out BMP file name including bmp file extension " << endl;
            cin >> output_image;
            vector<vector<Pixel>> image = read_image(input_image);  
            vector<vector<Pixel>> new_image = process_4(image); 
            bool success = write_image(output_image, new_image);  
            cout << " " << endl;
            cout << "***Successfully applied 90 degree rotation!***" << endl;
            cout << " " << endl;
        }       
        else if (selection == 5)
        {
            cout << "You selected rotate multiple 90 degrees" << endl; 
            cout << "Enter out BMP file name including bmp file extension " << endl;
            cin >> output_image;
            cout << "Enter positive integer for number of 90 degree rotations (i.e. 2): " << endl;
            cin >> num_rotations;
            vector<vector<Pixel>> image = read_image(input_image);  
            vector<vector<Pixel>> new_image = process_5(image, num_rotations); 
            bool success = write_image(output_image, new_image);  
            cout << " " << endl;
            cout << "***Successfully applied multiple 90 degree rotations!***" << endl;
            cout << " " << endl;
        }   
        else if (selection == 6)
        {
            cout << "You selected enlarge" << endl; 
            cout << "Enter out BMP file name including bmp file extension " << endl;
            cin >> output_image;
            cout << "Enter positive integer for x scale (i.e. 2): " << endl;
            cin >> x_scale;
            cout << "Enter positive integer for y scale (i.e. 3): " << endl;
            cin >> y_scale;
            vector<vector<Pixel>> image = read_image(input_image);  
            vector<vector<Pixel>> new_image = process_6(image, x_scale, y_scale); 
            bool success = write_image(output_image, new_image);  
            cout << " " << endl;
            cout << "***Successfully applied enlarge!***" << endl;
            cout << " " << endl;
        } 
        else if (selection == 7)
        {
            cout << "You selected high contrast" << endl; 
            cout << "Enter out BMP file name including bmp file extension " << endl;
            cin >> output_image;
            vector<vector<Pixel>> image = read_image(input_image); 
            vector<vector<Pixel>> new_image = process_7(image); 
            bool success = write_image(output_image, new_image);  
            cout << " " << endl;
            cout << "***Successfully applied high contrast!***" << endl;
            cout << " " << endl;
        }
        else if (selection == 8)
        {
            cout << "You selected lighten" << endl; 
            cout << "Enter out BMP file name including bmp file extension " << endl;
            cin >> output_image;
            cout << "Enter positive decimal for scaling factor (i.e. 0.3): " << endl;
            cin >> scaling_factor;
            vector<vector<Pixel>> image = read_image(input_image);  
            vector<vector<Pixel>> new_image = process_8(image, scaling_factor); 
            bool success = write_image(output_image, new_image);  
            cout << " " << endl;
            cout << "***Successfully applied lighten!***" << endl;
            cout << " " << endl;
        }
        else if (selection == 9)
        {
            cout << "You selected darken" << endl; 
            cout << "Enter out BMP file name including bmp file extension " << endl;
            cin >> output_image;
            cout << "Enter positive decimal for scaling factor (i.e. 0.3): " << endl;
            cin >> scaling_factor;
            vector<vector<Pixel>> image = read_image(input_image);  
            vector<vector<Pixel>> new_image = process_9(image, scaling_factor); 
            bool success = write_image(output_image, new_image);  
            cout << " " << endl;
            cout << "***Successfully applied darken!***" << endl;
            cout << " " << endl;
        }
        else if (selection == 10)
        {
            cout << "You selected black, white, red, green, and blue" << endl; 
            cout << "Enter out BMP file name including bmp file extension " << endl;
            cin >> output_image;
            vector<vector<Pixel>> image = read_image(input_image);  
            vector<vector<Pixel>> new_image = process_10(image); 
            bool success = write_image(output_image, new_image);  
            cout << " " << endl;
            cout << "***Successfully applied black, white, red, green, and blue filter!***" << endl;
            cout << " " << endl;
        }
        else if (selection == 11)
        {
            cout << "What file would you like to use now? " << endl;
            cin >> input_image;
        }
        else
        {
            cout << "Thank you for using my program! Quitting now..." << endl;
            user_continue = 0;
        }
    }
    return 0;
}
