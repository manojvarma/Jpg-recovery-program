/**
 * recover.c
 * Penmatsa Manoj
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdint.h>

#define SUCCESS         0
#define ERR_OPEN_FILE   1

#define BLOCK_SIZE      512
#define RAW_FILENAME    "card.raw"

typedef uint8_t BYTE;


int main(void)
{
    int err;

    // open raw file
    FILE* rawFile = fopen(RAW_FILENAME, "r");
    
    if (rawFile != NULL)
    {
        // setup a buffer to hold chunks of bytes read in from the raw file
        BYTE buffer[BLOCK_SIZE];
        
        // setup an index for target filenames
        int fileIndex = -1;
        
        // create a pointer to a new recovered file, but wait to open
        // it until a file pattern match is actually found
        FILE* recovFile = NULL;
        char rcovFilename[13];
        
        
        // start to read in the raw file
        while (fread(buffer, BLOCK_SIZE, 1, rawFile))
        {
            // are the first 4 bytes a JPEG image file signature?
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff
                && (buffer[3] == 0xe0 || buffer[3] == 0xe1))
            {
                // if we've been writing to a file, close it, since
                // this means we've encountered a new file header
                if (recovFile != NULL)
                {
                    fclose(recovFile);
                }
                  
                // create new file to write to
                fileIndex++;
                sprintf(rcovFilename, "%03i.jpg", fileIndex);
                recovFile = fopen(rcovFilename, "w");
            }
             
            // continue to write blocks...
            if (recovFile != NULL)
            {
                fwrite(buffer, BLOCK_SIZE, 1, recovFile);
            }
        }
        
        // close any recovered file we may have open
        if (recovFile != NULL)
        {
            fclose(recovFile);
        }
        
        fclose(rawFile);

        err = SUCCESS;
    }
    else
    {
        printf("Unable to open file %s\n", RAW_FILENAME);
        err = ERR_OPEN_FILE;
    }
    
    return err;
}

