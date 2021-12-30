#include "helpers.h"
#include <math.h>

// Prototypes
int limit(int number);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Original color values for pixel
            int originalRed   = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue  = image[i][j].rgbtBlue;

            int average = round((originalRed + originalGreen + originalBlue) / 3.0);

            // Set average value as new color values for pixel
            image[i][j].rgbtRed   = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue  = average;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy pixel values from image to tempImage
    RGBTRIPLE tempImage[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tempImage[i][j].rgbtRed   = image[i][j].rgbtRed;
            tempImage[i][j].rgbtGreen = image[i][j].rgbtGreen;
            tempImage[i][j].rgbtBlue  = image[i][j].rgbtBlue;
        }
    }

    // Calculate the average color values of nearby pixels and set new color values in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Used for calculating the average color values of up to 9 pixels
            int sumRed   = 0;
            int sumGreen = 0;
            int sumBlue  = 0;
            float nPixels = 0.0;

            // For each pixel: Sum up color values for up to 9 nearby pixels
            for (int row = i - 1; row <= i + 1; row++)
            {
                if (row < 0 || row >= height)
                {
                    continue;
                }
                for (int column = j - 1; column <= j + 1; column++)
                {
                    if (column < 0 || column >= width)
                    {
                        continue;
                    }
                    sumRed   += tempImage[row][column].rgbtRed;
                    sumGreen += tempImage[row][column].rgbtGreen;
                    sumBlue  += tempImage[row][column].rgbtBlue;
                    nPixels++;
                }
            }

            // Calculates the average color values of up to 9 pixels
            int avgRed   = roundf(sumRed   / nPixels);
            int avgGreen = roundf(sumGreen / nPixels);
            int avgBlue  = roundf(sumBlue  / nPixels);

            // Set the new average color values for this pixel to create the box blur
            image[i][j].rgbtRed   = avgRed;
            image[i][j].rgbtGreen = avgGreen;
            image[i][j].rgbtBlue  = avgBlue;
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy pixel values from image to originalImage
    RGBTRIPLE originalImage[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            originalImage[i][j].rgbtRed   = image[i][j].rgbtRed;
            originalImage[i][j].rgbtGreen = image[i][j].rgbtGreen;
            originalImage[i][j].rgbtBlue  = image[i][j].rgbtBlue;
        }
    }

    // Loop through each pixel in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Used for this pixel to hold gx and gy values for each color
            int gxRed   = 0;
            int gxGreen = 0;
            int gxBlue  = 0;

            int gyRed   = 0;
            int gyGreen = 0;
            int gyBlue  = 0;

            // For each pixel: Loop through up to 9 surrounding pixels
            for (int row = i - 1; row <= i + 1; row++)
            {
                if (row < 0 || row >= height)
                {
                    continue;
                }
                for (int column = j - 1; column <= j + 1; column++)
                {
                    if (column < 0 || column >= width)
                    {
                        continue;
                    }

                    // Upper row
                    if (row < i)
                    {
                        // Upper left pixel
                        if (column < j)
                        {
                            gxRed   +=  originalImage[row][column].rgbtRed   * -1;
                            gxGreen +=  originalImage[row][column].rgbtGreen * -1;
                            gxBlue  +=  originalImage[row][column].rgbtBlue  * -1;

                            gyRed   +=  originalImage[row][column].rgbtRed   * -1;
                            gyGreen +=  originalImage[row][column].rgbtGreen * -1;
                            gyBlue  +=  originalImage[row][column].rgbtBlue  * -1;
                        }
                        // Upper middle pixel
                        else if (column == j)
                        {
                            gyRed   +=  originalImage[row][column].rgbtRed   * -2;
                            gyGreen +=  originalImage[row][column].rgbtGreen * -2;
                            gyBlue  +=  originalImage[row][column].rgbtBlue  * -2;
                        }
                        // Upper right pixel
                        else
                        {
                            gxRed   +=  originalImage[row][column].rgbtRed   *  1;
                            gxGreen +=  originalImage[row][column].rgbtGreen *  1;
                            gxBlue  +=  originalImage[row][column].rgbtBlue  *  1;

                            gyRed   +=  originalImage[row][column].rgbtRed   * -1;
                            gyGreen +=  originalImage[row][column].rgbtGreen * -1;
                            gyBlue  +=  originalImage[row][column].rgbtBlue  * -1;
                        }
                    }
                    // Middle row
                    else if (row == i)
                    {
                        // Middle left pixel
                        if (column < j)
                        {
                            gxRed   +=  originalImage[row][column].rgbtRed   * -2;
                            gxGreen +=  originalImage[row][column].rgbtGreen * -2;
                            gxBlue  +=  originalImage[row][column].rgbtBlue  * -2;
                        }
                        // Middle right pixel
                        else if (column > j)
                        {
                            gxRed   +=  originalImage[row][column].rgbtRed   * 2;
                            gxGreen +=  originalImage[row][column].rgbtGreen * 2;
                            gxBlue  +=  originalImage[row][column].rgbtBlue  * 2;
                        }
                    }
                    // Bottom row
                    else
                    {
                        // Bottom left pixel
                        if (column < j)
                        {
                            gxRed   +=  originalImage[row][column].rgbtRed   * -1;
                            gxGreen +=  originalImage[row][column].rgbtGreen * -1;
                            gxBlue  +=  originalImage[row][column].rgbtBlue  * -1;

                            gyRed   +=  originalImage[row][column].rgbtRed   * 1;
                            gyGreen +=  originalImage[row][column].rgbtGreen * 1;
                            gyBlue  +=  originalImage[row][column].rgbtBlue  * 1;
                        }
                        // Bottom middle pixel
                        else if (column == j)
                        {
                            gyRed   +=  originalImage[row][column].rgbtRed   * 2;
                            gyGreen +=  originalImage[row][column].rgbtGreen * 2;
                            gyBlue  +=  originalImage[row][column].rgbtBlue  * 2;
                        }
                        // Bottom right pixel
                        else
                        {
                            gxRed   +=  originalImage[row][column].rgbtRed   * 1;
                            gxGreen +=  originalImage[row][column].rgbtGreen * 1;
                            gxBlue  +=  originalImage[row][column].rgbtBlue  * 1;

                            gyRed   +=  originalImage[row][column].rgbtRed   * 1;
                            gyGreen +=  originalImage[row][column].rgbtGreen * 1;
                            gyBlue  +=  originalImage[row][column].rgbtBlue  * 1;
                        }
                    }
                }
            }

            // Calculates the new color values for edge filter
            int edgeRed   = limit(round(sqrt(pow(gxRed,   2) + pow(gyRed,   2))));
            int edgeGreen = limit(round(sqrt(pow(gxGreen, 2) + pow(gyGreen, 2))));
            int edgeBlue  = limit(round(sqrt(pow(gxBlue,  2) + pow(gyBlue,  2))));

            // Set the new edge color values for this pixel
            image[i][j].rgbtRed   = edgeRed;
            image[i][j].rgbtGreen = edgeGreen;
            image[i][j].rgbtBlue  = edgeBlue;
        }
    }
}

// Limit number to 255
int limit(int number)
{
    if (number > 255)
    {
        return 255;
    }
    return number;
}