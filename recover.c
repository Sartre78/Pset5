/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
 
typedef uint8_t  BYTE;
 
int main(void)
{
    // Open Card File
    FILE* file = fopen("card.raw", "r");
    
    // create buffer array
    BYTE buffer[512];
    
    // create picture counter
    int jpg_counter = 0;
    
    // create title array
    char* title = malloc(1000000 * sizeof(int));
    FILE* img;
    
    while (fread(&buffer, sizeof(BYTE), 512, file) == 512)
    {
    
        // Case 1:  Very 1st JPG found 
        if (jpg_counter == 0 && buffer[0] == 0xff && buffer[1] == 0xd8 
        && buffer[2] == 0xff && (buffer[3] == 0xe0 || buffer[3] == 0xe1))
        {
            // title new JPG
            sprintf(title, "%.3d.jpg", jpg_counter);
            // open image for writing 
            img = fopen(title, "w");
            // write to image
            fwrite(buffer, sizeof(BYTE), 512, img);
            // increase JPG counter  
            jpg_counter++;    
        }
        
        // Case 2:  Subsequent JPG found
        else if (jpg_counter != 0 && buffer[0] == 0xff && buffer[1] == 0xd8 
        && buffer[2] == 0xff && (buffer[3] == 0xe0 || buffer[3] == 0xe1))
        {
            // close img
            fclose(img); 
            // title new JPG
            sprintf(title, "%.3d.jpg", jpg_counter); 
            // open image for writing
            img = fopen(title, "w"); 
            // open image for writing
            // write to image
            fwrite(buffer, sizeof(BYTE), 512, img); 
            // increase JPG counter 
            jpg_counter++;   
        }
        
        // Case 3: In-between JPG's
        else if (jpg_counter != 0)
        {
            // write to image
            fwrite(buffer, sizeof(BYTE), 512, img); 
        }
        
        // Case 4:  1st JPG has yet to be discovered
        else
        {
            // Nothing to do
        }
    }
    // free title
    free(title);
    // close img
    fclose(img);
    // close "card.raw"
    fclose(file);
}   
