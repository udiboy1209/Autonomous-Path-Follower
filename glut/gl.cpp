#include "main.hpp"

#define DRAG 0
#define CLICK 1

int generate; //generate decides whether points are made by clicking on the canvas or by draging the mouse on the canvas

void save(){
    ofstream output_file("output.txt");

    if(! output_file.is_open()) return ;

    output_file<<arcs.size()<<endl;

    for(int i=0;i<arcs.size();i++){
        output_file<<int(arcs[i].radius)<<" "
                    <<int(arcs[i].theta/arcs[i].direction)<<" "
                    <<char(98-arcs[i].direction)
                    <<endl;

        if(!output_file.good()) return;
    }

    cout<<"Saved to output.txt"<<endl;

    output_file.close();
}

void clear(){
    centres.clear();
    path.clear();
    arcs.clear();
}

void menu(int num){
    if(num == 0){
        exit(0);
    } else if(num==1){
        clear();
    } else if(num==2){
        save();
    }
    glutPostRedisplay();
} 

void createMenu(void){ 
    glutCreateMenu(menu);

    glutAddMenuEntry("Clear", 1);
    glutAddMenuEntry("Save", 2);
    glutAddMenuEntry("Quit", 0);  
    glutAttachMenu(GLUT_RIGHT_BUTTON);
} 

void mouseClick(int button, int state, int x, int y){
    //button signifies which mouse button is pressed=left , right or centre
    //state tells whether that button is pressed or not
    x-=WINDOW_SIZE;
    y=WINDOW_SIZE-y;
    //reduces the co-ordinates of the point clicked with respect to the dimensions of the GLUT canvas
    
    if(generate==CLICK && button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        addPoint(x,y);
        //on pressing the left button and if clicking has been chosen as the option for 
        //points generation initially , then that specific point is to be added to the canvas
    }

    glutPostRedisplay();
    //implements whatever change has been done by the above function by either clearing 
    //the canvas or by drawing the new point on the canvas 
}

void mouseDrag(int x, int y){
    //cout<<"mouseDrag "<<x<<" "<<y<<endl;

    x-=WINDOW_SIZE;
    y=WINDOW_SIZE-y;
        //reduces the co-ordinates of the point clicked with respect to the dimensions of the GLUT canvas

    if(generate==DRAG){
        addPoint(x,y);
        // if generate was specified for drawing points on dragging ,then the current point will be added to the canvas

        glutPostRedisplay();
        // this point is drawn on the canvas 
    }
}

void display(){
    glClearColor(WHITE, 1.0f); 
    // Set Background to white ( R,G,B) and full transparency
    glClear(GL_COLOR_BUFFER_BIT);


    //glRasterPos2f(2*(WINDOW_SIZE/2-100)/WINDOW_SIZE , 2*(WINDOW_SIZE/2-50)/WINDOW_SIZE);
    //glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, "SAVE");

    //glBegin(GL_POINTS);
    //glColor3f(0.0f,0.0f,0.0f);
       for(int i=0; i<path.size(); i++){
        // for points which have been entered by the user = i points 
            //glVertex2f(path[i].x/WINDOW_SIZE, path[i].y/WINDOW_SIZE);

            if(arcs.size()>0 && i>0){
                arcs[i-1].draw();

                centres[i-1].draw(1, BLACK);
                // gives the command for adding the arc joining path[i-1] and path [i] 
                //and the center of the arc[i-1] to be added to the canvas
            }

            path[i].draw(3, RED);
            // the last points added is also drawn on the canvas.
            //the arc through this and the next point entered and the centre of this arc will be added in the next iteration 
            //cout<<path[i].x<<" "<<path[i].y<<endl;
       }

       for(int j=0; j<obstacles.size(); j++){
            obstacles[j].draw(10, GREEN, 0.5f);

            Vector x(OBSTACLE_RADIUS, 0);

            Arc a(obstacles[j]+x, obstacles[j]+x, obstacles[j], ANTICLOCKWISE);
            a.theta=360; a.sweep=3.6;
            a.draw();
       }
    //glEnd();
    glFlush();
    // executes this change on the canvas 
}

int main(int argc, char** argv){
    glutInit(&argc, argv);

    if(argc>1)
        generate = argv[1][0]-48;
    else 
        generate = CLICK; // Set as drag by default
    // takes the input 0 or 1 from the user on whether to draw points by 
    // dragging or clicking . The ASCII values of 0 and 1 are converted 
    // to 0 or 1 by subtracting 48 from it. 

    glutInitWindowSize(WINDOW_SIZE * 2, WINDOW_SIZE * 2);
    // initialises the GLUT canvas size 
    glutCreateWindow("Hello GLUT");
    //names the GLUT canvas
    glutDisplayFunc(display);
    // recalls the display function
    glutMotionFunc(mouseDrag);
    //motion id detected based on the mouse drag
    glutMouseFunc(mouseClick);
    //mouse function depends on which mouse button is clicked
    
    createMenu();

    glutMainLoop();

    return 0;
}
