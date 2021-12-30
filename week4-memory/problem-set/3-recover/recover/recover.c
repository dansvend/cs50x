#include <cs50.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Type declarations
typedef uint8_t BYTE;

// Constants
const int BLOCK_SIZE = 512;

// Prototypes
bool hasJpegSignature(BYTE *buffer);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // Open input file
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 1;
    }

    BYTE *buffer = malloc(BLOCK_SIZE);
    FILE *outptr;
    int jpegsFound = 0;

    // Runs to the end of the file
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, inptr) == BLOCK_SIZE)
    {
        if (hasJpegSignature(buffer))
        {
            // Close jpeg file before starting to write a new one
            if (jpegsFound > 0)
            {
                fclose(outptr);
            }

            char imgName[8];
            sprintf(imgName, "%03i.jpg", jpegsFound);

            // Open output file
            outptr = fopen(imgName, "w");
            if (outptr == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", imgName);
                return 1;
            }
            jpegsFound++;
        }

        if (jpegsFound > 0)
        {
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, outptr);
        }
    }

    // Free memory for buffer
    free(buffer);

    // Close infile
    fclose(inptr);

    // Close imgName
    fclose(outptr);

    return 0;
}

// Checks if the first four bytes of a block of memory is a jpeg signature
bool hasJpegSignature(BYTE *buffer)
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
        return true;
    }
    return false;
}