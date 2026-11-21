#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Define standard FAT file system block size
#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // Ensure proper usage with exactly one command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the forensic memory card image for reading
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Create a 512-byte buffer to store blocks read from the card
    uint8_t buffer[BLOCK_SIZE];

    // Track output file pointer, filename string, and JPEG counter
    FILE *img = NULL;
    char filename[8];
    int jpeg_count = 0;

    // Read 512-byte blocks until the end of the memory card
    while (fread(buffer, 1, BLOCK_SIZE, card) == BLOCK_SIZE)
    {
        // Check if the current block starts with a valid JPEG header signature
        bool is_jpeg = (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
                        (buffer[3] & 0xf0) == 0xe0);

        if (is_jpeg)
        {
            // Close the previous JPEG file if one is currently open
            if (img != NULL)
            {
                fclose(img);
            }

            // Format the new filename (e.g., 000.jpg) and open a new file
            sprintf(filename, "%03i.jpg", jpeg_count);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                fclose(card);
                return 1;
            }
            jpeg_count++;
        }

        // Write the current 512-byte block if an output JPEG is active
        if (img != NULL)
        {
            fwrite(buffer, 1, BLOCK_SIZE, img);
        }
    }

    // Close any remaining open files to prevent memory leaks
    if (img != NULL)
    {
        fclose(img);
    }
    fclose(card);

    return 0;
}
