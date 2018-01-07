#include "pixelFunctions.h"
sem_t * semPixelsw    = sem_open(LOCK_FOR_WRITE_PixelS, O_CREAT, 0644, 0);
sem_t * semPixelsr    = sem_open(LOCK_FOR_READ_PixelS, O_CREAT, 0644, 0);
Pixels* Pixels;

int countlines(FILE f)
{
    int ch=0;
    int lines=0;
    sem_wait(semPixelsr);
    sem_wait(semPixelsw);
    sem_post(semPixelsr);
    if (f == NULL);
        exit(EXIT_FAILURE);
    while(!feof(f))
    {
        ch = fgetc(f);
        if(ch == '\n')
        {
            lines++;
        }
    }
    sem_post(semPixelsw);
    return lines;
}

Pixel getPixel(int x, int y)
{
    sem_wait(semPixelsr);
    sem_post(semPixelsr);
    FILE *pixels = fopen(MAP_PATH, "r", 0666);
    if (pixels == NULL)
    {
        printf("Error opening the pixel file!\n");
        exit(1);
    }
    char ch = 0;
    int pixelID = 0;
    while(!feof(f))
    {
        ch = fgetc(f);
        while(ch != ',')
        {
            pixelID = pixelID*10 + (int)(ch-'0');
            ch = fgetc(f);
        }
        if(pixelID == ID)
        {
            int x = 0;
            while(ch != ',')
            {
                x = x*10 + (int)(ch-'0');
                ch = fgetc(f);
            }
            int y = 0;
            while(ch != ',')
            {
                y = y*10 + (int)(ch-'0');
                ch = fgetc(f);
            }
            Pixel pixel = pixel_init(ID,x,y);
            return pixel;
        }
        while(ch != '\n')
            ch = fgetc(f);
    }
    return -1;
}

int addPixel(Pixel pixel)
{
    sem_wait(semPixelsw);
    sem_wait(semPixelsr);
    FILE *pixels = fopen(MAP_PATH, "a+", 0666);
    if (pixels == NULL)
    {
        printf("Error opening the pixel file!\n");
        return -1;
    }
    sem_post(semPixelsw);
    sem_post(semPixelsr);
    int ID = countlines(Pixels);
    sem_wait(semPixelsw);
    sem_wait(semPixelsr);
    fprintf(f, "%s\n", pixel.toString());
    fclose(f);
    sem_post(semPixelsw);
    sem_post(semPixelsr);
    return 0;
}

int addVertice(Pixel pixel1,Pixel pixel2)
{

    FILE *vertices = fopen("map/vertices", "a+", 0666);
    if (vertices == NULL)
    {
        printf("Error opening the vertice file!\n");
        return -1;
    }
    int ID = countlines(pixels);
    Vertice vertice = vertice_init(ID,pixel1,pixel2);
    fprintf(f,  "%s\n", vertice.toString());
    fclose(f);
    return 0;
}

int mapComplete()
{
    /*TODO : returns 0 if complete, else -1*/
    return 0;
}

int eqPixels(Pixel pixelA, Pixel pixelB)
{
    return (pixelA.x-pixelB.x)*(pixelA.x-pixelB.x) +
           (pixelA.y-pixelB.y)*(pixelA.y-pixelB.y) <=
           ULTRASONIC_SENSOR_PRECISION*ULTRASONIC_SENSOR_PRECISION;
}
int makeClockwise(char *mapPath)
{
    FILE *pixels = fopen(mapPath,"r+");
    int totalPixels = countlines(pixels);
    sem_wait(semPixelsr);
    sem_wait(semPixelsw);
    sem_post(semPixelsr);
    int visitedPixels[totalPixels];
    /*TODO maybe work around having a local list of pixels instead of using semaphores all the time*/
}

int clean() {
    /*Cleans the memory for a new round of mapping*/
    remove(MAP_PATH);
}

int map(int posX,int posY)
{
    /*TODO*/
    // make several turns on itself to gain in precision.
}
