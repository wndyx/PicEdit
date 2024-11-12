#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "image_manip.h"
#include "ppm_io.h"


////////////////////////////////////////
// Definitions of the functions       //
// declared in image_manip.h go here! //
////////////////////////////////////////

int radius_calculator(int x, int y, int x_new, int y_new, int radius){ 
	//distance formula between two points
	double calc_val = sqrt((x - x_new) * (x - x_new) + (y - y_new) * (y - y_new));

	//checks if the distance is greater than the radius
	if(calc_val > radius){
		return 0;
	}else{
		return 1;
	}
}

/* Convert a RGB pixel to a single grayscale intensity;
* uses NTSC standard conversion
*/
unsigned char pixel_to_gray( Pixel p ) {
	return (unsigned char)( 0.3 * p.r + 0.59 * p.g + 0.11 * p.b );
}

//______invert______
/* invert the intensity of each channel 
*/
Image invert( const Image in){
	Image inverted_in = make_image(in.rows, in.cols);
	for( int i = 0; i < in.rows; i++){
		for(int j = 0; j < in.cols; j++){
			inverted_in.data[i * in.cols + j].r = 255 - in.data[i * in.cols + j].r;
			inverted_in.data[i * in.cols + j].g = 255 - in.data[i * in.cols + j].g;
			inverted_in.data[i * in.cols + j].b = 255 - in.data[i * in.cols + j].b;

		}
	}
	return inverted_in;
}

//______crop______
/* takes an image and points defining a bounding box,
* and crops that box out of the image, returning a newly
* created image containing just the cropped region
*/
Image crop( const Image in , Point top_left , Point bot_right ){
    //allocate memory for the cropped image
    int cropped_rows=bot_right.x-top_left.x;
    int cropped_cols=bot_right.y-top_left.y;
    Image cropped_image = make_image(cropped_rows, cropped_cols);

    //if(!cropped_iamge.data）
    //Copy pixels from the original image to the cropped image
    for(int i=0; i<cropped_rows; i++){
        for(int j=0; j<cropped_cols; j++){
            cropped_image.data[i*cropped_cols+j] = in.data[(top_left.x+i)*in.cols+top_left.y+j];
        }
    }
    return cropped_image;
}


//______zoom_out______
/* "zoom out" an image, by taking a 2x2 square of pixels and averaging
* each of the three color channels to make a single pixel. If an odd
* number of rows in original image, we lose info about the bottom row.
* If an odd number of columns in original image, we lose info about the
* rightmost column.
*/
Image zoom_out( const Image in ){
	Image inverted_in = make_image(in.rows / 2, in.cols / 2); //initialize the Image that stores the zoom
	for( int i = 0; i < in.rows / 2; i++){ //iterates through the rows a even number of times
		for(int j = 0; j < in.cols / 2; j++){ //iterates through the columns a even number of times
			inverted_in.data[i * (in.cols / 2) + j].r = (in.data[(i * 2) * in.cols + j * 2].r + //takes average of the r values
												in.data[(i * 2 + 1) * in.cols + j * 2].r + //and adds the values to the 
												in.data[(i * 2) * in.cols + j * 2 + 1].r + //corresponding space in inverted_in
												in.data[(i * 2 + 1) * in.cols + j * 2 + 1].r) / 4 ; 
			inverted_in.data[i * (in.cols / 2) + j].g = (in.data[(i * 2) * in.cols + j * 2].g + //same as above but for g
												in.data[(i * 2 + 1) * in.cols + j * 2].g + 
												in.data[(i * 2) * in.cols + j * 2 + 1].g + 
												in.data[(i * 2 + 1) * in.cols + j * 2 + 1].g) / 4 ; 
			inverted_in.data[i * (in.cols / 2) + j].b = (in.data[(i * 2) * in.cols + j * 2].b + //same as above but for b
												in.data[(i * 2 + 1) * in.cols + j * 2].b + 
												in.data[(i * 2) * in.cols + j * 2 + 1].b + 
												in.data[(i * 2 + 1) * in.cols + j * 2 + 1].b) / 4 ; 
		}
	}
	return inverted_in;
}

//_______pointilism________
/* apply a painting like effect, i.e. pointilism technique.
*/
Image pointilism( const Image in , unsigned int seed ){
	srand(seed);
	int num_chosen = in.rows * in.cols * 0.03; //finds the number of samples to take (3%)
	int radius, min_x, min_y, max_x, max_y; //initializes the random radius
	Image inverted_in = make_image(in.rows, in.cols); //where the output is stored
	Point rand_p; //stores the coordinates for the random point
	for(int l = 0; l < num_chosen; l++){ //finds num_chosen amount of random samples
		radius = (rand() % 5) + 1; //random radius stored in variable radius
		rand_p.x = rand() % in.rows; //random x coordinate
		rand_p.y = rand() % in.cols; //random y coordinate
		//to fill the entire data array in inverted_in
		min_x = rand_p.x - radius; // find the minimal x value possible for the circle
		if(min_x < 0){
			min_x = 0;
		}
		min_y = rand_p.y - radius; // find the minimal y value possible for the circle
		if(min_y < 0){
			min_y = 0;
		}
		max_x = rand_p.x + radius + 1; // find the maximum x value possible for the circle
		if(max_x > in.rows){
			max_x = in.rows;
		}
		max_y = rand_p.y + radius + 1; // find the maximum y value possible for the circle
		if(max_y > in.cols){
			max_y = in.cols;
		}

		//iterates through a rectangle that encompasses the circle around the point
		for(int i  = min_x; i < max_x; i++){ //iterates through min and max of the circle x
			for(int j = min_y; j < max_y; j++){ //iterates through min and max of the circle y 
				if(radius_calculator(rand_p.x, rand_p.y, i, j, radius)){ //checks if the point is within radius
					inverted_in.data[i * in.cols + j] = in.data[rand_p.x * in.cols + rand_p.y]; //copy Pixel to inverted_in
				}
			}
		}
	}
	return inverted_in; //return the finished image
}

// Function to create a Gaussian matrix (kernel) based on sigma and size N
// The matrix represents a 2D Gaussian distribution used for blurring.
double** make_Gaussian(double sigma, int N) {
    // Allocate memory for the Gaussian matrix (N x N)
    double** matrix = (double**)malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++) {
        matrix[i] = (double*)malloc(N * sizeof(double));
    }

    int center = N / 2;  // Center of the Gaussian kernel
    // Calculate each value of the Gaussian matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // Formula for Gaussian distribution (2D)
            matrix[i][j] = (1.0 / (2.0 * PI * sq(sigma))) * exp(-(sq(i - center) + sq(j - center)) / (2 * sq(sigma)));
        }
    }

    // Return the generated Gaussian matrix
    return matrix;
}

// Function to free the memory allocated for the Gaussian matrix
// N is the size of the matrix (N x N)
void free_matrix(double** matrix, int N) {
    // Free each row of the matrix
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    // Free the main pointer that holds the row pointers
    free(matrix);
}

// Function to calculate the blurred value of a single pixel using Gaussian convolution
// im: the original image
// x, y: the coordinates of the pixel to blur
// N: size of the Gaussian kernel
// matrix: the precomputed Gaussian kernel
Pixel blur_pixel(Image im, int x, int y, int N, double** matrix) {
    double sum = 0;  // Accumulates the total weight (used for normalization)
    double r_sum = 0, g_sum = 0, b_sum = 0;  // Accumulates the weighted sum for each color channel

    // Iterate over the neighborhood (N x N) centered at (x, y)
    for (int i = x - N / 2; i <= x + N / 2; i++) {
        for (int j = y - N / 2; j <= y + N / 2; j++) {
            // Check if the neighboring pixel is within the image boundaries
            if (i >= 0 && i < im.rows && j >= 0 && j < im.cols) {
                // Calculate the corresponding weight from the Gaussian kernel
                double weight = matrix[i - x + N / 2][j - y + N / 2];

                // Get the pixel from the image at (i, j)
                Pixel p = im.data[i * im.cols + j];

                // Accumulate the weighted color values (r, g, b)
                r_sum += p.r * weight;
                g_sum += p.g * weight;
                b_sum += p.b * weight;

                // Accumulate the total weight for normalization
                sum += weight;
            }
        }
    }

    // Create a new pixel to store the blurred color values
    Pixel blur_p;
    // Normalize the accumulated color values by the sum of the weights
    blur_p.r = (unsigned char)(r_sum / sum);
    blur_p.g = (unsigned char)(g_sum / sum);
    blur_p.b = (unsigned char)(b_sum / sum);

    // Return the blurred pixel
    return blur_p;
}

// ______blur______
// Apply a blurring filter to the entire image using a Gaussian kernel
// in: the input image
// sigma: standard deviation for the Gaussian distribution (controls blur intensity)
Image blur(const Image in, double sigma) {
    // Create a new image to store the blurred result (same dimensions as the input image)
    Image blurred_image = make_image(in.rows, in.cols);

    // Calculate the size N of the Gaussian kernel (based on sigma)
    // N is always an odd number (to ensure symmetry around the center)
    int N = (int)(10 * sigma) | 1;

    // Generate the Gaussian kernel based on sigma and size N
    double** gaussian_matrix = make_Gaussian(sigma, N);

    // Iterate over every pixel in the input image
    for (int i = 0; i < in.rows; i++) {
        for (int j = 0; j < in.cols; j++) {
            // Apply the Gaussian blur to each pixel
            blurred_image.data[i * in.cols + j] = blur_pixel(in, i, j, N, gaussian_matrix);
        }
    }

    // Free the memory used by the Gaussian matrix
    free_matrix(gaussian_matrix, N);

    // Return the blurred image
    return blurred_image;
}


//______binarize______
//convert to a black and white image based on a threshold

Image binarize( const Image in , const int threshold ){
    // Validate the threshold
    if (threshold < 0 || threshold > 255) {
        fprintf(stderr, "Threshold must be between 0 and 255.\n");
        return (Image){ NULL, 0, 0 };
    }
    // Create a new image for the binarized output
    Image binarized_image = make_image(in.rows, in.cols);

    // Iterate over each pixel and binarize
    for (int i = 0; i < in.rows; i++) {
        for (int j = 0; j < in.cols; j++) {
            Pixel p = in.data[i * in.cols + j];
            unsigned char gray = pixel_to_gray(p);  

            // Apply threshold to binarize
            unsigned char binary_value = (gray < threshold) ? 0 : 255;

            // Set the binary value to all color channels of the pixel
            binarized_image.data[i * in.cols + j].r = binary_value;
            binarized_image.data[i * in.cols + j].g = binary_value;
            binarized_image.data[i * in.cols + j].b = binary_value;
        }
    }

    // Return the binarized image
    return binarized_image;
}
