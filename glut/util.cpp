#include "main.hpp"

#define THRESHOLD 70

//returns the value of square of the distance of the point from
//the origin ( square of modulus of the vector joining origin and A
float modsq(Point& A){
    return A.x*A.x + A.y*A.y;
}

//Prints out the coordinates of a point P in ( x,y) format
ostream& operator<<(ostream& cout,const Point& p){
    cout<<"("<<p.x<<","<<p.y<<")";

    return cout;
}

// definition os a signum function which is 1 for all positive numbers
//and -1 for all negative numbers
int sgn(float f){
    if(f<0) return -1;
    else return 1;
}

/* This function finds the circumcenter of three given points A, B and C
It is the intersection points of the perpendicular bisectors of the three
sides of the triangle formed by A , B and C. For a triangle with cordinates
A (A_x, A_y) ; B (B_x, B_y) ; C (C_x, C_y) , the circumcenter is given by
 CC ( CC_x , CC_y ) where -
CC_x = [(A_x^2 + A_y^2)(B_y - C_y) + (B_x^2 + B_y^2)(C_y - A_y) + (C_x^2 + C_y^2)(A_y - B_y)] / D,
CC_y = [(A_x^2 + A_y^2)(C_x - B_x) + (B_x^2 + B_y^2)(A_x - C_x) + (C_x^2 + C_y^2)(B_x - A_x)]/ D,

and D = 2[A_x(B_y - C_y) + B_x(C_y - A_y) + C_x(A_y - B_y)] */
Point circumcenter(Point A, Point B, Point C){
    float D=2*(A.x*(B.y-C.y)+B.x*(C.y-A.y)+C.x*(A.y-B.y));

    float x = (modsq(A)*(B.y-C.y)+modsq(B)*(C.y-A.y)+modsq(C)*(A.y-B.y));
    float y = (modsq(A)*(C.x-B.x)+modsq(B)*(A.x-C.x)+modsq(C)*(B.x-A.x));

    cout<<"D : "<<D<<", x : "<<x<<" y : "<<y<<endl;

    Point cc(x/D,y/D);

    return cc;
}

// Three arrays are defined one which will contain all the points , one which
// will contain all the centres of the arcs and one which will contain all the arcs formed
vector<Point> path;
vector<Point> centres;
vector<Arc> arcs;

// Function addPoint defined to add a new point to the array starts
void addPoint(int x, int y){
    Point p((float)x, (float)y);

    if(path.size()==0 || path[path.size()-1].distance(p) > THRESHOLD){
    //if condition to check if the point can be added starts

        /* if the path array does not have any points , then an input should be taken whatever
        the given point input maybe . Or if there are initially some point(s) in the array path ,
        then take the input only if the point is at a minmum distance (threshold distance- defined globally)
        from the previous point which is the last element in the array path and hence the distance of p is
         taken from the point which is at the [path.size]th position in the array ( [path.size-1] th element). */

        //cout<<"addPoint : "<<x<<" "<<y<<endl;
        path.push_back(p);
        // if point is allowed to be taken from earlier step , it is added to the array path

        int i=path.size();
        if(i==3){
            // if there are only three points in the array path then the element in the centres array
            // will take the circumcenter of the first three points input

            centres.push_back(circumcenter(path[0], path[1], path[2]));
            // circumcenter of first three points is added to centres

            centres.push_back(centres[0]);
            // the same point is once more added to the array centres so as to now set centres[i] as the
            // centre of the arc joining points path[i] and path[i+1]

            Arc arc0(path[0],path[1],centres[0],MINOR_ARC), arc1(path[1],path[2],centres[1],MINOR_ARC);
            // The part of the circumcircle of points path[0] , path[1] and path[2]
            // is broken into 2 continuous arcs between 0 and 1 and 1 and 2 respectively and
            // these 2 arcs are then added to the array arcs
            arcs.push_back(arc0); arcs.push_back(arc1);

        } else if(i>3 && i-1>centres.size()){
            /* If the number of points inputed in the array path are more than 3 , then an
               arc needs to be made joining the new added point and the last point in the array.
               The new point has been added to the array above and hence now is the [i-1]th element
               of the array path . So the current pint is path[i-1] and the last point before this is
               path[i-2]. Also the centre of the circle joining path[i-3] and path[i-2] is given by
               centres[i-3].
             */

            cout<<"Adding arc "<<i<<endl;

            Point p1=path[i-2], p2=path[i-1], c=centres[i-3];

            /* We want to make a  new arc passing through path[i-2] and path[i-1]
               which is also tangent to the previous arc - be it the circumcircle
               for i=4 or the arc joining path[i-3] and path[i-2] .
               For this specific arc , the tangent to arc arcs[i-3] and the arc to be made
               should be the same line . So the line joining path[i-2] to the center centres[i-3]
               when extended will conatin the center of the new arc to be formed centre[i-2] .
               Also the line joining the points path[i-2] and path[i-1] will be perpendicular
               to the line joining the midpoint of this segment with centre of the new arc centres.[i-2]
               (As the perpendicular bisector of a chord passes through the centre of the circle)
               we know 2 lines on which the centre of the new arc has to lie and by solving the equations
               of the 2 lines , we will get the coordinates of the centre of the new arc centres[i-2].
             */


            float A=(p1.y-c.y)/(p1.x-c.x),
                    B=(c.y*p1.x-c.x*p1.y)/(p1.x-c.x),
                    C=(p1.x-p2.x)/(p2.y-p1.y),
                    D=(modsq(p2)-modsq(p1))/(2*(p2.y-p1.y));
            /* y=Ax+B is the equation of the line joining the points path[i-2] and
               centres[i-3] .
               y=Cx+D is the equation of the perpendicular bisector of the line joining
               the points path[i-2] and path[i-1].
               Solving these 2 equations , centres[i-2] is (m,n) where
               m=(D-B)/(A-C), n=(D*A-B*C)/(A-C)
             */


            float x=(D-B)/(A-C), y=(D*A-B*C)/(A-C);

            if(p2.y==p1.y){ x=p1.x/2+p2.x/2; y=B+A*x; }

            /*If the y coordinates of the new point entered and the previous point are the same
            then the perpendicular bisector of this segement will be a line parallel to the y axis
            and from the above formula will have an undefined slope- infinity . So the perpendicular
            bisector is approximated by the above equation where we take the x coordinate of the midpoint
            of these 2 points and correspondingly find a y . */

            // cout<<"A:"<<A<<" B:"<<B<<" C:"<<C<<" D:"<<D<<endl;

            Point newc(x,y);
            //A new point in the centres array is added and it is initialised to having x and y coordinate
            // as the x and y that we have now calculated .

            centres.push_back(newc);
            // This point is added to the centres array

            Arc arc(p1,p2,newc,arcs[i-3].nextDirection(newc));
            /*An arc having initail and final points as path[i-2] and path[i-1] respectively
            is formed with the centre as the one just calculated and also the direction of this arc
            is found by using the nextDirection function */

            arcs.push_back(arc);
            //This arc is added to the array arcs
        }
        //else if adding the new centre and arc ends
    }
    // The if statement ends which was checking for whether the point could be added to the array or not
}
//The function addPoint ends

vector<Point> obstacles;

void addObstacle(int arcno, double theta, double distance){
    Arc arc = arcs[arcno];

    Vector a(arc.centre, arc.start);
    a.rotate(theta*arc.direction);

    Point cur_pos = arc.centre+a;

    a.rotate(90*arc.direction);
    double mod = a.mod();
    a.i*=distance/mod;
    a.j*=distance/mod;

    Point obst = cur_pos+a;
    obstacles.push_back(obst);

    int i,j;
    for(i=0; i<arcs.size() ; i++){

        if(arcs[i].end.distance(obst) < OBSTACLE_RADIUS){

            for(j=i; j<arcs.size() && arcs[j].end.distance(obst) < OBSTACLE_RADIUS; j++);

            Arc a1=arcs[i], a2=arcs[j];
            Point new_start, new_end;

            { // Substitute a1
                Vector r1(a1.centre,a1.start), x(a1.centre,obst);
                float cos_a = (r1.i*x.i+r1.j*x.j) / r1.mod() /x.mod();

                float r=(r1.mod()*r1.mod()-OBSTACLE_RADIUS*OBSTACLE_RADIUS - 2*r1.mod()*x.mod()*cos_a) /
                        2 / (r1.mod()+OBSTACLE_RADIUS-x.mod()*cos_a);

                Point new_centre=a1.start+r1*(-r/r1.mod());
                Vector v(new_centre,obst);
                new_end=new_centre+v*(r/v.mod());

                Arc alt_arc(a1.start, new_end, new_centre, a1.direction);

                arcs.insert(arcs.begin()+i, alt_arc);

                i++;j++;
            }

            { // Substitute a2
                Vector r1(a2.centre,a2.end), x(a2.centre,obst);
                float cos_a = (r1.i*x.i+r1.j*x.j) / r1.mod() /x.mod();

                float r=(r1.mod()*r1.mod()-OBSTACLE_RADIUS*OBSTACLE_RADIUS - 2*r1.mod()*x.mod()*cos_a) /
                        2 / (r1.mod()+OBSTACLE_RADIUS-x.mod()*cos_a);

                Point new_centre=a2.end+r1*(-r/r1.mod());
                Vector v(new_centre,obst);
                new_start=new_centre+v*(r/v.mod());

                Arc alt_arc(new_start, a2.end, new_centre, a2.direction);

                arcs.insert(arcs.begin()+j+1, alt_arc);
            }

            arcs.erase(arcs.begin()+i, arcs.begin()+j+1); // Remove arcs i ... j . These are inside the obstacle circle

            Arc around_obst(new_end,new_start,obst, a1.direction*-1);
            arcs.insert(arcs.begin()+i,around_obst);

            i+=2;
        }
    }
}
