#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    if (argc != 2) // accept only 1 argument
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("Could not open %s\n", argv[1]);
        return 1;
    }


    FILE *outfile = NULL;

    unsigned char buffer[512]; // to reads blocks of 512 bytes from input file

    int jpgnum = 0; // for creating JPEG filename counter

    char jpgfile[8];

    while (fread(buffer, sizeof(buffer), 1, file) == 1) // read until end of file
    {
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0)) // denotes start of JPEG file
        {

            if (jpgnum != 0) // close file, except when it is the first block read
            {
                fclose(outfile);
            }

            sprintf(jpgfile, "%03i.jpg", jpgnum); // generate filename for JPEG

            outfile = fopen(jpgfile, "w"); // write current block of data read

            if (outfile == NULL)
            {
                printf("Error opening %s for write.\n", jpgfile);
                return 1;
            }

            jpgnum = jpgnum + 1; // increment filename counter

        }

        if (jpgnum != 0) // if there is atleast one JPEG block read
        {
            fwrite(buffer, sizeof(char), 512, outfile);
        }

    }

    fclose(file);
    fclose(outfile);
    return 0;

}