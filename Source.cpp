#include <GL/glut.h>  
#include <math.h>
#include <cstdio>
#include <SOIL2.h>
#include <iostream>
using namespace std;
#include <fstream>
#include <time.h>
#include <vector>
#include <string>
#include <windows.h>
#include <MMSystem.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "winmm.lib")
#define pi 3.142857
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
GLuint textureID;
GLuint textureWall;
GLuint textureBox;
GLuint textureRoof;
GLuint textureBoxContainer;

struct Point
{
    int x,y, z;
};

struct Point p1;
struct Point p2;
struct Point p3;
struct Point p4;
struct Point p5;

void points() {

    p1.x = 20;
    p1.y = 3;
    p1.z = 10;

    p2.x = -20;
    p2.y = 3;
    p2.z = -10;

    p3.x = 10;
    p3.y = 3;
    p3.z = 8;
    
    p4.x = -18;
    p4.y = 3;
    p4.z = 19;
}

bool item1 = TRUE;
bool item2 = TRUE;
bool item3 = TRUE;
bool item4 = TRUE;

void loadContainer() {

    textureBoxContainer = SOIL_load_OGL_texture(
        "container.jpg",  // Replace with the path to your texture file
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
    );
    if (!textureBoxContainer) {
        printf("Texture loading failed: %s\n", SOIL_last_result());
    }
}
void loadBox() {
    textureBox = SOIL_load_OGL_texture(
        "box.jpg",  // Replace with the path to your texture file
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
    );
    if (!textureBox) {
        printf("Texture loading failed: %s\n", SOIL_last_result());
    }
}
void loadLand() {
    textureID = SOIL_load_OGL_texture(
        "floor2.jpg",  // Replace with the path to your texture file
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
    );
    if (!textureID) {
        printf("Texture loading failed: %s\n", SOIL_last_result());
    }
}

void loadRoof() {
    textureWall = SOIL_load_OGL_texture(
        "wall2.jpg",  // Replace with the path to your texture file
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
    );
    if (!textureWall) {
        printf("Texture loading failed: %s\n", SOIL_last_result());
    }
}

void loadWall() {
    textureRoof = SOIL_load_OGL_texture(
        "roof.jpg",  // Replace with the path to your texture file
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
    );
    if (!textureRoof) {
        printf("Texture loading failed: %s\n", SOIL_last_result());
    }
}
class Model {
private:
    class Face {
    public:
        int edge;
        int* vertices;
        int* texcoords;
        int normal;

        Face(int edge, int* vertices, int* texcoords, int normal = -1) {
            this->edge = edge;
            this->vertices = vertices;
            this->texcoords = texcoords;
            this->normal = normal;
        }
    };
    std::vector<float*> vertices;
    std::vector<float*> texcoords;
    std::vector<float*> normals;
    std::vector<Face> faces;
    GLuint list;
public:
    void load(const char* filename) {
        std::string line;
        std::vector<std::string> lines;
        std::ifstream in(filename);
        if (!in.is_open()) {
            printf("Cannot load model %s\n", filename);
            return;
        }
        while (!in.eof()) {
            std::getline(in, line);
            lines.push_back(line);
        }
        in.close();
        float a, b, c;
        for (std::string& line : lines) {
            if (line[0] == 'v') {
                if (line[1] == ' ') {
                    sscanf_s(line.c_str(), "v %f %f %f", &a, &b, &c);
                    vertices.push_back(new float[3]{ a, b, c });
                }
                else if (line[1] == 't') {
                    sscanf_s(line.c_str(), "vt %f %f", &a, &b);
                    texcoords.push_back(new float[2]{ a, b });
                }
                else {
                    sscanf_s(line.c_str(), "vn %f %f %f", &a, &b, &c);
                    normals.push_back(new float[3]{ a, b, c });
                }
            }
            else if (line[0] == 'f') {
                int v0, v1, v2, t0, t1, t2, n;
                sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n);
                int* v = new int[3]{ v0 - 1, v1 - 1, v2 - 1 };
                faces.push_back(Face(3, v, NULL, n - 1));
            }
        }
        list = glGenLists(1);
        glNewList(list, GL_COMPILE);
        for (Face& face : faces) {
            if (face.normal != -1)
                glNormal3fv(normals[face.normal]);
            else
                glDisable(GL_LIGHTING);
            glBegin(GL_POLYGON);
            for (int i = 0; i < face.edge; i++)
                glVertex3fv(vertices[face.vertices[i]]);
            glEnd();
            if (face.normal == -1)
                glEnable(GL_LIGHTING);
        }
        glEndList();
        //printf("Model: %s\n", filename);
        //printf("Vertices: %d\n", vertices.size());
        //printf("Texcoords: %d\n", texcoords.size());
        //printf("Normals: %d\n", normals.size());
        //printf("Faces: %d\n", faces.size());
        for (float* f : vertices)
            delete f;
        vertices.clear();
        for (float* f : texcoords)
            delete f;
        texcoords.clear();
        for (float* f : normals)
            delete f;
        normals.clear();
        faces.clear();
    }
    void draw() { glCallList(list); }
};

Model model;

//camera variables
GLfloat camX = 0, camY = 5, camZ = 0;

//variables to move the scene
GLfloat sceRX = 0.0; GLfloat sceRY = 0.0; GLfloat sceRZ = 0.0;
GLfloat sceTX = 0.0; GLfloat sceTY = 0.0; GLfloat sceTZ = 0.0;

////variables to move the objects
GLfloat objRX = 0.0; GLfloat objRY = 0.0; GLfloat objRZ = 0.0;
GLfloat objTX = 0.0; GLfloat objTY = 0.0; GLfloat objTZ = 0.0;

float pos_x, pos_y, pos_z;
float angle_x = 30.0f, angle_y = 0.0f;        

int x_old = 0, y_old = 0;
int current_scroll = 5;
float zoom_per_scroll;

bool is_holding_mouse = false;
bool is_updated = false;
float sensitivity = 0.2;
float velocity_y = 0.0f; // Initial velocity
bool jumping = false;

void mouse(int button, int state, int x, int y) {
    is_updated = true;

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            x_old = x;
            y_old = y;
            is_holding_mouse = true;
          


        }
        else
            is_holding_mouse = false;
    }



}


void motion(int x, int y) {
    if (is_holding_mouse) {
        is_updated = true;


        angle_y += (x - x_old) * sensitivity;
        x_old = x;
        if (angle_y > 360.0f)
            angle_y -= 360.0f;
        else if (angle_y < 0.0f)
            angle_y += 360.0f;

        angle_x += (y - y_old) * sensitivity;
        y_old = y;
        if (angle_x > 90.0f)
            angle_x = 90.0f;
        else if (angle_x < -90.0f)
            angle_x = -90.0f;

    }

    if (angle_x >= 12) {
        angle_x = 12;
    }


    else if (angle_x <= -6){
        angle_x =-6;
    }
    if (angle_y >= 130) {
        angle_y = 130;
    }
    else if (angle_y <= 45) {
        angle_y = 45;
    }
    // change the value as necessary 
    printf("%f ", angle_x);
    //printf("%f ", angle_y);

}
void render_text(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glClearDepth(1.0);
    loadBox();
    loadWall();
    loadLand();
    loadRoof();
    loadContainer();
    model.load("deagle.obj");
    points();
}
void drawGrid() {
    GLint line;
    GLfloat step = 1.0f;

    glLineWidth(2);
    glBegin(GL_LINES);

    for (line = -100; line <= 100; line += step) {
        glVertex3f(line, 0, 100);
        glVertex3f(line, 0, -100);

        glVertex3f(100, 0, line);
        glVertex3f(-100, 0, line);
    }
    glEnd();

}

void drawAxes() {
    glBegin(GL_LINES);
    glColor3f(1.0, 0, .0);
    glVertex3f(0, 0, 20);
    glVertex3f(0, 0, -20);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 1.0, 0);
    glVertex3f(0, 10, 0);
    glVertex3f(0, -10, -0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 0, 1.0);
    glVertex3f(-10, 0, 0);
    glVertex3f(10, 0, 0);
    glEnd();
}

void drawWall() {
    glBindTexture(GL_TEXTURE_2D, textureWall);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(25, 0, 20);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25, 0, -20);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(25, 10, -20);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(25, 10, 20);

    glEnd();

    //back
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25, 0, -20);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25, 0, -20);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(25, 10, -20);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25, 10, -20);

    glEnd();

    //left
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25, 0, -20);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-25, 0, 20);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-25, 10, 20);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25, 10, -20);

    glEnd();

    //front
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25, 0, 20);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25, 0, 20);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(25, 10, 20);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25, 10, 20);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawFloor() {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_POLYGON);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-25, 0, 20);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25, 0, 20);

    glTexCoord2f(1.0f, 1.0f); glVertex3f(25, 0, -20);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25, 0, -20);
    glEnd();
    glDisable(GL_TEXTURE_2D);


}

void drawRoof() {
    glBindTexture(GL_TEXTURE_2D, textureRoof);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);

    glTexCoord2f(0.0f, 0.0f);  glVertex3f(-25, 10, 20);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(25, 10, 20);

    glTexCoord2f(1.0f, 1.0f); glVertex3f(25, 10, -20);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-25, 10, -20);
    glEnd();
    glDisable(GL_TEXTURE_2D);



}

void drawBox(int type=1) {
    glShadeModel(GL_SMOOTH);

    if (type == 1) {
        glBindTexture(GL_TEXTURE_2D, textureBox);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, textureBoxContainer);


    }
    //top
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1, 2, 1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1, 2, -1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1, 2, -1);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, 2, 1);
    glEnd();

    //left
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, 0, -1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1, 0, 1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1, 2, 1);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, 2, -1);
    glEnd();

    //front
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, 0, 1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1, 0, 1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 2, 1);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, 2, 1);
    glEnd();

    //right
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1, 0, 1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1, 0, -1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 2, -1);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1, 2, 1);
    glEnd();

    //back
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, 0, -1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1, 0, -1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 2, -1);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, 2, -1);
    glEnd();


    glDisable(GL_TEXTURE_2D);

}

void drawHeart() {
    glBegin(GL_POLYGON);
    for (float x = -1.139; x <= 1.139; x += 0.001)
    {
        float delta = cbrt(x * x) * cbrt(x * x) - 4 * x * x + 4;
        float y1 = (cbrt(x * x) + sqrt(delta)) / 2;
        float y2 = (cbrt(x * x) - sqrt(delta)) / 2;
        glVertex2f(x, y1);
        glVertex2f(x, y2);
    }
    glEnd();
}

void pyramid() {

    glBegin(GL_POLYGON);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 0, -1);
    glVertex3f(-1, 0, -1);
    glVertex3f(-1, 0, 1);
    glEnd();


    glBegin(GL_POLYGON);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 0, -1);
    glVertex3f(0, 2, 0);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(1, 0, 1);
    glVertex3f(-1, 0, 1);
    glVertex3f(0, 2, 0);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(-1, 0, 1);
    glVertex3f(-1, 0, -1);
    glVertex3f(0, 2, 0);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(1, 0, -1);
    glVertex3f(-1, 0, -1);
    glVertex3f(0, 2, 0);
    glEnd();
}

void drawHouse() {
    // floor
    glPushMatrix();
    glColor3f(1, 1, 1);
    drawFloor();
    glPopMatrix();

    // wall
    glPushMatrix();
    glColor3f(0.8, 0.5, 0.1);
    drawWall();
    glPopMatrix();

    // roof
    glPushMatrix();
    glColor3f(0.8, 0.5, 0.1);
    drawRoof();
    glPopMatrix();

    // Draw box1
    glPushMatrix();
    glColor3f(0.1, 0.5, 0.3);
    glTranslated(1, 0, 1);
    glScaled(2, 1.2, 1.2);
    drawBox();
    glPopMatrix();

    // draw box 2
    glPushMatrix();
    glTranslated(10, 0, 15);
    glScaled(6, 3, 3);
    drawBox(2);
    glPopMatrix();

    // draw box 3
    glPushMatrix();
    glTranslated(20, 0, 10);
    glScaled(3, 1, 1);
    glRotatef(90, 0, 1, 0);
    drawBox();
    glPopMatrix();


    //box 4 on box3
    glPushMatrix();
    glTranslated(10, 6, 15);
    glScaled(3, 3, 3);
    glRotatef(60, 0, 1, 0);
    drawBox();
    glPopMatrix();
    
    //box 5
    glPushMatrix();
    glTranslated(-10, 0, -10);
    glScaled(6,3,3);
    drawBox(2);
    glPopMatrix();

    //box 6
    glPushMatrix();
    glTranslated(-10, 0, 10);
    glScaled(4, 1.5, 1.8);
    drawBox();

    glPopMatrix();


    //box 7
    glPushMatrix();
    glTranslated(10, 0, -10);
    glScaled(6, 3, 3);
    drawBox(2);
    glPopMatrix();

    

     //Lights 1
    glPushMatrix();
    glColor3d(1, 1, 1);
    glTranslated(-20.0, 10.0, 19.0);
    glScaled(4, 1, 1);
    glutSolidCube(0.2);
    glPopMatrix();

    //Lights 2
    glPushMatrix();
    glColor3d(1, 1, 1);
    glTranslated(20.0, 9.0, -15.0);
    glScaled(4, 1, 1);
    glutSolidCube(0.2);
    glPopMatrix();

    //Lights 3
    glPushMatrix();
    glColor3d(1, 1, 1);
    glTranslated(20.0, 9.0, -19.0);
    glScaled(4, 1, 1);
    glutSolidCube(0.2);
    glPopMatrix();
    //item1
    if (item1) {
        glPushMatrix();
        glColor3d(1, 1, 0);
        glTranslated(p1.x, p1.y, p1.z);
        glRotated(objRY, 0, 1, 0);
        glutSolidCube(1);
        glPopMatrix();
    }

    //item2
    if (item2) {
        glPushMatrix();
        glColor3d(1, 0, 0);
        glTranslated(p2.x, p2.y, p2.z);
        glRotated(objRY, 0, 1, 0);
        glScaled(0.8, 0.8, 0.8);
        drawHeart();
        glPopMatrix();
    }

    //item3
    if (item3) {
        glPushMatrix();
        glColor3d(1, 0, 0);
        glTranslated(p3.x, p3.y, p3.z);
        glRotated(objRY, 0, 1, 0);
        glScaled(0.8, 0.8, 0.8);
        //drawHeart();
        pyramid();
        glPopMatrix();
    }
    if (item4) {
        glPushMatrix();
        glColor3d(1, 0, 0);
        glTranslated(p4.x, p4.y, p4.z);
        glRotated(objRY, 0, 1, 0);
        glScaled(0.8, 0.8, 0.8);
        //drawHeart();
        pyramid();
        glPopMatrix();
    }
}

void drawGun(int radius,int height) {
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;

    float angle = 0.0;
    float stepSize =  2.0 * 3.142 / 50;

    glBegin(GL_QUAD_STRIP);
    while (angle < 2 * 3.142) {
        x = radius * cos(angle);
        z = radius * sin(angle);
        glVertex3f(x, y, z);
        glVertex3f(x, y + height, z);
        angle += stepSize;
    }

    //Last two points
    glVertex3f(radius, 0, 0);
    glVertex3f(radius, height, 0);



    glEnd();
}
void gun(int radius,int height) {
    glPushMatrix();
    drawGun(radius, height);
    glPopMatrix();

    glPushMatrix();
    glColor3d(0.2, 0.2, 0.2);
    glScaled(0.4, 0.4, 0.4);
    glTranslated(0, height - (height * 0.8), -radius*3);
    drawGun(radius, height);
    glPopMatrix();

}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 5, 5, 0, camY, 0, 0, 1, 0);

    /* set material parameters */
    const GLfloat blue[4] = { 0.356, 0.376, 0.396, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0f);

    /* positioned the light source 1 */
    GLfloat position0[] = { 20.0,9.0,-15.0,1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, position0);

    /* set light intensities for light source 1 */
    glLightfv(GL_LIGHT0, GL_DIFFUSE, blue);
    GLfloat white[] = { 1.0,1.0,1.0,1.0 };

    /* positioned the light source 2 */
    GLfloat position1[] = { 24.0,9.0,-19.0,1.0 };
    glLightfv(GL_LIGHT1, GL_POSITION, position1);

    /* set light intensities for light source 2 */
    glLightfv(GL_LIGHT1, GL_DIFFUSE, blue);

    /* positioned the light source 3 */
    GLfloat position2[] = { -20.0,10.0,19.0,1.0 };
    glLightfv(GL_LIGHT2, GL_POSITION, position2);

    /* set light intensities for light source 3 */
    glLightfv(GL_LIGHT2, GL_DIFFUSE, blue);


    glDisable(GL_NORMALIZE);
    //glDisable(GL_COLOR_MATERIAL);
    glPushMatrix();

    glRotatef(angle_x, 1.0f, 0.0f, 0.0f);
    glRotatef(angle_y, 0.0f, 1.0f, 0.0f);
    glTranslatef(pos_x, 3, pos_z);

    glPushMatrix();
    drawAxes();

   /* glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    drawGrid();
    glPopMatrix();*/

    glPushMatrix();
    drawHouse();
    glPopMatrix();


    //gun
    glPushMatrix();
    glColor3f(0.1, 0.2, 0.1);

    glTranslated(-pos_x-2, pos_y+1, -pos_z);
    glScaled(0.3, 0.3, 0.3);

    glRotatef(-angle_x, 1, 0, 0);

    glRotatef(-angle_y, 0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glScaled(0.1, 5, 0.1);
    gun(4,2);

    //model.draw();
    glPopMatrix();




    glPopMatrix();

    glutSwapBuffers();
    glFlush();
}

// animation timer function
void Timer(int v) {
    objRY += 3;

    if (objRY == 360) {
        objRY = 0;
    }
   
    glutPostRedisplay();
    glutTimerFunc(60, Timer, 0);
}


// keyboardSpecial
void keyboardSpecial(int key, int x, int y) {


    // camera up and down
  
 
    if (key == GLUT_KEY_UP) {
        if (pos_x >= 23) {
            pos_x = 22;
        }
        if (pos_x <= -23) {
            pos_x = -22;
        }
        else {
            pos_x += 1;
        }
    }
    if (key == GLUT_KEY_DOWN) {
        if (pos_x >= 23) {
            pos_x = 22;
        }
        if (pos_x <= -23) {
            pos_x = -22;
        }
        else {
            pos_x -= 1;
        }
    }

    // camera left and right
    if (key == GLUT_KEY_RIGHT) {
        if (pos_z <= -20) {
            pos_z = -19;
       }
        else {
            pos_z -= 1;
        }
        
    }
    if (key == GLUT_KEY_LEFT) {
      
        if (pos_z >= 20) {
            pos_z = 19;
        }
        else {
            pos_z += 1;
        }
        

    }


   
    glutPostRedisplay();
}


void update(int value) {

    
    // Update position based on velocity
    pos_y += velocity_y;
    camY += velocity_y;

    if (camY <= 5) {
        camY = 5;
    }
    // Simulate gravity
    velocity_y -= 0.01f;

    // Check if the character has reached the ground
    if (pos_y < 0.0f) {
        pos_y = 0.0f;
        velocity_y = 0.0f;
        jumping = false;
    }

    if (camY < 0.0f) {
        camY = 0.0f;
        velocity_y = 0.0f;
        jumping = false;
    }

    // Call update function again
    glutTimerFunc(60, update, 0);
}


// keyboard move object 
void keyboard(unsigned char key, int x, int y) {

 
    if (key == 'Z')
        sceTZ += 1;

    if (key == 'z')
        sceTZ -= 1;

    if (key == 'w')
        sceTX += 1;

    if (key == 's')
        sceTX -= 1;

    if (key == 'y')
        sceRY += 1;

    if (key == 'Y')
        sceRY -= 1;

    // Control light source 1
    if (key == '1') {
        glEnable(GL_LIGHT0);

    }
    if (key == '!') {
        glDisable(GL_LIGHT0);
    }
 

    if (key == 'c' && !jumping) {
        // Apply upward velocity for jumping
        velocity_y = 0.2f;
        jumping = true;
    }

    // Control light source 2
    if (key == '2') {
        glEnable(GL_LIGHT1);
    }
    if (key == '@') {
        glDisable(GL_LIGHT1);
    }
    if (key == '3') {
        glEnable(GL_LIGHT2);
    }
    if (key == '#') {
        glDisable(GL_LIGHT2);
    }

    if(key == 'f'){

        //item1
        float xx1 = abs(-pos_x - p1.x);
        float zz1 = abs(-pos_z - p1.z);
        if ((xx1 <=2) && (zz1 <=2)) {
            item1 = FALSE;
        }
        //item2
        float xx2 = abs(-pos_x - p2.x);
        float zz2 = abs(-pos_z - p2.z);
        if ((xx2 <= 2) && (zz2 <= 2)) {
            item2 = FALSE;
        }
        //item3
        float xx3 = abs(-pos_x - p3.x);
        float zz3 = abs(-pos_z - p3.z);
        if ((xx3 <= 2) && (zz3 <= 2)) {
            item3 = FALSE;
        }
        //item4
        float xx4 = abs(-pos_x - p4.x);
        float zz4 = abs(-pos_z - p4.z);
        printf("%f ", xx4);
        printf("%f ", zz4);

        if ((xx4 <= 2) && (zz4 <= 2)) {
            item4 = FALSE;
        }
    }

    glutPostRedisplay();
}


void reshape(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(50.0, aspect_ratio, 1.0, 100.0);

}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("Project");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // keyboard function activation
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(200, update, 0);

    glutTimerFunc(200, Timer, 0);

    glutMainLoop();

    return 0;
}