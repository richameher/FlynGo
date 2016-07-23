#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>
#include <iostream>


float sealevel;
float polysize;
float x=0.0,y=0.0,z=0.3;
float upx=0.0,upy=0.0,upz=1.0;
float eye[3]={1,0,0.0};
int yaw=0,roll=0;
double angle = 0.0,angle2=0.0,angle3=0.0;
int pitch=0;
float head[3]={1,0,0.0};                        //vector is in the direction of the xaxis and it passes through the nose of the plane
float up[3]={0.0,0.0,1.0};                      //this is the up vector of the plane
float wing[3]={0.0,1.0,0.0};                    // this is the wing vector of the plane and is prallel to yaxis
int seed(float x, float y) {
    static int a = 1588635695, b = 1117695901;
    int xi = *(int *)&x;
    int yi = *(int *)&y;
    return ((xi * a) % b) - ((yi * b) % a);
}

void mountain(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float s)
{
    float x01,y01,z01,x12,y12,z12,x20,y20,z20;

    if (s < polysize) {
        x01 = x1 - x0;
        y01 = y1 - y0;
        z01 = z1 - z0;

        x12 = x2 - x1;
        y12 = y2 - y1;
        z12 = z2 - z1;

        x20 = x0 - x2;
        y20 = y0 - y2;
        z20 = z0 - z2;

        float nx = y01*(-z20) - (-y20)*z01;
        float ny = z01*(-x20) - (-z20)*x01;
        float nz = x01*(-y20) - (-x20)*y01;

        float den = sqrt(nx*nx + ny*ny + nz*nz);

        if (den > 0.0) {
            nx /= den;
            ny /= den;
            nz /= den;
        }

        glNormal3f(nx,ny,nz);
        glColor3f(0.23,0.34,1.0);
        glBegin(GL_TRIANGLES);
        glVertex3f(x0,y0,z0);
        glVertex3f(x1,y1,z1);
        glVertex3f(x2,y2,z2);
        glEnd();

        return;
    }

    x01 = 0.5*(x0 + x1);
    y01 = 0.5*(y0 + y1);
    z01 = 0.5*(z0 + z1);

    x12 = 0.5*(x1 + x2);
    y12 = 0.5*(y1 + y2);
    z12 = 0.5*(z1 + z2);

    x20 = 0.5*(x2 + x0);
    y20 = 0.5*(y2 + y0);
    z20 = 0.5*(z2 + z0);

    s *= 0.5;

    srand(seed(x01,y01));
    z01 += 0.3*s*(2.0*((float)rand()/(float)RAND_MAX) - 1.0);
    srand(seed(x12,y12));
    z12 += 0.3*s*(2.0*((float)rand()/(float)RAND_MAX) - 1.0);
    srand(seed(x20,y20));
    z20 += 0.3*s*(2.0*((float)rand()/(float)RAND_MAX) - 1.0);

    mountain(x0,y0,z0,x01,y01,z01,x20,y20,z20,s);
    mountain(x1,y1,z1,x12,y12,z12,x01,y01,z01,s);
    mountain(x2,y2,z2,x20,y20,z20,x12,y12,z12,s);
    mountain(x01,y01,z01,x12,y12,z12,x20,y20,z20,s);
}

void init(void)
{
    GLfloat white[] = {1.0,1.0,1.0,1.0};
    GLfloat lpos[] = {0.0,1.0,0.0,0.0};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    glClearColor (0.8, 0.5, 1.0, 0.0);
    glShadeModel (GL_FLAT);
    glEnable(GL_DEPTH_TEST);

    sealevel = 0.0;
    polysize = 0.01;
}
void crossProduct(float v1[], float v2[], float vR[]) {
    vR[0] =   ( (v1[1] * v2[2]) - (v1[2] * v2[1]) );
    vR[1] = - ( (v1[0] * v2[2]) - (v1[2] * v2[0]) );
    vR[2] =   ( (v1[0] * v2[1]) - (v1[1] * v2[0]) );
}

void display(void)
{

    GLfloat tanamb[] = {0.2,0.15,0.1,1.0};
    GLfloat tandiff[] = {0.4,0.3,0.2,1.0};

    GLfloat seaamb[] = {0.0,0.0,0.7,1.0};
    GLfloat seadiff[] = {0.0,0.0,0.8,1.0};
    GLfloat seaspec[] = {0.8,0.5,1.0,1.0};





    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);
    glLoadIdentity ();             /* clear the matrix */
    /* viewing transformation  */

    gluLookAt (0.2,0,0.4,eye[0]+head[0], eye[1]+head[1], eye[2]+head[2],up[0], up[1], up[2]);
    // glRotatef(angle, 1.0, 0.0, 0.0);
    glTranslatef(x,y-0.6, z);      /* modeling transformation */


    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tanamb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tandiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tandiff);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 15.0);


    mountain(0.0,0.0,0.0, 1.0,0.0,0.0, 0.0,1.0,0.0, 1.0);
    mountain(1.0,1.0,0.0, 0.0,1.0,0.0, 1.0,0.0,0.0, 1.0);


    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, seaamb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, seadiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, seaspec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);

    glNormal3f(0.0,0.0,1.0);
    glBegin(GL_QUADS);
    glVertex3f(0.0,0.0,sealevel);
    glVertex3f(1.0,0.0,sealevel);
    glVertex3f(1.0,1.0,sealevel);
    glVertex3f(0.0,1.0,sealevel);
    glEnd();


    glutSwapBuffers();
    glFlush ();
    if(yaw==1)

    {                                                       //partially implements yaw
        float p,q,r,s;
        p=wing[0],q=wing[1];
        wing[0]=cos(angle)*p-sin(angle)*q;
        wing[1]=sin(angle)*p+cos(angle)*q;
        r=head[0],s=head[1];
        head[0]=cos(angle)*r-sin(angle)*s;
        head[1]=sin(angle)*r+cos(angle)*s;



    }


    if(roll==1)                                            //rolls left if left key pressed
    {

        float p,q,r,s;
        p=up[1];q=up[2],r=wing[1],s=wing[2];          //rotates wing vector and up vector about the head vector xaxis
        wing[1]=cos(angle2)*r+sin(angle2)*s;
        wing[2]=-sin(angle2)*r+cos(angle2)*s;
        up[1]=cos(angle2)*p+sin(angle2)*q;
        up[2]=-sin(angle2)*p+cos(angle2)*q;
        crossProduct(wing, up, head);                  // updates head vector after every roll require for testing whether head is in the +ve x or -ve x direction

    }
    if(pitch==1)                                        //goes upward if up key pressed
    {
        float w,u,p,q;
        w=up[0];u=up[2];
        p=head[0];q=head[2];
        head[0]=cos(angle3)*p-sin(angle3)*q;        //rotates head vector and up vector about the wing vector/ xaxis

        head[2]=sin(angle3)*p+cos(angle3)*q;
        up[0]=cos(angle3)*w-sin(angle3)*u;
        up[2]=sin(angle3)*w+cos(angle3)*u;
        crossProduct(up,head,wing);        // updates wing vector after every roll require for testing whether head is in the +ve x or -ve y direction
    }
    if(roll==2)                                          //rolls right if right key pressed
    {float p,q,r,s;
        p=up[1];q=up[2],r=wing[1],s=wing[2];
        wing[1]=cos(angle2)*r-sin(angle2)*s;        //rotates wing vector and up vector about the head vector or xaxis
        wing[2]=sin(angle2)*r+cos(angle2)*s;
        up[1]=cos(angle2)*p-sin(angle2)*q;
        up[2]=sin(angle2)*p+cos(angle2)*q;
        crossProduct(wing, up, head);      // updates head vector after every roll require for testing whether head is in the +ve x or -ve x direction
    }

    if(pitch==2)                                        //goes downward if down key pressed
    {
        float w,u,p,q;
        w=up[0];u=up[2];
        p=head[0];q=head[2];
        head[0]=cos(angle3)*p+sin(angle3)*q;    //rotates head vector and up vector about the wing vector/yaxis

        head[2]=-sin(angle3)*p+cos(angle3)*q;
        up[0]=cos(angle3)*w+sin(angle3)*u;
        up[2]=-sin(angle3)*w+cos(angle3)*u;
        crossProduct(up,head,wing);         // updates wing vector after every roll require for testing whether head is in the +ve x or -ve y direction
    }

    //These conditions are base on the direction of wing vector and up vector and incremented accordingly since the rotation of the plane is only in x-z plane, only these directions are incremented


    if((wing[1]>0 && up[2]>0) || (wing[2]>0 && up[1]<0)||(wing[1]<0 && up[2]<0 )||(up[1]>0 && wing[2]<0))
    {
        x=x-0.0005;


    }
    else if((wing[1]>0 && up[2]<0) || (wing [2]<0 && up[1]<0)||(wing [1]<0 && up[2]>0)||( up[1]>0 && wing[2]>0))
    {
        x=x+0.0005;


    }
    if(head[2]!=0)
    { if(head[2]>0)
        z=z-0.000001;
    else if(head[1]<0)
        z=z+0.000001;
    }

    glutPostRedisplay();



}




void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(90.0,1.0,0.01,10.0);
    glMatrixMode (GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case '-':
            sealevel -= 0.01;
            break;
        case '+':
        case '=':
            sealevel += 0.01;
            break;
        case 'f':
            polysize *= 0.5;
            break;
        case 'c':
            polysize *= 2.0;
            break;
        case 27:
            exit(0);
            break;
        case 'r':
            angle+=0.001;
            yaw=1;
            break;
        case '0':
            exit(0);                    //case 0 exits the program
            break;


    }
}
void spkey(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_LEFT:                         //positive direction roll /roll left
            roll=1;
            angle2+=0.001;
            break;
        case GLUT_KEY_RIGHT:                        //-ve direction roll /roll left
            roll=2;
            angle2+=0.001;
            break;
        case GLUT_KEY_UP:                           // pitch upwards
            pitch=1;
            angle3+=0.001;
            break;
        case GLUT_KEY_DOWN:                          // pitch downwards
            pitch=2;
            angle3+=0.001;
            break;

    }
}

void sprel(int key,int x,int y)         //pitch and roll occurs as long as key is pressed
{
    switch(key)
    {
        case GLUT_KEY_LEFT:
            roll=0;

            break;
        case GLUT_KEY_UP:
            pitch=0;
            break;
        case GLUT_KEY_RIGHT:
            roll=0;

            break;

        case GLUT_KEY_DOWN:
            pitch=0;

            break;
    }

}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (1000, 1000);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);                 //normal key function
    glutSpecialFunc(spkey);                 //function for special keys like up,down arrows
    glutSpecialUpFunc(sprel);               //special release key function
    glutMainLoop();
    return 0;
}
