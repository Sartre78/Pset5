/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Resizes a BMP piece by piece by an inputed
 * factor of n.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // scaling factor
    int n = atoi(argv[1]);
    
    // limits scaling factor to a maximum of 100x
    if (n >= 100 || n < 1)
    {
        printf("Resize limit: 100x max, 1x min\n");
        return 1;
    }
    
    // remember file names
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Change bi.biWidth, bi.biHeight (note: abs value), and bi.biSizeImage
    // in BITMAPINFOHEADER.
    // Change bf.bfSize (size of file) in BITMAPFILEHEADER.
    
    int original_biWidth = bi.biWidth;
    int original_biHeight = bi.biHeight;

    // elongate bi.biWidth by a factor of n
    bi.biWidth *= n;
    
    // elongate bi.biHeight by a factor of n
    bi.biHeight *= n;
    
    // determine total bytes of padding to add
    int size_padding =  abs(bi.biHeight) * 
    ((4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4);
    
    // resize biSizeImage
    bi.biSizeImage = 3 * ((bi.biWidth) * abs(bi.biHeight)) + size_padding;
    
    // resize bf.bfSize
    bf.bfSize = 54 + bi.biSizeImage;
    

  // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int old_padding =  (4 - (original_biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // Temporary storage
    RGBTRIPLE *RGB = malloc(sizeof(RGBTRIPLE) * bi.biWidth);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(original_biHeight); i < biHeight; i++)
    {
        // set counter for pixel copying
        int counter = 0;
        
        // iterate over pixels in scanline
        for (int j = 0; j < original_biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            for (int jj = 0; jj < n; jj++)
            {
                // store RGB triple to RGB "n" number of times
                RGB[counter] = triple;
                counter++;
            }
        }

        // skip over old padding, if any
        fseek(inptr, old_padding, SEEK_CUR);

        for (int k = 0; k < n; k++)
        {
            // write RGB pixels to outfile from temp storage RGB
            fwrite(RGB, sizeof(RGBTRIPLE), bi.biWidth, outptr);
            // add any new padding, if appicable
            for (int kk = 0; kk < new_padding; kk++)
            {
                fputc(0x00, outptr);
            }
        }
    }

    // free temporary memory allocated in tempStore
    free(RGB);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
