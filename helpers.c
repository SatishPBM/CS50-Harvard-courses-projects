#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int average_color = round(((float)image[i][j].rgbtBlue + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtRed) / 3);

            image[i][j].rgbtBlue = average_color;
            image[i][j].rgbtGreen = average_color;
            image[i][j].rgbtRed = average_color;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            int sepiaRed = round(0.189 * image[i][j].rgbtBlue + 0.769 * image[i][j].rgbtGreen + 0.393 * image[i][j].rgbtRed);

            if (sepiaRed > 255) // 255 is max value
            {
                sepiaRed = 255;
            }

            int sepiaGreen = round(0.168 * image[i][j].rgbtBlue + 0.686 * image[i][j].rgbtGreen + 0.349 * image[i][j].rgbtRed);

            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            int sepiaBlue = round(0.131 * image[i][j].rgbtBlue + 0.534 * image[i][j].rgbtGreen + 0.272 * image[i][j].rgbtRed);

            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtRed = sepiaRed;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            int refBlue = image[i][j].rgbtBlue;
            int refGreen = image[i][j].rgbtGreen;
            int refRed = image[i][j].rgbtRed;

            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;

            image[i][width - j - 1].rgbtBlue = refBlue;
            image[i][width - j - 1].rgbtGreen = refGreen;
            image[i][width - j - 1].rgbtRed = refRed;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE temp[height][width];    //create a copy of image as it will be getting overwritten

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0) // top row
            {
                if (j == 0) // for first column of top row
                {

                    image[i][j].rgbtBlue = (int)round((temp[i][j].rgbtBlue +
                                                       temp[i][j + 1].rgbtBlue +
                                                       temp[i + 1][j].rgbtBlue +
                                                       temp[i + 1][j + 1].rgbtBlue) / 4.0);

                    image[i][j].rgbtGreen = (int)round((temp[i][j].rgbtGreen +
                                                        temp[i][j + 1].rgbtGreen +
                                                        temp[i + 1][j].rgbtGreen +
                                                        temp[i + 1][j + 1].rgbtGreen) / 4.0);

                    image[i][j].rgbtRed = (int)round((temp[i][j].rgbtRed +
                                                      temp[i][j + 1].rgbtRed +
                                                      temp[i + 1][j].rgbtRed +
                                                      temp[i + 1][j + 1].rgbtRed) / 4.0);


                }
                if ((j > 0) && (j < (width - 1))) // top row columns, excluding 1st and last column
                {

                    image[i][j].rgbtBlue = (int)round((temp[i][j - 1].rgbtBlue +
                                                       temp[i][j].rgbtBlue +
                                                       temp[i][j + 1].rgbtBlue +
                                                       temp[i + 1][j - 1].rgbtBlue +
                                                       temp[i + 1][j].rgbtBlue +
                                                       temp[i + 1][j + 1].rgbtBlue) / 6.0);

                    image[i][j].rgbtGreen = (int)round((temp[i][j - 1].rgbtGreen +
                                                        temp[i][j].rgbtGreen +
                                                        temp[i][j + 1].rgbtGreen +
                                                        temp[i + 1][j - 1].rgbtGreen +
                                                        temp[i + 1][j].rgbtGreen +
                                                        temp[i + 1][j + 1].rgbtGreen) / 6.0);

                    image[i][j].rgbtRed = (int)round((temp[i][j - 1].rgbtRed +
                                                      temp[i][j].rgbtRed +
                                                      temp[i][j + 1].rgbtRed +
                                                      temp[i + 1][j - 1].rgbtRed +
                                                      temp[i + 1][j].rgbtRed +
                                                      temp[i + 1][j + 1].rgbtRed) / 6.0);
                }

                if (j == (width - 1)) // top row, last column
                {
                    image[i][j].rgbtBlue = (int)round((temp[i][j - 1].rgbtBlue +
                                                       temp[i][j].rgbtBlue +
                                                       temp[i + 1][j - 1].rgbtBlue +
                                                       temp[i + 1][j].rgbtBlue) / 4.0);

                    image[i][j].rgbtGreen = (int)round((temp[i][j - 1].rgbtGreen +
                                                        temp[i][j].rgbtGreen +
                                                        temp[i + 1][j - 1].rgbtGreen +
                                                        temp[i + 1][j].rgbtGreen) / 4.0);

                    image[i][j].rgbtRed = (int)round((temp[i][j - 1].rgbtRed +
                                                      temp[i][j].rgbtRed +
                                                      temp[i + 1][j - 1].rgbtRed +
                                                      temp[i + 1][j].rgbtRed) / 4.0);

                }
            }

            if ((i > 0) && (i < (height - 1))) // for rows other than top row and last row
            {
                if (j == 0) // first column of rows other than top and last row
                {
                    image[i][j].rgbtBlue = (int)round((temp[i - 1][j].rgbtBlue +
                                                       temp[i - 1][j + 1].rgbtBlue +
                                                       temp[i][j].rgbtBlue +
                                                       temp[i][j + 1].rgbtBlue +
                                                       temp[i + 1][j].rgbtBlue +
                                                       temp[i + 1][j + 1].rgbtBlue) / 6.0);

                    image[i][j].rgbtGreen = (int)round((temp[i - 1][j].rgbtGreen +
                                                        temp[i - 1][j + 1].rgbtGreen +
                                                        temp[i][j].rgbtGreen +
                                                        temp[i][j + 1].rgbtGreen +
                                                        temp[i + 1][j].rgbtGreen +
                                                        temp[i + 1][j + 1].rgbtGreen) / 6.0);

                    image[i][j].rgbtRed = (int)round((temp[i - 1][j].rgbtRed +
                                                      temp[i - 1][j + 1].rgbtRed +
                                                      temp[i][j].rgbtRed +
                                                      temp[i][j + 1].rgbtRed +
                                                      temp[i + 1][j].rgbtRed +
                                                      temp[i + 1][j + 1].rgbtRed) / 6.0);


                }


                if ((j > 0) && (j < (width - 1))) // except last and first columns of rows other than top and last row
                {
                    image[i][j].rgbtBlue = (int)round((temp[i - 1][j - 1].rgbtBlue +
                                                       temp[i - 1][j].rgbtBlue +
                                                       temp[i - 1][j + 1].rgbtBlue +
                                                       temp[i][j - 1].rgbtBlue +
                                                       temp[i][j].rgbtBlue +
                                                       temp[i][j + 1].rgbtBlue +
                                                       temp[i + 1][j - 1].rgbtBlue +
                                                       temp[i + 1][j].rgbtBlue +
                                                       temp[i + 1][j + 1].rgbtBlue) / 9.0);

                    image[i][j].rgbtGreen = (int)round((temp[i - 1][j - 1].rgbtGreen +
                                                        temp[i - 1][j].rgbtGreen +
                                                        temp[i - 1][j + 1].rgbtGreen +
                                                        temp[i][j - 1].rgbtGreen +
                                                        temp[i][j].rgbtGreen +
                                                        temp[i][j + 1].rgbtGreen +
                                                        temp[i + 1][j - 1].rgbtGreen +
                                                        temp[i + 1][j].rgbtGreen +
                                                        temp[i + 1][j + 1].rgbtGreen) / 9.0);

                    image[i][j].rgbtRed = (int)round((temp[i - 1][j - 1].rgbtRed +
                                                      temp[i - 1][j].rgbtRed +
                                                      temp[i - 1][j + 1].rgbtRed +
                                                      temp[i][j - 1].rgbtRed +
                                                      temp[i][j].rgbtRed +
                                                      temp[i][j + 1].rgbtRed +
                                                      temp[i + 1][j - 1].rgbtRed +
                                                      temp[i + 1][j].rgbtRed +
                                                      temp[i + 1][j + 1].rgbtRed) / 9.0);

                }

                if (j == (width - 1)) // last column of rows other than top and last rows
                {
                    image[i][j].rgbtBlue = (int)round((temp[i - 1][j - 1].rgbtBlue +
                                                       temp[i - 1][j].rgbtBlue +
                                                       temp[i][j - 1].rgbtBlue +
                                                       temp[i][j].rgbtBlue +
                                                       temp[i + 1][j - 1].rgbtBlue +
                                                       temp[i + 1][j].rgbtBlue) / 6.0);

                    image[i][j].rgbtGreen = (int)round((temp[i - 1][j - 1].rgbtGreen +
                                                        temp[i - 1][j].rgbtGreen +
                                                        temp[i][j - 1].rgbtGreen +
                                                        temp[i][j].rgbtGreen +
                                                        temp[i + 1][j - 1].rgbtGreen +
                                                        temp[i + 1][j].rgbtGreen) / 6.0);

                    image[i][j].rgbtRed = (int)round((temp[i - 1][j - 1].rgbtRed +
                                                      temp[i - 1][j].rgbtRed +
                                                      temp[i][j - 1].rgbtRed +
                                                      temp[i][j].rgbtRed +
                                                      temp[i + 1][j - 1].rgbtRed +
                                                      temp[i + 1][j].rgbtRed) / 6.0);

                }

            }

            if (i == (height - 1)) // for last row
            {
                if (j == 0) // first column of last row
                {

                    image[i][j].rgbtBlue = (int)round((temp[i - 1][j].rgbtBlue +
                                                       temp[i - 1][j + 1].rgbtBlue +
                                                       temp[i][j].rgbtBlue +
                                                       temp[i][j + 1].rgbtBlue) / 4.0);

                    image[i][j].rgbtGreen = (int)round((temp[i - 1][j].rgbtGreen +
                                                        temp[i - 1][j + 1].rgbtGreen +
                                                        temp[i][j].rgbtGreen +
                                                        temp[i][j + 1].rgbtGreen) / 4.0);

                    image[i][j].rgbtRed = (int)round((temp[i - 1][j].rgbtRed +
                                                      temp[i - 1][j + 1].rgbtRed +
                                                      temp[i][j].rgbtRed +
                                                      temp[i][j + 1].rgbtRed) / 4.0);


                }

                if ((j > 0) && (j < (width - 1))) // except last and first column of last row
                {
                    image[i][j].rgbtBlue = (int)round((temp[i - 1][j - 1].rgbtBlue +
                                                       temp[i - 1][j].rgbtBlue +
                                                       temp[i - 1][j + 1].rgbtBlue +
                                                       temp[i][j - 1].rgbtBlue +
                                                       temp[i][j].rgbtBlue +
                                                       temp[i][j + 1].rgbtBlue) / 6.0);

                    image[i][j].rgbtGreen = (int)round((temp[i - 1][j - 1].rgbtGreen +
                                                        temp[i - 1][j].rgbtGreen +
                                                        temp[i - 1][j + 1].rgbtGreen +
                                                        temp[i][j - 1].rgbtGreen +
                                                        temp[i][j].rgbtGreen +
                                                        temp[i][j + 1].rgbtGreen) / 6.0);

                    image[i][j].rgbtRed = (int)round((temp[i - 1][j - 1].rgbtRed +
                                                      temp[i - 1][j].rgbtRed +
                                                      temp[i - 1][j + 1].rgbtRed +
                                                      temp[i][j - 1].rgbtRed +
                                                      temp[i][j].rgbtRed +
                                                      temp[i][j + 1].rgbtRed) / 6.0);

                }

                if (j == (width - 1)) // last column of last row
                {
                    image[i][j].rgbtBlue = (int)round((temp[i - 1][j - 1].rgbtBlue +
                                                       temp[i - 1][j].rgbtBlue +
                                                       temp[i][j - 1].rgbtBlue +
                                                       temp[i][j].rgbtBlue) / 4.0);

                    image[i][j].rgbtGreen = (int)round((temp[i - 1][j - 1].rgbtGreen +
                                                        temp[i - 1][j].rgbtGreen +
                                                        temp[i][j - 1].rgbtGreen +
                                                        temp[i][j].rgbtGreen) / 4.0);

                    image[i][j].rgbtRed = (int)round((temp[i - 1][j - 1].rgbtRed +
                                                      temp[i - 1][j].rgbtRed +
                                                      temp[i][j - 1].rgbtRed +
                                                      temp[i][j].rgbtRed) / 4.0);

                }
            }
        }
    }
    return;
}