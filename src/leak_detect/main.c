#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PICSIZE 256     // this should be deleted and a 1920 and 1080 int value should be used
#define MAXMASK 100

int pic[PICSIZE][PICSIZE];
int histogram[PICSIZE];

double maskx[MAXMASK][MAXMASK];
double masky[MAXMASK][MAXMASK];

double xconv[PICSIZE][PICSIZE];
double yconv[PICSIZE][PICSIZE];

double ival[PICSIZE][PICSIZE];      // scaled magnitudes
double cand[PICSIZE][PICSIZE];      // peaks
double final[PICSIZE][PICSIZE];

// args -> inputFile.pgm   output1.pgm   output2.pgm   output3.pgm   sigma   percent
int main(int argc, char **argv)
{
    int i, j, p, q, mr, centx, centy, moretodo;
    double maskxval, maskyval, sum1, sum2, sig, slope, maxival, percent, cutoff, loThres = 0, hiThres = 0, area = 0, peaksCount = 0;
    FILE *fo1, *fo2, *fo3, *fp1;

    // input file
    fp1 = fopen(argv[1], "rb");


    // output files
    fo1 = fopen(argv[2], "wb");
    fprintf(fo1, "P5\n");
    fprintf(fo1, "%d %d\n", PICSIZE, PICSIZE);
    fprintf(fo1, "255\n");

    fo2 = fopen(argv[3], "wb");
    fprintf(fo2, "P5\n");
    fprintf(fo2, "%d %d\n", PICSIZE, PICSIZE);
    fprintf(fo2, "255\n");

    fo3 = fopen(argv[4], "wb");
    fprintf(fo3, "P5\n");
    fprintf(fo3, "%d %d\n", PICSIZE, PICSIZE);
    fprintf(fo3, "255\n");

    sig = atoi(argv[5]);
    percent = atof(argv[6]);

    mr = (int) (sig * 3);
    centx = (MAXMASK / 2);
    centy = (MAXMASK / 2);



    for (i = 0; i < PICSIZE; i++)
        for (j = 0; j < PICSIZE; j++)
            pic[i][j] = getc(fp1);



    // calculate derivatives and masks
    for (p = -mr; p <= mr; p++)
        for (q = -mr; q <= mr; q++)
        {
            maskxval = (q * (exp(-1 * (((p * p) + (q * q)) / (2 * (sig * sig))))));
            maskyval = (p * (exp(-1 * (((p * p) + (q * q)) / (2 * (sig * sig))))));

            maskx[p + centy][q + centx] = maskxval;
            masky[p + centy][q + centx] = maskyval;
        }



    // compute convolutions for each pixel
    for (i = mr; i <= 255 - mr; i++)
        for (j = mr; j <= 255 - mr; j++)
        {
            sum1 = 0;
            sum2 = 0;

            for (p = -mr; p <= mr; p++)
                for (q = -mr; q <= mr; q++)
                {
                    sum1 += pic[i + p][j + q] * maskx[p + centy][q + centx];
                    sum2 += pic[i + p][j + q] * masky[p + centy][q + centx];
                }

            xconv[i][j] = sum1;
            yconv[i][j] = sum2;
        }



    // create magnitudes and store in ival
    maxival = 0;
    for (i = mr; i < PICSIZE - mr; i++)
        for (j = mr; j < PICSIZE - mr; j++)
        {
            ival[i][j] = sqrt((xconv[i][j] * xconv[i][j]) + (yconv[i][j] * yconv[i][j]));

            if (ival[i][j] > maxival)
                maxival = ival[i][j];
        }


    // create magnitude pgm file and scale the values in ival
    for (i = 0; i < PICSIZE; i++)
        for (j = 0; j < PICSIZE; j++)
        {
            ival[i][j] = (ival[i][j] / maxival) * 255;
            fprintf(fo1, "%c", (char) ((int) (ival[i][j])));
        }



    // calculate peaks
    for (i = mr; i < PICSIZE - mr; i++)
        for (j = mr; j < PICSIZE - mr; j++)
        {
            if ( (xconv[i][j]) == 0.0)
                xconv[i][j] = .00001;

            slope = yconv[i][j] / xconv[i][j];

            if ( (slope <= .4142) && (slope > -.4142) )
            {
                if ( (ival[i][j] > ival[i][j - 1]) && (ival[i][j] > ival[i][j + 1]) )
                    cand[i][j] = 255;
            }
            else if ( (slope <= 2.4142) && (slope > .4142) )
            {
                if ( (ival[i][j] > ival[i - 1][j - 1]) && (ival[i][j] > ival[i + 1][j + 1]) )
                    cand[i][j] = 255;
            }
            else if( (slope <= -.4142) && (slope > -2.4142) )
            {
                if ( (ival[i][j] > ival[i + 1][j - 1]) && (ival[i][j] > ival[i - 1][j + 1]))
                    cand[i][j] = 255;
            }
            else {
                if ( (ival[i][j] > ival[i - 1][j]) && (ival[i][j] > ival[i + 1][j]) )
                    cand[i][j] = 255;
            }
        }



    // print peaks
    for (i = 0; i < PICSIZE; i++)
        for (j = 0; j < PICSIZE; j++)
            fprintf(fo2, "%c", (char) ((int) (cand[i][j])));



    for (i = 0; i < PICSIZE; i++)
        histogram[i] = 0;


    // calculate histogram
    for (i = 0; i < PICSIZE; i++)
        for (j = 0; j < PICSIZE; j++)
            histogram[(int) (ival[i][j])]++;



    // calculate threshold values
    cutoff = percent * PICSIZE * PICSIZE;

    for (i = PICSIZE - 1; i >= 0; i--)
    {
        area += histogram[i];

        if (area > cutoff)
        {
            hiThres = i;
            loThres = 0.35 * hiThres;
            break;
        }
    }



    // apply thresholds
    for (i = 0; i < PICSIZE; i++)
        for (j = 0; j < PICSIZE; j++)
            if (cand[i][j] == 255)
            {
                if (ival[i][j] > hiThres)
                {
                    cand[i][j] = 0;
                    final[i][j] = 255;
                }
                else if (ival[i][j] < loThres)
                {
                    cand[i][j] = 0;
                    final[i][j] = 0;
                }
            }



    moretodo = 1;
    while (moretodo == 1)
    {
        moretodo = 0;

        // check adjacent neighbors to pixels that fall between the lo and hi thresholds and determine if they
        // have a neighbor that is included in the final output image. If so, repeat loop.
        for (i = 0; i < PICSIZE; i++)
            for (j = 0; j < PICSIZE; j++)
                if (cand[i][j] == 255)
                    for (p = -1; p <= 1; p++)
                        for (q = -1; q <= 1; q++)
                            if (final[i + p][j + q] == 255)
                            {
                                cand[i][j] = 0;
                                final[i][j] = 255;
                                peaksCount += 1;
                                moretodo = 1;
                            }
    }


    // print peaks
    for (i = 0; i < PICSIZE; i++)
        for (j = 0; j < PICSIZE; j++)
            fprintf(fo3, "%c", (char) ((int) (final[i][j])));


    // check percentage of peaks pixels relative to rest of image
    double percentPeaks = (peaksCount / (PICSIZE * PICSIZE)) * 100;

    if (peaksCount > 5) {
        printf("Leak detected");
        // create output image and return (depends on how we will process image in updated gui website).
    }


    return 0;
}
