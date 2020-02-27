#include<windows.h>
#include<stdio.h>
#include<math.h>
#ifdef __APPLE__
#else
#include <GL/glut.h>
#endif
#include "imageloader.h"
using namespace std;

///all variables
GLuint _textureIdSphereWall;
GLuint _textureIdGlobeFloor;
GLuint _textureIdDay;
GLuint _textureIdNight;
GLuint _textureIdGround;
GLuint _textureIdPiller;
GLuint _textureIdTreeBody;
GLuint _textureIdGrass;
GLuint _textureIdGlassWall;
GLuint _textureIdWhite;
GLuint _textureIdName;
GLuint _textureIdRoad1;
GLuint _textureIdRoad2;

int WindowHeight = 800;
int WindowWidth = 800;
int night = 0;
int light = 1;
int fogflag=0;
int bottomflag=0;
int cornerflag=0;
float posL1=0.8f,posL2=0.8f,posL3=0.8f;
float R=0,G=0,B=0;
float skyAngle = 360;
///initial view
float tb_angle = 0,lr_angle = 0;
float view_x=0,view_y=0.49,view_z=-27;
///


///set wave
GLuint leaf_tex[10];
int leaf_tex_cnt=4, current_leaf_tex=0, leaf_tex_change_time=250;

GLuint flag_tex[10];
int flag_tex_cnt=9, current_flag_tex=0, flag_tex_change_time=250;
///


GLuint loadTexture(Image* image)
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 image->width, image->height,
                 0,//image border
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 image->pixels);
    return textureId;
}

float angle(float deg)
{
    float r = (deg / 180.0f) * 3.141592653589793f;
    return r;
}

void draw_cylinder(float cylinder_radius,float cylinder_height,float slice_count, GLuint _texture)
{
    // Wheel
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glColor3f(1,1,1);
    // angle
    float ttl_angle, angle_per_slice=360.0f/slice_count;
    // points
    float x, x_back=cylinder_radius;
    float y, y_back=0.0f;
    for(int i=1; i<=slice_count; i++)
    {
        // calculate coordinates
        ttl_angle = i*angle_per_slice;
        x = cylinder_radius * cos(angle(ttl_angle));
        y = cylinder_radius * sin(angle(ttl_angle));


        glBegin(GL_TRIANGLES);
        glTexCoord2f(0.5f+x*0.5f,0.5f+y*0.5f);
        glNormal3f(x,y,cylinder_height/2.0f);
        glVertex3f(x,y,cylinder_height/2.0f);

        glTexCoord2f(0.5f+x_back*0.5f,0.5f+y_back*0.5f);
        glNormal3f(x_back,y_back,cylinder_height/2.0f);
        glVertex3f(x_back,y_back,cylinder_height/2.0f);

        glTexCoord2f(0.5f+0.0f,0.5f+0.0f);
        glNormal3f(0.0f,0.0f,cylinder_height/2.0f);
        glVertex3f(0.0f,0.0f,cylinder_height/2.0f);
        glEnd();

        glBegin(GL_QUADS);
        glNormal3f(x,y,cylinder_height/2.0f);
        glVertex3f(x,y,cylinder_height/2.0f);
        glNormal3f(x_back,y_back,cylinder_height/2.0f);
        glVertex3f(x_back,y_back,cylinder_height/2.0f);
        glNormal3f(x_back,y_back,-cylinder_height/2.0f);
        glVertex3f(x_back,y_back,-cylinder_height/2.0f);
        glNormal3f(x,y,-cylinder_height/2.0f);
        glVertex3f(x,y,-cylinder_height/2.0f);
        glEnd();


        glBegin(GL_TRIANGLES);

        glTexCoord2f(0.5f+x*0.5f,0.5f+y*0.5f);
        glNormal3f(x,y,-cylinder_height/2.0f);
        glVertex3f(x,y,-cylinder_height/2.0f);

        glTexCoord2f(0.5f+x_back*0.5f,0.5f+y_back*0.5f);
        glNormal3f(x_back,y_back,-cylinder_height/2.0f);
        glVertex3f(x_back,y_back,-cylinder_height/2.0f);

        glTexCoord2f(0.5f+0.0f,0.5f+0.0f);
        glNormal3f(0.0f,0.0f,-cylinder_height/2.0f);
        glVertex3f(0.0f,0.0f,-cylinder_height/2.0f);
        glEnd();

        x_back = x;
        y_back = y;

    }
    glPopMatrix();
}


void initRendering()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);



    Image* image = loadBMP("novotheater_globe.bmp");
    _textureIdSphereWall = loadTexture(image);
    delete image;


    image = loadBMP("globe_floor.bmp");
    _textureIdGlobeFloor = loadTexture(image);
    delete image;


    image = loadBMP("day.bmp");
    _textureIdDay = loadTexture(image);
    delete image;

    image = loadBMP("night.bmp");
    _textureIdNight = loadTexture(image);
    delete image;

    image = loadBMP("ground.bmp");
    _textureIdGround = loadTexture(image);
    delete image;

    image = loadBMP("grass.bmp");
    _textureIdGrass = loadTexture(image);
    delete image;

    image = loadBMP("wall.bmp");
    _textureIdPiller = loadTexture(image);
    delete image;

    image = loadBMP("white.bmp");
    _textureIdWhite = loadTexture(image);
    delete image;

    image = loadBMP("glass_wall.bmp");
    _textureIdGlassWall = loadTexture(image);
    delete image;

    image = loadBMP("tree_body.bmp");
    _textureIdTreeBody = loadTexture(image);
    delete image;

    image = loadBMP("name.bmp");
    _textureIdName = loadTexture(image);
    delete image;

    image = loadBMP("road2.bmp");
    _textureIdRoad1 = loadTexture(image);
    delete image;

    image = loadBMP("road.bmp");
    _textureIdRoad2 = loadTexture(image);
    delete image;

    image = loadBMP("f1.bmp");
    flag_tex[0] = loadTexture(image);
    delete image;
    image = loadBMP("f2.bmp");
    flag_tex[1] = loadTexture(image);
    delete image;
    image = loadBMP("f3.bmp");
    flag_tex[2] = loadTexture(image);
    delete image;
    image = loadBMP("f4.bmp");
    flag_tex[3] = loadTexture(image);
    delete image;
    image = loadBMP("f5.bmp");
    flag_tex[4] = loadTexture(image);
    delete image;
    image = loadBMP("f6.bmp");
    flag_tex[5] = loadTexture(image);
    delete image;
    image = loadBMP("f7.bmp");
    flag_tex[6] = loadTexture(image);
    delete image;
    image = loadBMP("f8.bmp");
    flag_tex[7] = loadTexture(image);
    delete image;
    image = loadBMP("f9.bmp");
    flag_tex[8] = loadTexture(image);
    delete image;



    image = loadBMP("l1.bmp");
    leaf_tex[0] = loadTexture(image);
    delete image;
    image = loadBMP("l2.bmp");
    leaf_tex[1] = loadTexture(image);
    delete image;
    image = loadBMP("l3.bmp");
    leaf_tex[2] = loadTexture(image);
    delete image;
    image = loadBMP("l4.bmp");
    leaf_tex[3] = loadTexture(image);
    delete image;

}


void handleResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}
///SPHERE
void draw_sphere(int scalex, int scaley, GLfloat r)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdSphereWall);
    float M_PI=3.1416;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int i, j;
    GLfloat v[scalex*scaley][3];

    for (i=0; i<scalex; ++i)
    {
        for (j=0; j<scaley; ++j)
        {
            v[i*scaley+j][0]=r*cos(j*1.5*M_PI/scaley)*cos(i*M_PI/(1.5*scalex));
            v[i*scaley+j][1]=r*sin(i*M_PI/(1.5*scalex));
            v[i*scaley+j][2]=r*sin(j*1.5*M_PI/scaley)*cos(i*M_PI/(1.5*scalex));
        }
    }

    glBegin(GL_QUADS);
    for (i=0; i<scalex-1; ++i)
    {
        for (j=0; j<scaley; ++j)
        {
            glTexCoord2f(0,0);
            glVertex3fv(v[i*scaley+j]);
            glTexCoord2f(1,0);
            glVertex3fv(v[i*scaley+(j+1)%scaley]);
            glTexCoord2f(1,1);
            glVertex3fv(v[(i+1)*scaley+(j+1)%scaley]);
            glTexCoord2f(0,1);
            glVertex3fv(v[(i+1)*scaley+j]);
        }
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
///STAIRS
void draw_frontRoundStair()
{
    glPushMatrix();
    glTranslatef(0,-6.5,-24);
    glRotatef(90,1,0,0);
    draw_cylinder(12,.5,100,_textureIdGlobeFloor);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-6,-25);
    glRotatef(90,1,0,0);
    draw_cylinder(11.5,1.0,100,_textureIdGlobeFloor);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-5.5,-26);
    glRotatef(90,1,0,0);
    draw_cylinder(11,1.5,100,_textureIdGlobeFloor);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-5.0,-27);
    glRotatef(90,1,0,0);
    draw_cylinder(10.5,2.0,100,_textureIdGlobeFloor);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-4.4,-28);
    glRotatef(90,1,0,0);
    draw_cylinder(10,2.5,100,_textureIdGlobeFloor);
    glPopMatrix();
    glEnd();

}
///ROUND PILLARS
void draw_pillers()
{
    //7 left pillars
    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(-29.5,-4,-10.0);
    glRotatef(90,1,0,0);
    draw_cylinder(1.0,14,10,_textureIdPiller);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(-25.5,-4,-12);
    glRotatef(90,1,0,0);
    draw_cylinder(1.0,14,10,_textureIdPiller);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(-21.5,-4,-14.5);
    glRotatef(90,1,0,0);
    draw_cylinder(1.0,14,10,_textureIdPiller);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(-17.5,-4,-17);
    glRotatef(90,1,0,0);
    draw_cylinder(1.0,14,10,_textureIdPiller);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(-13.5,-4,-19.8);
    glRotatef(90,1,0,0);
    draw_cylinder(1.0,14,10,_textureIdPiller);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(-10.5,-4,-21.5);
    glRotatef(90,1,0,0);
    draw_cylinder(1.0,14,10,_textureIdPiller);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(-7.5,-4,-24);
    glRotatef(90,1,0,0);
    draw_cylinder(1.0,14,10,_textureIdPiller);
    glPopMatrix();
    glEnd();


    //7 right pillars
    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(29.5,-4,-10.0);
    glRotatef(90,1,0,0);
    draw_cylinder(1.0,14,10,_textureIdPiller);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(25.5,-4,-12);
    glRotatef(90,1,0,0);
    draw_cylinder(1.0,14,10,_textureIdPiller);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(21.5,-4,-14.5);
    glRotatef(90,1,0,0);
    draw_cylinder(1.0,14,10,_textureIdPiller);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(17.5,-4,-17);
    glRotatef(90,1,0,0);
    draw_cylinder(1.0,14,10,_textureIdPiller);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(13.5,-4,-19.8);
    glRotatef(90,1,0,0);
    draw_cylinder(1.0,14,10,_textureIdPiller);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(10.5,-4,-21.5);
    glRotatef(90,1,0,0);
    draw_cylinder(1.0,14,10,_textureIdPiller);
    glPopMatrix();
    glEnd();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(7.5,-4,-24);
    glRotatef(90,1,0,0);
    draw_cylinder(1.0,14,10,_textureIdPiller);
    glPopMatrix();
    glEnd();

    ///left big pillar
    glPushMatrix();
    glTranslatef(-32,-1.8,-34);
    glRotatef(90,1,0,0);
    draw_cylinder(3,15,10,_textureIdPiller);
    glPopMatrix();
    glEnd();

    //round shape above pillar
    glPushMatrix();
    glTranslatef(-32,5.6,-34);
    glRotatef(129,-1,1,1);
    draw_cylinder(3.2,1,10,_textureIdSphereWall);
    glPopMatrix();
    glEnd();

    ///right big pillar
    glPushMatrix();
    glTranslatef(32,-1.8,-34);
    glRotatef(90,1,0,0);
    draw_cylinder(3,15,10,_textureIdPiller);
    glPopMatrix();
    glEnd();
    //round shape above pillar
    glPushMatrix();
    glTranslatef(32.1,5.6,-34);
    glRotatef(229,1,1,1);
    draw_cylinder(3.2,1,10,_textureIdSphereWall);
    glPopMatrix();
    glEnd();

}
///FLAG SET UP
void draw_flag()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, flag_tex[current_flag_tex]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glColor3f(1,1,1);
    glTranslatef(3,0.0,0.0);
    ///flag
    glBegin(GL_QUADS);
    glNormal3f(1,1,1);
    glTexCoord2f(0,0);
    glVertex3f(-2,0,0);
    glTexCoord2f(1,0);
    glVertex3f(-2,2.5,0);
    glTexCoord2f(1,1);
    glVertex3f(2.5,2.5,0);
    glTexCoord2f(0,1);
    glVertex3f(2.5,0,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    ///flag pillar
    glPushMatrix();
    glTranslatef(-2.4,-3,0);
    glRotatef(90,1,0,0);
    draw_cylinder(.4,12,10,_textureIdPiller);
    glPopMatrix();
    glEnd();
}

///ALL TREES
void draw_tree(int base,int height)
{
    ///tree body
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdTreeBody);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glColor3f(1,1,1);

    //body front part
    glBegin(GL_QUADS);
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0);
    glVertex3f(-base/2,0,base/2);
    glTexCoord2f(1,0);
    glVertex3f(-base/2,height-3,base/2);
    glTexCoord2f(1,1);
    glVertex3f(base/2,height-3,base/2);
    glTexCoord2f(0,1);
    glVertex3f(base/2,0,base/2);
    //body left part
    glNormal3f(1,0,0);
    glTexCoord2f(0,0);
    glVertex3f(-base/2,0,base/2);
    glTexCoord2f(1,0);
    glVertex3f(-base/2,height-3,base/2);
    glTexCoord2f(1,1);
    glVertex3f(-base/2,height-3,-base/2);
    glTexCoord2f(0,1);
    glVertex3f(-base/2,0,-base/2);

    //body right part
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0);
    glVertex3f(base/2,0,base/2);
    glTexCoord2f(1,0);
    glVertex3f(base/2,height-3,base/2);
    glTexCoord2f(1,1);
    glVertex3f(base/2,height-3,-base/2);
    glTexCoord2f(0,1);
    glVertex3f(base/2,0,-base/2);

    //body back part
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0);
    glVertex3f(-base/2,0,-base/2);
    glTexCoord2f(1,0);
    glVertex3f(-base/2,height-3,-base/2);
    glTexCoord2f(1,1);
    glVertex3f(base/2,height-3,-base/2);
    glTexCoord2f(0,1);
    glVertex3f(base/2,0,-base/2);

    glEnd();
    glDisable(GL_TEXTURE_2D);

    ///tree leaf
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, leaf_tex[current_leaf_tex]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glColor3f(1,1,1);

    //front part
    glBegin(GL_TRIANGLES);
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0);
    glVertex3f(0,height,0);
    glTexCoord2f(1,1);
    glVertex3f(-1*base,(height-10),2*base);
    glTexCoord2f(0,1);
    glVertex3f(1*base,(height-10),2*base);

    //back part
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0);
    glVertex3f(0,height,0);
    glTexCoord2f(1,1);
    glVertex3f(-1*base,(height-10),-2*base);
    glTexCoord2f(0,1);
    glVertex3f(1*base,(height-10),-2*base);

    //left part
    glNormal3f(1,0,0);
    glTexCoord2f(0,0);
    glVertex3f(0,height,0);
    glTexCoord2f(1,1);
    glVertex3f(-1*base,(height-10),-2*base);
    glTexCoord2f(0,1);
    glVertex3f(-1*base,(height-10),2*base);

    //right part
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0);
    glVertex3f(0,height,0);
    glTexCoord2f(1,1);
    glVertex3f(1*base,(height-10),-2*base);
    glTexCoord2f(0,1);
    glVertex3f(1*base,(height-10),2*base);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}

///BACK OF THE THEATER
void draw_back()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdPiller);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //front
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,0,0);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-2.0,-1.0,5.0);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-2.0,2.4,5.0);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(2.0,2.4,5.0);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(2.0,-1.0,5.0);
    glEnd();
    glPopMatrix();

    //back
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,0,0);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-2.0,-1.0,-5.0);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-2.0,2.4,-5.0);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(2.0,2.4,-5.0);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(2.0,-1.0,-5.0);
    glEnd();
    glPopMatrix();

    //top
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,0,0);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-2.0,2.4,5.0);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-2.0,2.4,-5.0);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(2.0,2.4,-5.0);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(2.0,2.4,5.0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdGlassWall);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //left
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,0,0);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-2.0,-1.0,5.0);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-2.0,-1.0,-5.0);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-2.0,2.4,-5.0);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-2.0,2.4,5.0);
    glEnd();
    glPopMatrix();

    //right
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,0,0);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(2.0,-1.0,5.0);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(2.0,-1.0,-5.0);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(2.0,2.4,-5.0);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(2.0,2.4,5.0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

///LEFT SIDE OF THE THEATER
void draw_leftBack()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdPiller);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glColor3f(1.0,1.0,1.0);

    //top
    glPushMatrix();
    glBegin(GL_QUADS);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-2.0,2.4,22.0);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-2.0,2.4,-5);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(5,2.4,-5);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(6,2.4,4.5);
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdGlassWall);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glColor3f(1.0,1.0,1.0);
    //left
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-2.0,2.4,14.0);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-2.0,2.4,-5.0);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-2.0,-2.4,-5.0);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-2.0,-2.4,14.0);
    glEnd();
    glPopMatrix();

    //front
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-2.0,-2.4,14.0);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-2.0,2.4,14.0);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(4,2.4,1.0);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(4,-2.4,1.0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(4,2.4,1.0);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(4,-2.4,1.0);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(4.0,-2.4,-3.0);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(4.0,2.4,-3.0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdWhite);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //front_bottom
    glPushMatrix();
    glTranslatef(0.0,-1.5,0.0);
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-2.0,-0.8,14.2);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-2.0,0.8,14.2);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(4,0.8,1.2);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(4,-0.8,1.2);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0,-1.5,3.58);
    glScalef(1.0,1.0,-2.0);
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-2.01,0.8,14.3);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-2.01,0.8,-5.3);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-2.01,-0.8,-5.3);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-2.01,-0.8,14.3);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

///RIGHT SIDE OF THE THEATER
void draw_rightBack()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdPiller);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glColor3f(1.0,1.0,1.0);

    //top
    glPushMatrix();
    glBegin(GL_QUADS);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(2.0,2.4,22);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(2.0,2.4,-5.0);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-5,2.4,-5.0);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-6.0,2.4,4.5);
    glEnd();
    glPopMatrix();



    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdGlassWall);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glColor3f(1.0,1.0,1.0);
    //right
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(2.0,2.4,14.0);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(2.0,2.4,-5.0);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(2.0,-2.4,-5.0);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(2.0,-2.4,14.0);
    glEnd();
    glPopMatrix();

    //front
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(2.0,-2.4,14.0);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(2.0,2.4,14.0);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-4,2.4,1.0);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-4,-2.4,1.0);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-4,2.4,1.0);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-4,-2.4,1.0);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-4,-2.4,-3.0);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-4,2.4,-3.0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdWhite);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //front_bottom
    glPushMatrix();
    glTranslatef(0.0,-1.5,0.0);
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(2.0,-0.8,14.3);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(2.0,0.8,14.3);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-4,0.8,1.3);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-4,-0.8,1.3);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0,-1.5,3.58);
    glScalef(1.0,1.0,-2.0);
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(2.01,0.8,14.2);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(2.01,0.8,-5.2);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(2.01,-0.8,-5.2);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(2.01,-0.8,14.2);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


///MAIN GROUND with TREES and ROADS
void draw_ground()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdGround);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPushMatrix();
    glTranslatef(0,-0.24,5);
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(0,1,0);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-1000,0,1000);
    glTexCoord2f(10000.0f,0.0f);
    glVertex3f(1000,0,1000);
    glTexCoord2f(10000.0f,10000.0f);
    glVertex3f(1000,0,-1000);
    glTexCoord2f(0.0f,10000.0f);
    glVertex3f(-1000,0,-1000);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glPushMatrix();
    glTranslatef(-30,0,-50);
    draw_tree(3,25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-30,0,-30);
    draw_tree(3,20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-30,0,-10);
    draw_tree(3,19);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-30,0,10);
    draw_tree(3,20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-30,0,30);
    draw_tree(3,20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-30,0,50);
    draw_tree(3,20);
    glPopMatrix();


    ///////////////////////////////////////

    glPushMatrix();
    glTranslatef(-17,0,-40);
    draw_tree(3,20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10,0,-64);
    draw_tree(3,20);
    glPopMatrix();


    ///////////////////

    ///right

    glPushMatrix();
    glTranslatef(30,0,-50);
    draw_tree(3,25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(30,0,-30);
    draw_tree(3,20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(30,0,-10);
    draw_tree(3,19);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(30,0,10);
    draw_tree(3,20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(30,0,30);
    draw_tree(3,20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(30,0,50);
    draw_tree(3,20);
    glPopMatrix();

    ///////////////////////////////////////

    glPushMatrix();
    glTranslatef(17,0,-40);
    draw_tree(3,20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(10,0,-64);
    draw_tree(3,20);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdGrass);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPushMatrix();
    glTranslatef(0,-0.2,5);
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-35,0,60);
    glTexCoord2f(100.0f,0.0f);
    glVertex3f(35,0,60);
    glTexCoord2f(100.0f,100.0f);
    glVertex3f(35,0,-80);
    glTexCoord2f(0.0f,100.0f);
    glVertex3f(-35,0,-80);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdRoad1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    ///Front Road
    glPushMatrix();
    glTranslatef(0,-0.2,65);
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-40.0,0.0,80);
    glTexCoord2f(0.0f,2.0f);
    glVertex3f(-40.0,0.0,10);
    glTexCoord2f(10.0f,2.0f);
    glVertex3f(100.0,0.0,10);
    glTexCoord2f(10.0f,0.0f);
    glVertex3f(100.0,0.0,80);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdRoad2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    ///left road
    glPushMatrix();
    glTranslatef(0,-0.2,20);
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-70.0,0.0,-600);
    glTexCoord2f(2.0f,1.0f);
    glVertex3f(-39.5,0.0,-600);
    glTexCoord2f(2.0f,10.0f);
    glVertex3f(-39.5,0.0,400);
    glTexCoord2f(0.0f,10.0f);
    glVertex3f(-70.0,0.0,400);
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

}
///NAME STONE OF NOVO THEATER
void draw_nameStone()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-55,0,1);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-55,4,1);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-45,4,1);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-45,0,1);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureIdWhite);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(0,1,0);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-55,0,-1);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-55,4,-1);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-45,4,-1);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-45,0,-1);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-55,0,1);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-55,0,-1);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-55,4,-1);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-55,4,1);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-45,0,1);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-45,0,-1);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-45,4,-1);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-45,4,1);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glNormal3f(1,1,1);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-55,4,1);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-55,4,-1);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-45,4,-1);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-45,4,1);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

///DRAW ALL STRUCTURES
void draw_theater()
{
    glPushMatrix();
    glTranslatef(0.0f, -3.0f, -32.0f);
    glScalef(1.5,1.8,1.5);
    glRotatef(398,0,1,0);
    glNormal3f(1,1,1);
    draw_sphere(15,15,6.5);
    glPopMatrix();

    glPushMatrix();
    draw_frontRoundStair();
    glPopMatrix();

    glPushMatrix();
    draw_pillers();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0,-4.3,-45.0);
    glScalef(15.4,3.3,2.0);
    draw_back();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-23.8,-2.0,-30);
    glScalef(3.5,2.1,1.0);
    draw_leftBack();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(23.8,-2.0,-30);
    glScalef(3.5,2.1,1.0);
    draw_rightBack();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0,-7.0,-12.0);
    glScalef(3.0,1.0,1.0);
    draw_ground();
    glPopMatrix();

    glPushMatrix();
    draw_flag();
    glPopMatrix();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(0.0,-7.0,-18.0);
    glRotatef(90,1,0,0);
    draw_cylinder(50.0,0.5,100,_textureIdGround);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(0.0,-7.1,-18.0);
    glRotatef(90,1,0,0);
    draw_cylinder(70.0,0.5,100,_textureIdGrass);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(-35,-7,85);
    glRotatef(-35,0,1,0);
    draw_nameStone();
    glPopMatrix();

}

void drawScene()
{
    GLUquadric *quadric;
    quadric = gluNewQuadric();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear information from last draw
    glClearColor(R,G,B,1); //background color
    glMatrixMode(GL_MODELVIEW); //switch to the drawing perspective
    glLoadIdentity();//reset the drawing perspective
    glTranslatef(0,0,-10);//move forward 10 units

    GLfloat ambientColor[] = {0.5,0.5,0.5,1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);//ambient light of whole scene

    // Light 0
    GLfloat lightColor0[] = {posL1, posL2, posL3, 1.0f};
    GLfloat lightPos0[] = {0.0f, 8.0f, -4.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    ///whole scene rotation,translation
    glRotatef (tb_angle,1,0,0);//top-bottom rotation
    glTranslatef(view_x,view_y,view_z);//initial camera position
    glRotatef(lr_angle,0,1,0);//left-right rotation



    ///sky_day_night
    glPushMatrix();
    glRotatef(0.0,0.0,0.0,1.0);
    glTranslatef(1.40,-0.5,0.0);
    glRotatef(skyAngle,0.0,1.0,1.0);
    glColor3f(4.0, 4.0, 4.0);

    glEnable(GL_TEXTURE_2D);
    if (night == 1)
    {
        glBindTexture(GL_TEXTURE_2D, _textureIdNight);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, _textureIdDay);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluQuadricTexture(quadric, 50);
    gluSphere(quadric,100,80,80);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    ///fog
    if(fogflag==1)
    {
        GLfloat fogcolor[4]= {1.0f, 1.0f, 1.0f, 5};

        glFogfv(GL_FOG_COLOR,fogcolor);
        glFogf(GL_FOG_DENSITY,0.2);
        glFogi(GL_FOG_MODE,GL_LINEAR);
        glFogf(GL_FOG_START,2.0);
        glFogf(GL_FOG_END,50.0);
        glEnable(GL_FOG);
    }
    if(fogflag==0)
    {
        glDisable(GL_FOG);
    }
    glPushMatrix();
    glTranslatef(0.0,-0.5,7.5);
    glScalef(0.2,0.2,0.2);
    draw_theater();
    glPopMatrix();
    glutSwapBuffers();

    if(night==0)
    {
        glPushMatrix();
        glColor3f(1,1,1);
        glTranslated(4.97,4.55,70.49);
        glutSolidSphere(0.5,10,10);
        glPopMatrix();
    }
    if(night==1)
    {
        glPushMatrix();
        glColor3f(1,1,1);
        glTranslated(4.97,4.55,70.49);
        glutSolidSphere(0.5,10,10);
        glPopMatrix();
    }

}


void updateFlag(int val)
{
    current_flag_tex++;
    if(current_flag_tex >= flag_tex_cnt)
    {
        current_flag_tex = 0;
    }
    glutPostRedisplay();
    glutTimerFunc(flag_tex_change_time,updateFlag,0);
}

void updateTreeLeaf(int val)
{
    current_leaf_tex++;
    if(current_leaf_tex >= leaf_tex_cnt)
    {
        current_leaf_tex = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(leaf_tex_change_time,updateTreeLeaf,0);
}

void my_keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    ///light on and off
    case 'l':
        if(light == 1)
            light = 0,glDisable(GL_LIGHTING);
        else
            light = 1, glEnable(GL_LIGHTING);
        break;
    ///fog
    case 'f':
        if(fogflag == 1)
            fogflag = 0,glDisable(GL_FOG);
        else
            fogflag = 1, glEnable(GL_FOG);
        break;
    ///downward rotation
    case 'd':
        tb_angle-=4;
        if(tb_angle<4)
            tb_angle=5;
        break;
    ///upward rotation
    case 'u':
        tb_angle+=8;
        if(tb_angle>90)
            tb_angle=90;
        break;
    ///left rotation
    case 'q':
        lr_angle += 2.5f;
        if (lr_angle > 360)
        {
            lr_angle -= 360;
        }
        break;
    ///right rotation
    case 'w':
        lr_angle -= 2.5f;
        if (lr_angle < 360)
        {
            lr_angle += 360;
        }
        break;
    ///night mode
    case 'n':
        if(night == 0)
        {
            posL1=0.2f,posL2=0.2f,posL3=0.2f,night=1;
        }
        else
        {
            posL1=0.8f,posL2=0.8f,posL3=0.8f,night=0;
        }
        break;
    ///bottom view
    case 'b' :
        if(bottomflag==0)
        {
            tb_angle = 4;
            view_x=0,view_y=2,view_z=-19;
            lr_angle=33;
            bottomflag=1;
        }
        else if(bottomflag==1)
        {
            tb_angle = 4;
            view_x=-10,view_z=-84,view_y=0.49;
            lr_angle=33;
            bottomflag=2;

        }
        else
        {
            tb_angle = 4;
            view_x=0,view_z=-10,view_y=0.49;
            lr_angle=0;
            bottomflag=0;
        }

        break;
    ///top view
    case 't' :
        lr_angle=0;
        tb_angle = 80;
        view_x=0,view_z=-10,view_y=-40;
        break;
    ///upward
    case 'p':
        view_y+=.5;
        if(view_y>=0.49)
        {
            view_y=0.49;
        }
        break;
    ///downward
    case 'o':
        view_y-=.5;
        if(view_y<=-40)
        {
            view_y=-40;
        }
        break;
    ///corner view
    case 'c':
        if(cornerflag==0)
        {
            tb_angle = 4;
            view_x=0,view_z=-40,view_y=0.65;
            lr_angle=33;
            cornerflag=1;
        }
        else if(cornerflag==1)
        {
            tb_angle = 4;
            view_x=0,view_z=-40,view_y=0.65;
            lr_angle=-40;
            cornerflag=2;
        }
        else if(cornerflag==2)
        {
            tb_angle = 4;
            view_x=0,view_z=-40,view_y=0.49;
            lr_angle=-120;
            cornerflag=3;
        }
        else
        {
            tb_angle = 4;
            view_x=0,view_z=-40,view_y=0.49;
            lr_angle=120;
            cornerflag=0;
        }
        break;
    ///initial view
    case 'a' :
        lr_angle=0;
        tb_angle = 0;
        view_x=0,view_z=-27,view_y=0.49;
        break;

        glutPostRedisplay();
    }
}

void special_key(int key,int x,int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT :
        view_x += .5;
        break;
    case GLUT_KEY_RIGHT:
        view_x -= .5;
        break;
    case GLUT_KEY_UP   :
        view_z += .5;
        break;
    case GLUT_KEY_DOWN :
        view_z -= .5;
        break;
    }
    glutPostRedisplay();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WindowWidth, WindowHeight);


    glutInitWindowSize(WindowWidth,WindowHeight);
    glutInitWindowPosition(50,50);

    glutCreateWindow("Bongobondhu Sheikh Mujibur Rahman Novo Theater");
    initRendering();

    glutDisplayFunc(drawScene);
    glutKeyboardFunc(my_keyboard);
    glutReshapeFunc(handleResize);
    glutSpecialFunc(special_key);
    glutTimerFunc(flag_tex_change_time,updateFlag,0);
    glutTimerFunc(flag_tex_change_time,updateTreeLeaf,0);
    glutPostRedisplay();
    glutMainLoop();
    return 0;
}
