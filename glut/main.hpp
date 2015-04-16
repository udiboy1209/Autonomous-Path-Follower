#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <fstream>

#ifndef _PATH_FOLLOW_H
#define _PATH_FOLLOW_H 0

#define PI 3.141592F
#define WINDOW_SIZE 350
#define OBSTACLE_RADIUS 100

#define RED 1.0f,0.0f,0.0f
#define GREEN 0.0f,1.0f,0.0f
#define BLUE 0.0f,0.0f,1.0f
#define BLACK 0.0f,0.0f,0.0f
#define WHITE 1.0f,1.0f,1.0f

#define CLOCKWISE -1
#define ANTICLOCKWISE 1
#define MINOR_ARC 0

using namespace std;

class Point;

class Vector{
    public:
        float i, j;
        //constructor of class vector

        Vector();
        Vector(float, float);
        //vector joining a point (x,y) to origin
        Vector(Point&, Point&);
        // vector joining 2 points

        float angle();
        //angle made by the vector with the x-axis in the anticlockwise direction
        float mod();
        //length of the vector
        void rotate(float);
        //function which rotates a vector by a specific angle

        Vector operator*(float);
};

class Point{
    public:

    float x;
    float y;

    Point();
    Point(float, float);
    //constructor of class Point
    Point operator+(const Vector&);
    // addition operation defined which adds corresponding coordinates of the 2 points

    float distance(Point& p);
    //calculates the distance of this point from a point p

    void draw(int,float,float,float,float=1.0f);
    //draws the point on the GLUT canvas
};

class Arc{
    public:
        Point start, end, centre;
        //an arc is specified with its centre , starting point and end point

        float radius, sweep, theta;
        //radius of the arc and the angle enclosed by the arc are specified

        int direction;
        //whether the arc is formed in the clockwise direction or the anticlockwise direcn is specfied

        Arc(Point, Point, Point, int);
        void draw();
        // draws the arc on the GLUT canvas

        int nextDirection(Point);
        // finds the direction of the arc formed by joining the end point of this arc and a new point
};

/* util methods declaration begins */

// Three arrays are declared
extern vector<Point> path;
extern vector<Point> centres;
extern vector<Point> obstacles;
extern vector<Arc> arcs;

// Function addPoint adds a new point to the array path
// and generates centres and arcs accordingly
void addPoint(int, int);

void addObstacle(int, double, double);

//This function finds the circumcenter of three given points A, B and C
Point circumcenter(Point A, Point B, Point C);

//returns the value of square of the distance of the point from
//the origin ( square of modulus of the vector joining origin and A )
float modsq(Point&);

//Prints out the coordinates of a point P in (x,y) format
ostream& operator<<(ostream&,const Point&);

// definition of a signum function which is 1 for all positive numbers
// and -1 for all negative numbers
int sgn(float);

/* util methods declaration ends */

#endif
