/**
 * @Author: Axel_Soll <amrsoll>
 * @Date:   08/01/2018
 * @Email:  axel.soll@telecom-paristech.fr
 * @Last modified by:   amrsoll
 * @Last modified time: 08/01/2018
 */



#ifndef CLASSES
#define CLASSES

typedef struct Point Point;
struct Point {
    int x;
    int y;
};

typedef struct fPoint fPoint;
struct fPoint {
    float x;
    float y;
};

typedef struct Pixel Pixel;
struct Pixel {
    int x;
    int y;
    char type; //undefined : 0, clear : 1 or wall : 2
};

bool pixel_eq(Pixel p1, Pixel p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

bool point_eq(Point p1, Point p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

float norm(fPoint p)
{
    return sqrt(p.x*p.x + p.y*p.y);
}

fPoint project_onto(fPoint v, fPoint p)
{
    fPoint output;
    float n = norm(v);
    output.x = v.x*p.x/n;
    output.y = v.y*p.y/n;
    return output;
}

float sign (fPoint p1, fPoint p2, fPoint p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool fpoint_in_trigon (fPoint pt, fPoint v1, fPoint v2, fPoint v3)
{
    bool b1, b2, b3;

    b1 = sign(pt, v1, v2) < 0.0f;
    b2 = sign(pt, v2, v3) < 0.0f;
    b3 = sign(pt, v3, v1) < 0.0f;

    return ((b1 == b2) && (b2 == b3));
}

bool intpoint_in_trigon (Point s, Point a, Point b, Point c)
{
    int as_x = s.x-a.x;
    int as_y = s.y-a.y;

    bool s_ab = (b.x-a.x)*as_y-(b.y-a.y)*as_x > 0;

    if((c.x-a.x)*as_y-(c.y-a.y)*as_x > 0 == s_ab) return false;

    if((c.x-b.x)*(s.y-b.y)-(c.y-b.y)*(s.x-b.x) > 0 != s_ab) return false;

    return true;
}
#endif
