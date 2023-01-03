#include <stdio.h> /*  Marr-Hildreth.c  (or marrh.c) */
#include <stdlib.h>
#include <math.h>
#define PICSIZE 256
#define MAXMASK 100

int pic[PICSIZE][PICSIZE];
double outpicx[PICSIZE][PICSIZE];
double outpicy[PICSIZE][PICSIZE];
int edgeflag[PICSIZE][PICSIZE];
double xmask[MAXMASK][MAXMASK];
double ymask[MAXMASK][MAXMASK];
double convx[PICSIZE][PICSIZE];
double convy[PICSIZE][PICSIZE];
double peaks[PICSIZE][PICSIZE];
double ival[PICSIZE][PICSIZE];
double final[PICSIZE][PICSIZE];
int histogram[PICSIZE];

main(argc, argv) int argc;
char **argv;
{
    int i, j, p, q, s, t, mr, centx, centy, HI, LO, moretodo;
    double xmaskval, ymaskval, sum1, sum2, sig, maxival, minival, maxval, ZEROTOL, slope, cutoff, percent, areaOfTops;
    FILE *fo1, *fo2, *fo3, *fp1, *fopen();
    char *foobar;
    char throwaway[80];

    argc--;
    argv++;
    foobar = *argv;
    fp1 = fopen(foobar, "rb");

    fgets(throwaway, 80, fp1);
    fgets(throwaway, 80, fp1);
    fgets(throwaway, 80, fp1);
    if (!((throwaway[0] == '2') && (throwaway[1] == '5') && (throwaway[2] == '5')))
        fgets(throwaway, 80, fp1);

    argc--;
    argv++;
    foobar = *argv;
    fo1 = fopen(foobar, "wb");

    fprintf(fo1, "P5\n");
    fprintf(fo1, "%d %d\n", 256, 256);
    fprintf(fo1, "255\n");

    argc--;
    argv++;
    foobar = *argv;
    fo2 = fopen(foobar, "wb");

    fprintf(fo2, "P5\n");
    fprintf(fo2, "%d %d\n", 256, 256);
    fprintf(fo2, "255\n");

    argc--;
    argv++;
    foobar = *argv;
    fo3 = fopen(foobar, "wb");

    fprintf(fo3, "P5\n");
    fprintf(fo3, "%d %d\n", 256, 256);
    fprintf(fo3, "255\n");

    argc--;
    argv++;
    foobar = *argv;
    sig = atoi(foobar);

    // argc--;
    // argv++;
    // foobar = *argv;
    // HI = atoi(foobar);

    // argc--;
    // argv++;
    // foobar = *argv;
    // LO = atoi(foobar);

    argc--;
    argv++;
    foobar = *argv;
    percent = atof(foobar);

    mr = (int)(sig * 3);
    centx = (MAXMASK / 2);
    centy = (MAXMASK / 2);

    for (i = 0; i < 256; i++)
    {
        for (j = 0; j < 256; j++)
        {
            pic[i][j] = getc(fp1);
        }
    }

    for (p = -mr; p <= mr; p++)
    {
        for (q = -mr; q <= mr; q++)
        {
            xmaskval = ((q) *
                       (exp(-1 * (((p * p) + (q * q)) / (2 * (sig * sig))))));
            ymaskval = ((p) *
                       (exp(-1 * (((p * p) + (q * q)) / (2 * (sig * sig))))));
            (xmask[p + centy][q + centx]) = xmaskval;
            (ymask[p + centy][q + centx]) = ymaskval;
        }
    }

    for (i = mr; i <= 255 - mr; i++)
    {
        for (j = mr; j <= 255 - mr; j++)
        {
            sum1 = 0;
            sum2 = 0;
            for (p = -mr; p <= mr; p++)
            {
                for (q = -mr; q <= mr; q++)
                {
                    sum1 += pic[i + p][j + q] * xmask[p + centy][q + centx];
                    sum2 += pic[i + p][j + q] * ymask[p + centy][q + centx];
                }
            }
            outpicx[i][j] = sum1;
            outpicy[i][j] = sum2;
            convx[i][j] = sum1;
            convy[i][j] = sum2;
        }
    }

    maxval = 0;
    maxival = 0;
    minival = 255;

    for (i = mr; i < 256 - mr; i++)
    {
        for (j = mr; j < 256 - mr; j++)
        {
            if (outpicx[i][j] > maxival)
                maxival = outpicx[i][j];
            if (outpicx[i][j] < minival)
                minival = outpicx[i][j];
            if (outpicy[i][j] > maxival)
                maxival = outpicy[i][j];
            if (outpicy[i][j] < minival)
                minival = outpicy[i][j];
        }
    }

    if (fabs(maxival) > fabs(minival))
        maxval = fabs(maxival);
    else
        maxval = fabs(minival);

    // Part One

    for (i = mr; i < 256 - mr; i++)
    {
        for (j = mr; j < 256 - mr; j++)
        {
            ival[i][j] = sqrt((double)((outpicx[i][j] * outpicx[i][j]) +
                                       (outpicy[i][j] * outpicy[i][j])));
            if (ival[i][j] > maxival)
                maxival = ival[i][j];
        }
    }


    // Part One - Print

    for (i = 0; i < 256; i++)
    {
        for (j = 0; j < 256; j++)
        {
            ival[i][j] = (ival[i][j] / maxival) * 255;
            fprintf(fo1, "%c", (char)((int)(ival[i][j])));
        }
    }

    // Part Two

    for (i = mr; i < 256 - mr; i++)
    {
        for (j = mr; j < 256 - mr; j++)
        {
            if (convx[i][j] == 0.0)
            {
                convx[i][j] = .00001;
            }
            slope = convy[i][j] / convx[i][j];
            if ((slope <= .4142) && (slope > -.4142))
            {
                if ((ival[i][j] > ival[i][j - 1]) && (ival[i][j] > ival[i][j + 1]))
                {
                    peaks[i][j] = 255;
                }
            }
            else if ((slope <= 2.4142) && (slope > .4142))
            {
                if ((ival[i][j] > ival[i - 1][j - 1]) && (ival[i][j] > ival[i + 1][j + 1]))
                {
                    peaks[i][j] = 255;
                }
            }
            else if ((slope <= -.4142) && (slope > -2.4142))
            {
                if ((ival[i][j] > ival[i + 1][j - 1]) && (ival[i][j] > ival[i - 1][j + 1]))
                {
                    peaks[i][j] = 255;
                }
            }
            else
            {
                if ((ival[i][j] > ival[i - 1][j]) && (ival[i][j] > ival[i + 1][j]))
                {
                    peaks[i][j] = 255;
                }
            }
        }
    }

    // Part Two - Print

    for (i = 0; i < 256; i++)
    {
        for (j = 0; j < 256; j++)
        {
            fprintf(fo2, "%c", (char)((int)(peaks[i][j])));
        }
    }
    
    // Part Three and Part Four

    cutoff = percent * PICSIZE * PICSIZE;

    for (i = 0; i < 256; i++)
    {
        histogram[i] = 0;
    }

    for (i = 0; i < 256; i++)
    {
        for (j = 0; j < 256; j++)
        {
            histogram[(int)ival[i][j]]++;
        }
    }

    for (i = 255; i >= 0; i--)
    {
        areaOfTops += histogram[i];
        if (areaOfTops > cutoff)
        {
            break;
        }
    }

    HI = i;
    LO = .35*HI;

    for (i = 0; i < 256; i++)
    {
        for (j = 0; j < 256; j++)
        {
            if (peaks[i][j] > 0)
            {
                if (ival[i][j] > HI)
                {
                    peaks[i][j] = 0;
                    final[i][j] = 255;
                }
                else if (ival[i][j] < LO)
                {
                    peaks[i][j] = 0;
                    final[i][j] = 0;
                }
            }
        }
    }
    moretodo = 1;
    while (moretodo == 1) 
    {
        moretodo = 0;
        for (i = 0; i < 256; i++)
        {
            for (j = 0; j < 256; j++)
            {
                if (peaks[i][j] > 0)
                {
                    for (p = -1; p <= 1; p++)
                    {
                        for (q = -1; q <= 1; q++)
                        {
                            if (final[i + p][j + q] > 0)
                            {
                                peaks[i][j] = 0;
                                final[i][j] = 255;
                                moretodo = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    // Part Three and Four- Print

    for (i = 0; i < 256; i++)
    {
        for (j = 0; j < 256; j++)
        {
            fprintf(fo3, "%c", (char)((int)(final[i][j])));
        }
    }
}
