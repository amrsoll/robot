/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   08/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 15/01/2018
 */



#include "classes.h"

Point Point_new(int x, int y) {
  Point p;
  p.x = x;
  p.y = y;
  return p;
}

tCoord tCoord_new(int i, int j) {
  tCoord p;
  p.i = i;
  p.j = j;
  return p;
}

fPoint fPoint_new(float x, float y) {
  fPoint p;
  p.x = x;
  p.y = y;
  return p;
}

Pixel Pixel_new(int x, int y, char type) {
  Pixel p;
  p.x = x;
  p.y = y;
  p.type = type;
  return p;
}

tCoord tCoord_init_str(char* str,size_t size)
{
    char* I,J;
    int i,j;
    i=0
    while(str[0]!=','){
        i++;
    }
    str[i] = '\0';
    j = atoi(&(str[i+1]));
    i = atoi(str);
    return tCoord_new(i,j);
}

char* tCoord_to_str(tCoord tc)
{
    char* str;
    fprintf(str, "%d,%d\n", tc.i, tc.j);
    return str;
}

Point fPoint_to_Point(fPoint f)
{
    return Point_new((int)f.x,(int)f.y);
}

fPoint Point_to_fPoint(Point f)
{
    return fPoint_new((float)f.x,(float)f.y);
}

tCoord Point_to_tCoord(Point p, tCoord origin)
//converts the cordinates of a point from a cartesian plot
//to table coordinates, where origin is the position of the
//cartesian plot's origin point inside the table.
{
    tCoord q = tCoord_new(origin.i-p.y,
                          origin.j+p.x);
    return q;
}

Point tCoord_to_Point(tCoord tc, tCoord origin)
{
    return Point_new(origin.i-tc.i,
                     tc.j-origin.j);
}

Point Padd(Point a, Point b)
{
    return Point_new(a.x+b.x, a.y+b.y);
}

fPoint fadd(fPoint a, fPoint b)
{
    return fPoint_new(a.x+b.x, a.y+b.y);
}

tCoord tadd(tCoord a, tCoord b)
{
    return tCoord_new(a.i+b.i, a.j+b.j);
}

Point Psub(Point a, Point b)
{
    return Point_new(a.x-b.x, a.y-b.y);
}

fPoint fsub(fPoint a, fPoint b)
{
    return fPoint_new(a.x-b.x, a.y-b.y);
}

tCoord tsub(tCoord a, tCoord b)
{
    return tCoord_new(a.i-b.i, a.j-b.j);
}


bool intsquare_fray_intersect(Point sqCoord,fPoint r0,fPoint r1)
//sqCoord is the coordinate of the bottom left summit of the square.
//The square is supposed to be of size 1.
//checks if the ray intersects one of the two diagonal lines.
{
    float rx = r1.x-r0.x;
    float ry = r1.y-r0.y;

    float inRay1 =( sqCoord.y  -r0.y    -  sqCoord.x + r0.x )*SQRT2;
    float inRay2 =( sqCoord.y+1-r0.y    +  sqCoord.x - r0.x )*SQRT2;
    float inDiag1=( sqCoord.y  -r0.y)*rx+(-sqCoord.x + r0.x )*ry;
    float inDiag2=( sqCoord.y+1-r0.y)*rx+(-sqCoord.x + r0.x )*ry;

    return( (inRay1>=0.0 && inDiag1>=0.0 && inDiag1<SQRT2*(ry-rx))
          ||(inRay2>=0.0 && inDiag2>=0.0 && inDiag2<SQRT2*(ry+rx)));
}

bool Pixel_eq(Pixel p1, Pixel p2)
{
    return p1.x == p2.x && p1.y == p2.y && p1.type == p2.type;
}

bool Point_eq(Point p1, Point p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

bool tCoord_eq(tCoord p1, tCoord p2)
{
    return p1.i == p2.i && p1.j == p2.j;
}

float intnorm(Point p)
{
    return sqrt(p.x*p.x + p.y*p.y);
}

float fnorm(fPoint p)
{
    return sqrt(p.x*p.x + p.y*p.y);
}

float tcnorm(tCoord tc)
{
    return sqrt(tc.i*tc.i + tc.j*tc.j);
}


// fPoint fproject_onto(fPoint v, fPoint p)
// {
//     fPoint output;
//     float n = fnorm(v);
//     output.x = v.x*p.x/n;
//     output.y = v.y*p.y/n;
//     return output;f
// }

float sign (fPoint p1, fPoint p2, fPoint p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool fPoint_in_trigon (fPoint pt, fPoint v1, fPoint v2, fPoint v3)
{
    bool b1, b2, b3;

    b1 = sign(pt, v1, v2) <= 0.0f;
    b2 = sign(pt, v2, v3) <= 0.0f;
    b3 = sign(pt, v3, v1) <= 0.0f;

    return ((b1 == b2) && (b2 == b3));
}

bool intpoint_in_trigon (Point p, Point a, Point b, Point c)
{
    int as_x = p.x-a.x;
    int as_y = p.y-a.y;

    bool s_ab = (b.x-a.x)*as_y-(b.y-a.y)*as_x >= 0;

    if((c.x-a.x)*as_y-(c.y-a.y)*as_x >= 0 == s_ab) return false;

    if((c.x-b.x)*(p.y-b.y)-(c.y-b.y)*(p.x-b.x) >= 0 != s_ab) return false;

    return true;
}

int set_char(tCoord coord, int width, int height, char value, char* str) //Changes the char value str on line i and j
{
    if(coord.i>=height){
        printf("set_char : index out of range : i = %d and height = %d\n", coord.i, height);
        return -1;
    }
    if(coord.j>=width){
        printf("set_char : index out of range : j = %d and width = %d\n", coord.j, width);
        return -1;
    }
    str[coord.i*(width+1)+coord.j] = value;
    return 0;
}

char get_char(tCoord coord, int width, int height, char* str)
//i0 and j0 is the position of the origin of the x and y axis
{
    if(coord.i>=height){
        printf("get_char : index out of range : i = %d and height = %d\n", coord.i, height);
        return -1;
    }
    if(coord.j>=width){
        printf("get_char : index out of range : j = %d and width = %d\n", coord.j, width);
        return -1;
    }
    return str[coord.i*(width+1)+coord.j];
}
