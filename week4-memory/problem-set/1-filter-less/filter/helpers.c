#include "helpers.h"

#include <math.h>
#include <stdlib.h>

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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Original color values for pixel
            int originalRed   = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue  = image[i][j].rgbtBlue;

            // Calculate new color values based on a sepia formula. Limit values to 255
            int sepiaRed   = limit(round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue));
            int sepiaGreen = limit(round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue));
            int sepiaBlue  = limit(round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue));

            // Set sepia color values for pixel
            image[i][j].rgbtRed   = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue  = sepiaBlue;
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
