#include <math.h>
#include <iostream>
#include "main.hpp"

/*  Vector Class definition starts -
 *  this class defines a
 *  variable which has two parameters - x,y
 *  and represents a vector on the cartesian plane system
 *  joining (x,y) and the origin - (0,0).
 */

// If no input is provided to the vector constructor sets the point as the origin (0,0) .
// Hence vector defaults to zero-vector.
Vector::Vector(){
    i=0.0f; j=0.0f;
}

Vector::Vector(float x, float y){
    i=x;j=y;
}
// The constructor of the vector class ends here .

/*  Member function angle is defined in the class vector which
 *  gives the argument angle of that specific vector with respect
 *  to the the cartesian x and y axes . It finds the angle of the
 *  line joining (x,y) and the origin  (0,0). The angle is given
 *  (in radians) by tan inverse of (y-0/x-0) . It is multiplied by
 *  180/pi to get the angle in degrees. Since tan-1 returns angles
 *  only in (-90 deg , 90 deg) , so by adding 180 to the negative
 *  values of theta, all the angles are now brought in (0,180deg).
 *  For y<0 , vector points in 3rd or 4th quadrant and the angle
 *  has to be reflex , so 180 deg is added to these angles 
 */
float Vector::angle(){
    float theta = atan(j/i)*180/PI;
    if(theta<0) theta+=180;
    if(j<0) theta+=180;

    return theta;
}

//This function gives the modulus (length) of the vector
float Vector::mod(){
    return sqrtf(i*i+j*j);
}

void Vector::rotate(float d_theta){
// Rotates the given vector  in anticlockwise direction by d_theta degrees
    float theta = angle()+d_theta, r = mod();

    i = r*cos(theta*PI/180); j = r*sin(theta*PI/180);
// After adding the angles , the co-ordinates of the point are [rsin(theta),rcos(theta)]
// but the theta are in degrees so they are multiplied by pi/180
}

Vector Vector::operator*(float f){
    Vector v(this->i*f,this->j*f);

    return v;
}

//defines a vector with starting point A and end point B and will then transform
// this to the coordinate system with A placed at the origin
Vector::Vector(Point& start, Point& end){
    i=end.x-start.x; j=end.y-start.y;
}

/* VECTOR CLASS DEFINITON ENDS HERE*/


/* Point class definitions starts here and defines a point in the cartesian plane*/
Point::Point(){
    x=0; y=0;
}

Point::Point(float a, float b){
    x=a;
    y=b;
}
//Constructor for point ends and will default any point to origin if arguments are not provided

//Defines addition operation of two points wherein the respective coordinates of the vectors are added
Point Point::operator+(const Vector& V){
    Point newp;

    newp.x=this->x+V.i;
    newp.y=this->y+V.j;

    return newp;
}

//defines the length (distance) between the current point and point p
float Point::distance(Point& p){
    return sqrtf((p.x-x)*(p.x-x)+(p.y-y)*(p.y-y));
}

void Point::draw(int size, float r, float g, float b, float alpha){//draw function for drawing a point starts
    glBegin(GL_QUADS);
    //a point is represented as a very small square (quadrilateral) on the GLUT canvas
        glColor4f(r,g,b,alpha);

        glVertex2f((x-size)/WINDOW_SIZE, (y-size)/WINDOW_SIZE);
        glVertex2f((x+size)/WINDOW_SIZE, (y-size)/WINDOW_SIZE);
        glVertex2f((x+size)/WINDOW_SIZE, (y+size)/WINDOW_SIZE);
        glVertex2f((x-size)/WINDOW_SIZE, (y+size)/WINDOW_SIZE);
        //the square is made with (x,y) as the center of the point and dimension of
        //2*size . Here the x coordinates will be x-size and x+ size while the y 
        //co-ordinates will be y-size and y+size. So a combination of these x and y 
        //co-ordinates will give the 4 vertices of the square.
    glEnd();
    //draw function ends
}

/*Point class definitions end here */

/* Arc class definiton starts here and defines an arc of a circle on the
cartesian plane with a start point , an end point , a centre and whether
the arc is in clockwise direction or anticlockwise direction form the start point*/
Arc::Arc(Point s, Point e, Point c, int dir){

    start.x=s.x;start.y=s.y;
    end.x=e.x; end.y=e.y;
    centre.x=c.x; centre.y=c.y;
// defines the start , end and centre points of the arc

    radius=start.distance(centre);
// Radius is the distance between the center and either of the start or end points

    Vector a(centre, start), b(centre, end);
// Vectors are defined which join the start and end points to the centre of the circle

    sweep = b.angle()-a.angle();
// the angle between these 2 vectors gives the angle enclosed by the arc as this is the angle
// between the 2 radii joining the centre to points a and b

    direction=sgn(sweep);
// If the argument of vector a was more than vector b , then the arc swept was in clockwise direction
// and thus direction should be set to -1 ( as given by signum (-ve number)) and vice versa for argument
// angle of b more than that of vecor a

    // to get the minor arc of the circle formed between the two points and the centre of the circle
    // if the angle enclosed between the 2 points is more than 180 , then accordingly it is made 
    // between -180 and 180 degrees
    if(dir==MINOR_ARC && (sweep<-180 || sweep>180))
        sweep=sweep-direction*360;

    if(dir != MINOR_ARC && dir != direction)
        sweep=sweep-direction*360;

    direction=sgn(sweep);

    theta = sweep;

    sweep=sweep/100;
    //sweep is broken into 100 parts which is then used later for plotting the arc


    cout<<"Radius : "<<radius<<", Theta : "<<theta<<endl;

}

void Arc::draw(){
    // draw function for s=drawing arcs starts
    glBegin(GL_LINE_STRIP);
    // drawing the arc starts - drawing a series of lines and just considering the ends points
        glColor3f(BLUE);
        // color is set to blue

        Vector s(centre, start);
        // a vector is defined from the center of the arc to the starting point
        // rotating this vector about the centre in the specified direction will
        // give the required points on the arc 

        //cout<<"Sweep  angle:" << sweep<<endl<<"Start:"<<s.angle()<<", End:"<<e.angle()<<endl;
        for(int i=0; i<100; i++){
            s.rotate(sweep);
            //the current vector is rotated about c by an angle of sweep i.e; 0.01 of the total angle of the arc
            Point p=centre+s;
            //the required point is found by adding the vector to the centre point
            glVertex2f(p.x/WINDOW_SIZE,p.y/WINDOW_SIZE);
            //point is marked on the canvas w.r.t. the dimensions of the canvas dimensions

        }
    glEnd();
    // drawing function ends
}

/* This function gives the direction of the next arc formed by considering the points - centre ,
 end and the new point . Just by comparing the x coordinates we can tell whether
the next arc will be in clockwise or anticlockwise direction. If the centre and the
point are on the same side of the line x= end.x ; i.e; the line parallel to y axis
passing through point end , then the next arc will be in clockwise direction and vice versa*/
int Arc::nextDirection(Point c2){
    return direction*sgn((centre.x-end.x)/(c2.x-end.x));
}

/* Arc class definiton ends here */
