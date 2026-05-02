#include <windows.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.1416
bool isRaining=0;
bool isNight=0;

GLfloat skyR=0.53f;
GLfloat skyG=0.81f;
GLfloat skyB=0.98f;
float rainY = 0.0f;

GLfloat boatX = 0.0f;

GLfloat sunY = 440.0f;
GLfloat moonY = 440.0f;

int dayState = 1;      // 1=day, 2=sunset, 3=night
int sunsetRun = 0;
int nightRun = 0;
int dayRun = 0;

GLfloat sunsetLevel = 0.0f;
GLfloat nightLevel = 0.0f;

//Boat

GLfloat defaultSpeed = 0.5f;
GLfloat boatSpeed = 0.5f;
int boatRun = 1;
int boatDir = 1;   // -1 = left, 1 = right

//Cloud
GLfloat cloudX = 0.0f;
GLfloat cloudSpeed = 0.3f;

//Bird
GLfloat birdX = 0.0f;
GLfloat birdSpeed = 0.6f;
GLfloat birdWing = 4.0f;
int wingUp = 1;

// Bottom realistic boat
GLfloat boat2X = 0.0f;
GLfloat boat2Speed = 0.5f;
GLfloat boat2DefaultSpeed = 0.5f;
int boat2Run = 1;
int boat2Dir = -1;

//fire
GLfloat fireScale = 1.0f;
int fireUp = 1;

// Plane globals
int planeActive = 0;
float planeX = 0.0f;
float planeDir = 1;       // 1 = left-to-right, -1 = right-to-left
float planeScale = 0.1f;  // starts small (far away), grows then shrinks
int planeGrowing = 1;
float planeY = 400.0f;

int lightningMode = 0;   // 0 = OFF, 1 = AUTO blinking
int lightningVisible = 0;
int lightningCounter = 0;

float boat3X     = 420.0f;
float boat3Y     = 112.0f;
float boat3Scale = 1.4f;
int   boat3State = 0;
float boat3Speed = 0.4f;

const float BOAT3_START_X = 420.0f;
const float BOAT3_START_Y = 112.0f;
const float BOAT3_START_S = 1.4f;
const float BOAT3_END_X   = 460.0f;   // slight rightward drift
const float BOAT3_END_Y   = 206.0f;
const float BOAT3_END_S   = 0.75f;    // stays clearly visible, not too small

void init()
{
    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);

}
// SAIFUL's Part

// Circle Drawing
void drawCircle(float cx, float cy, float r, float red, float green, float blue)
{
    glColor3f(red, green, blue);

    int triangleAmount = 40;
    float tp = 2.0f * PI;

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(cx, cy);   // center

    for (int i = 0; i <= triangleAmount; i++)
    {
        glVertex2f(
            cx + (r * cos(i * tp / triangleAmount)),
            cy + (r * sin(i * tp / triangleAmount))
        );
    }

    glEnd();
}

// Sun Drawing
void drawSun()
{
    if(isRaining)
        return;

    if(dayState != 3 || dayRun == 1)
        drawCircle(420, sunY, 22, 0.95f, 0.95f, 0.0f);
}


void drawMoon()
{
    if(isRaining)
        return;

    if (dayState == 3)
    {
        drawCircle(420, moonY, 18, 1.0f, 0.95f, 0.75f);
        drawCircle(428, moonY + 4, 16, 0.01f, 0.015f, 0.05f);
    }
}


//Cloud Drawing

void cloudCircle(float x, float y, float r)
{
    if(dayState == 3)   // night mode
        drawCircle(x, y, r, 0.25f, 0.25f, 0.27f);   // gray cloud
    else if(dayState == 2)   // sunset mode
        drawCircle(x, y, r, 1.0f, 0.85f, 0.65f);    // warm sunset cloud
    else   // day mode
        drawCircle(x, y, r, 1.0f, 1.0f, 1.0f);      // white cloud
}
void drawAllClouds()
{
    // Cloud 1
    cloudCircle(190, 420, 12);
    cloudCircle(204, 428, 14);
    cloudCircle(220, 420, 12);
    cloudCircle(206, 414, 13);

    // Cloud 2
    cloudCircle(50, 460, 12);
    cloudCircle(64, 468, 14);
    cloudCircle(80, 460, 12);
    cloudCircle(66, 454, 13);

    // Cloud 3
    cloudCircle(330, 455, 12);
    cloudCircle(344, 463, 14);
    cloudCircle(360, 455, 12);
    cloudCircle(346, 449, 13);
}

// Birds.
void drawBird(float x, float y, float s)
{
    glColor3f(0.10f, 0.10f, 0.10f);
    glLineWidth(2.0f);

    glBegin(GL_LINES);

    // left wing
    glVertex2f(x, y);
    glVertex2f(x + 10 * s, y + birdWing * s);

    // right wing
    glVertex2f(x + 10 * s, y + birdWing * s);
    glVertex2f(x + 20 * s, y);

    glEnd();

    glLineWidth(1.0f);
}

// All birds
void drawBirds()
{
    drawBird(330, 420, 1.0f);
    drawBird(347, 405, 0.8f);
    drawBird(314, 392, 0.9f);
    drawBird(355, 435, 0.7f);
    drawBird(300, 410, 0.75f);
}

//Lower ground

void drawLowerGround()
{
    if(dayState == 3)
        glColor3f(0.0f, 0.18f, 0.03f);
    else
        glColor3f(0.0f, 0.68f, 0.0f);

    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    glVertex2f(500, 0);
    glVertex2f(500, 110);
    glVertex2f(0, 110);
    glEnd();
}
//River

void drawRiver()
{
    glColor3f(0.56f, 0.86f, 0.90f);

    glBegin(GL_POLYGON);

    // Lower edge of the river
    glVertex2f(0,   110);
    glVertex2f(500, 110);

    // Upper edge of the river
    glVertex2f(500, 214);
    glVertex2f(0,   214);

    glEnd();
}

// Upper Ground

void drawUpperGround()
{
    if(dayState == 3)
        glColor3f(0.0f, 0.20f, 0.05f);
    else
        glColor3f(0.0f, 0.58f, 0.0f);

    glBegin(GL_POLYGON);
    glVertex2f(0, 214);
    glVertex2f(500, 214);
    glVertex2f(500, 276);
    glVertex2f(0, 276);
    glEnd();
}

void stoneType1()
{
    glBegin(GL_POLYGON);
    glVertex2f(0.0,  0.0);
    glVertex2f(9.0, -1.0);
    glVertex2f(14.0, 3.5);
    glVertex2f(16.0, 10.0);
    glVertex2f(10.0, 14.0);
    glVertex2f(3.0, 11.5);
    glVertex2f(-0.5, 5.0);
    glEnd();
}

void stoneType2()
{
    glBegin(GL_POLYGON);
    glVertex2f(0.0,  0.5);
    glVertex2f(12.0, -0.5);
    glVertex2f(18.0, 4.5);
    glVertex2f(21.0, 11.0);
    glVertex2f(14.0, 15.5);
    glVertex2f(4.5, 14.5);
    glVertex2f(-1.0, 7.0);
    glEnd();
}

void stoneType3()
{
    glBegin(GL_POLYGON);
    glVertex2f(0.0,  0.0);
    glVertex2f(7.5, -1.0);
    glVertex2f(12.5, 3.0);
    glVertex2f(14.5, 9.0);
    glVertex2f(9.0, 12.5);
    glVertex2f(2.5, 10.5);
    glVertex2f(-0.5, 4.5);
    glEnd();
}

void stoneType4()
{
    glBegin(GL_POLYGON);
    glVertex2f(0.0,  0.5);
    glVertex2f(5.0, -1.0);
    glVertex2f(14.0, 0.0);
    glVertex2f(20.0, 4.0);
    glVertex2f(22.5, 10.0);
    glVertex2f(18.5, 15.0);
    glVertex2f(11.0, 16.5);
    glVertex2f(4.0, 14.5);
    glVertex2f(-1.0, 7.5);
    glEnd();
}

void stoneType5()
{
    glBegin(GL_POLYGON);
    glVertex2f(0.0,  0.0);
    glVertex2f(10.0, -1.0);
    glVertex2f(15.0, 4.0);
    glVertex2f(17.5, 11.5);
    glVertex2f(11.5, 15.0);
    glVertex2f(3.5, 13.0);
    glVertex2f(-0.5, 5.5);
    glEnd();
}

void stoneType6()
{
    glBegin(GL_POLYGON);
    glVertex2f(0.0,  0.5);
    glVertex2f(7.0, -0.5);
    glVertex2f(16.0, -1.0);
    glVertex2f(22.0, 4.0);
    glVertex2f(24.5, 11.0);
    glVertex2f(20.0, 16.0);
    glVertex2f(12.5, 18.0);
    glVertex2f(5.0, 15.5);
    glVertex2f(-1.0, 7.0);
    glEnd();
}

void drawTopStones()
{
    if(dayState == 3)
        glColor3f(0.20f, 0.22f, 0.22f);
    else if(dayState == 2)
        glColor3f(0.55f, 0.50f, 0.45f);
    else
        glColor3f(0.47f, 0.47f, 0.47f);

    glPushMatrix(); glTranslatef(0,207,0);   stoneType3(); glPopMatrix();
    glPushMatrix(); glTranslatef(15,207,0);stoneType5(); glPopMatrix();
    glPushMatrix(); glTranslatef(35,207,0);  stoneType1(); glPopMatrix();
    glPushMatrix(); glTranslatef(50,207,0);stoneType4(); glPopMatrix();
    glPushMatrix(); glTranslatef(74,207,0);  stoneType2(); glPopMatrix();
    glPushMatrix(); glTranslatef(98,207,0);stoneType6(); glPopMatrix();
    glPushMatrix(); glTranslatef(124,207,0); stoneType3(); glPopMatrix();
    glPushMatrix(); glTranslatef(139,207,0);stoneType5(); glPopMatrix();
    glPushMatrix(); glTranslatef(160,207,0); stoneType1(); glPopMatrix();
    glPushMatrix(); glTranslatef(175,207,0);stoneType4(); glPopMatrix();
    glPushMatrix(); glTranslatef(199,207,0); stoneType2(); glPopMatrix();
    glPushMatrix(); glTranslatef(223,207,0);stoneType6(); glPopMatrix();
    glPushMatrix(); glTranslatef(249,207,0); stoneType3(); glPopMatrix();
    glPushMatrix(); glTranslatef(264,207,0);stoneType5(); glPopMatrix();
    glPushMatrix(); glTranslatef(285,207,0); stoneType1(); glPopMatrix();
    glPushMatrix(); glTranslatef(300,207,0);stoneType4(); glPopMatrix();
    glPushMatrix(); glTranslatef(324,207,0); stoneType2(); glPopMatrix();
    glPushMatrix(); glTranslatef(348,207,0);stoneType6(); glPopMatrix();
    glPushMatrix(); glTranslatef(374,207,0); stoneType3(); glPopMatrix();
    glPushMatrix(); glTranslatef(389,207,0);stoneType5(); glPopMatrix();
    glPushMatrix(); glTranslatef(410,207,0); stoneType1(); glPopMatrix();
    glPushMatrix(); glTranslatef(425,207,0);stoneType4(); glPopMatrix();
    glPushMatrix(); glTranslatef(449,207,0); stoneType2(); glPopMatrix();
    glPushMatrix(); glTranslatef(473,207,0);stoneType6(); glPopMatrix();
    glPushMatrix(); glTranslatef(496,207,0); stoneType1(); glPopMatrix();
}

void drawBottomStones()
{
    if(dayState == 3)
        glColor3f(0.18f, 0.20f, 0.20f);
    else if(dayState == 2)
        glColor3f(0.58f, 0.52f, 0.45f);
    else
        glColor3f(0.50f, 0.50f, 0.50f);

    glPushMatrix(); glTranslatef(1,103,0);    stoneType2(); glPopMatrix();
    glPushMatrix(); glTranslatef(20,104,0);   stoneType4(); glPopMatrix();
    glPushMatrix(); glTranslatef(45,103.5,0); stoneType1(); glPopMatrix();
    glPushMatrix(); glTranslatef(61,103,0);   stoneType5(); glPopMatrix();
    glPushMatrix(); glTranslatef(81,104,0);   stoneType3(); glPopMatrix();
    glPushMatrix(); glTranslatef(95,103,0);   stoneType6(); glPopMatrix();
    glPushMatrix(); glTranslatef(121,103.5,0);stoneType2(); glPopMatrix();
    glPushMatrix(); glTranslatef(144,104,0);  stoneType1(); glPopMatrix();
    glPushMatrix(); glTranslatef(160,103,0);  stoneType4(); glPopMatrix();
    glPushMatrix(); glTranslatef(184,103.5,0);stoneType3(); glPopMatrix();
    glPushMatrix(); glTranslatef(199,104,0);  stoneType5(); glPopMatrix();
    glPushMatrix(); glTranslatef(219,103,0);  stoneType2(); glPopMatrix();
    glPushMatrix(); glTranslatef(243,103.5,0);stoneType6(); glPopMatrix();
    glPushMatrix(); glTranslatef(269,104,0);  stoneType1(); glPopMatrix();
    glPushMatrix(); glTranslatef(285,103,0);  stoneType4(); glPopMatrix();
    glPushMatrix(); glTranslatef(310,103.5,0);stoneType3(); glPopMatrix();
    glPushMatrix(); glTranslatef(325,104,0);  stoneType5(); glPopMatrix();
    glPushMatrix(); glTranslatef(345,103,0);  stoneType2(); glPopMatrix();
    glPushMatrix(); glTranslatef(369,103.5,0);stoneType6(); glPopMatrix();
    glPushMatrix(); glTranslatef(395,104,0);  stoneType1(); glPopMatrix();
    glPushMatrix(); glTranslatef(410,103,0);  stoneType4(); glPopMatrix();
    glPushMatrix(); glTranslatef(435,103.5,0);stoneType3(); glPopMatrix();
    glPushMatrix(); glTranslatef(450,104,0);  stoneType5(); glPopMatrix();
    glPushMatrix(); glTranslatef(470,103,0);  stoneType2(); glPopMatrix();
    glPushMatrix(); glTranslatef(490,103.5,0);stoneType1(); glPopMatrix();
}

//Boat

void drawBoat()
{

    // Outer body of the boat
    glColor3f(0.0f, 0.0f, 0.0f);

// Left front part
    glBegin(GL_POLYGON);
    glVertex2f(88.4f, 199.6f);
    glVertex2f(97.7f, 181.0f);
    glVertex2f(107.0f, 168.6f);
    glVertex2f(107.0f, 181.0f);
    glEnd();

// Middle body part
    glBegin(GL_POLYGON);
    glVertex2f(107.0f, 181.0f);
    glVertex2f(107.0f, 168.6f);
    glVertex2f(125.6f, 165.5f);
    glVertex2f(144.2f, 168.6f);
    glVertex2f(147.3f, 181.0f);
    glEnd();

// Right front part
    glBegin(GL_POLYGON);
    glVertex2f(147.3f, 181.0f);
    glVertex2f(144.2f, 168.6f);
    glVertex2f(153.5f, 177.9f);
    glVertex2f(162.8f, 199.6f);
    glEnd();

// Main sail
    glColor3ub(255, 255, 153);
    glBegin(GL_POLYGON);
    glVertex2f(157.8f, 199.6f);
    glVertex2f(164.7f, 218.2f);
    glVertex2f(169.0f, 236.8f);
    glVertex2f(169.6f, 249.2f);
    glVertex2f(169.0f, 261.6f);
    glVertex2f(165.9f, 270.9f);
    glVertex2f(160.9f, 278.3f);
    glVertex2f(154.7f, 285.2f);
    glVertex2f(134.9f, 270.9f);
    glVertex2f(147.3f, 249.2f);
    glVertex2f(150.4f, 236.8f);
    glVertex2f(147.3f, 181.0f);
    glEnd();

// Vertical mast
    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
    glVertex2f(145.4f, 292.6f);
    glVertex2f(146.7f, 292.6f);
    glVertex2f(146.7f, 181.0f);
    glVertex2f(145.4f, 181.0f);
    glEnd();

// Right inner wood section
    glColor3f(0.55f, 0.27f, 0.0745f);
    glBegin(GL_POLYGON);
    glVertex2f(147.3f, 181.0f);
    glVertex2f(146.1f, 193.4f);
    glVertex2f(142.3f, 204.6f);
    glVertex2f(139.9f, 208.9f);
    glVertex2f(134.9f, 212.0f);
    glVertex2f(125.6f, 213.2f);
    glVertex2f(125.6f, 181.0f);
    glEnd();

// Left inner wood section
    glBegin(GL_POLYGON);
    glVertex2f(107.0f, 181.0f);
    glVertex2f(108.2f, 193.4f);
    glVertex2f(112.0f, 204.6f);
    glVertex2f(114.4f, 208.9f);
    glVertex2f(119.4f, 212.0f);
    glVertex2f(128.7f, 213.2f);
    glVertex2f(128.7f, 181.0f);
    glEnd();

// Rope and inner frame lines
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.2f);

    glBegin(GL_LINES);
    glVertex2f(112.0f, 204.6f);
    glVertex2f(154.7f, 285.2f);

    glVertex2f(112.0f, 204.6f);
    glVertex2f(134.9f, 270.9f);

    glVertex2f(108.2f, 193.4f);
    glVertex2f(146.1f, 193.4f);

    glVertex2f(112.0f, 204.6f);
    glVertex2f(142.3f, 204.6f);

    glVertex2f(128.7f, 213.2f);
    glVertex2f(128.7f, 181.0f);

    glVertex2f(119.4f, 212.0f);
    glVertex2f(119.4f, 181.0f);

    glVertex2f(138.0f, 210.8f);
    glVertex2f(138.0f, 181.0f);
    glEnd();

    glLineWidth(1.0f);
}

//Tent

void drawTent()
{
    // Body
    if(dayState == 3) // night
        glColor3ub(175, 70, 20);
    else if(dayState == 2) // sunset
        glColor3ub(245, 90, 25);
    else // day
        glColor3ub(247, 99, 29);

    glBegin(GL_POLYGON);
    glVertex2f(110.0f, 70.0f);
    glVertex2f(155.0f, 55.0f);
    glVertex2f(188.0f, 148.0f);
    glVertex2f(140.0f, 148.0f);
    glEnd();


    // Front
    if(dayState == 3)
        glColor3ub(150, 95, 45);
    else if(dayState == 2)
        glColor3ub(230, 160, 80);
    else
        glColor3ub(240, 184, 96);

    glBegin(GL_POLYGON);
    glVertex2f(155.0f, 55.0f);
    glVertex2f(212.0f, 60.0f);
    glVertex2f(188.0f, 148.0f);
    glEnd();


    //Light Strip
    if(dayState == 3)
        glColor3ub(210, 145, 70);
    else if(dayState == 2)
        glColor3ub(250, 190, 105);
    else
        glColor3ub(250, 198, 120);

    glBegin(GL_POLYGON);
    glVertex2f(155.0f, 55.0f);
    glVertex2f(160.0f, 55.0f);
    glVertex2f(193.0f, 144.0f);
    glVertex2f(188.0f, 148.0f);
    glEnd();


    //Dark Strip
    if(dayState == 3)
        glColor3ub(95, 55, 25);
    else if(dayState == 2)
        glColor3ub(145, 85, 35);
    else
        glColor3ub(160, 105, 45);

    glBegin(GL_POLYGON);
    glVertex2f(193.0f, 144.0f);
    glVertex2f(217.0f, 60.0f);
    glVertex2f(210.0f, 60.0f);
    glVertex2f(188.0f, 132.0f);
    glEnd();


    // Lines

    glColor3f(0.0f, 0.0f, 0.0f);


    glBegin(GL_LINES);
    glVertex2f(148.0f, 148.0f);
    glVertex2f(148.0f, 162.0f);

    glVertex2f(188.0f, 148.0f);
    glVertex2f(188.0f, 162.0f);

    glVertex2f(115.0f, 72.0f);
    glVertex2f(115.0f, 54.0f);

    glVertex2f(150.0f, 60.0f);
    glVertex2f(150.0f, 44.0f);

    glVertex2f(218.0f, 72.0f);
    glVertex2f(218.0f, 58.0f);

    glVertex2f(148.0f, 162.0f);
    glVertex2f(115.0f, 72.0f);

    glVertex2f(188.0f, 162.0f);
    glVertex2f(150.0f, 60.0f);

    glVertex2f(188.0f, 162.0f);
    glVertex2f(218.0f, 72.0f);
    glEnd();

    glLineWidth(1.0f);
}

//Wood

void drawWood()
{
    // First wooden piece
    glColor3ub(170, 120, 70);
    glBegin(GL_POLYGON);
    glVertex2f(242, 43);
    glVertex2f(247, 39);
    glVertex2f(272, 51);
    glVertex2f(276, 56);
    glVertex2f(271, 60);
    glVertex2f(246, 48);
    glEnd();

    // Second wooden piece
    glColor3ub(145, 95, 55);
    glBegin(GL_POLYGON);
    glVertex2f(243, 58);
    glVertex2f(247, 62);
    glVertex2f(272, 50);
    glVertex2f(278, 46);
    glVertex2f(273, 41);
    glVertex2f(248, 53);
    glEnd();
}

void drawFire()
{
    glPushMatrix();
    glTranslatef(260.0f, 48.0f, 0.0f);
    glScalef(1.0f, fireScale, 1.0f);

    // outer flame
    glColor3ub(255, 80, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(-15, 0);
    glVertex2f(15, 0);
    glVertex2f(0, 35);
    glEnd();

    // middle flame
    glColor3ub(255, 180, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(-10, 0);
    glVertex2f(10, 0);
    glVertex2f(0, 25);
    glEnd();

    // inner flame
    glColor3ub(255, 255, 150);
    glBegin(GL_TRIANGLES);
    glVertex2f(-5, 0);
    glVertex2f(5, 0);
    glVertex2f(0, 15);
    glEnd();

    glPopMatrix();
}


//Lower ground Trees

void drawTrees()
{
    // Bottom TREE 1

    // trunk
    glColor3ub(70, 20, 10);
    glBegin(GL_POLYGON);
    glVertex2f(15, 0);
    glVertex2f(20, 0);
    glVertex2f(20, 17);
    glVertex2f(15, 17);
    glEnd();

    // bottom layer
    glColor3ub(0, 55, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 17);
    glVertex2f(34, 17);
    glVertex2f(17, 42);
    glEnd();

    // middle layer
    glColor3ub(0, 85, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(2, 34);
    glVertex2f(32, 34);
    glVertex2f(17, 67);
    glEnd();

    // top layer
    glColor3ub(0, 120, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(4, 51);
    glVertex2f(30, 51);
    glVertex2f(17, 92);
    glEnd();


// Bottom TREE 2

// trunk
    glColor3ub(70, 20, 10);
    glBegin(GL_POLYGON);
    glVertex2f(42.5, 10);
    glVertex2f(47.5, 10);
    glVertex2f(47.5, 25);
    glVertex2f(42.5, 25);
    glEnd();

// bottom layer
    glColor3ub(0, 55, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(29.5, 25);
    glVertex2f(60.5, 25);
    glVertex2f(44.5, 48);
    glEnd();

// middle layer
    glColor3ub(0, 85, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(31.5, 38);
    glVertex2f(57.5, 38);
    glVertex2f(44.5, 60);
    glEnd();

// top layer
    glColor3ub(0, 120, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(33.5, 50);
    glVertex2f(55.5, 50);
    glVertex2f(44.5, 78);
    glEnd();

// Bottom TREE 3

// trunk
    glColor3ub(70, 20, 10);
    glBegin(GL_POLYGON);
    glVertex2f(70, 0);
    glVertex2f(75, 0);
    glVertex2f(75, 17);
    glVertex2f(70, 17);
    glEnd();

// bottom layer
    glColor3ub(0, 55, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(55, 17);
    glVertex2f(90, 17);
    glVertex2f(73, 42);
    glEnd();

// middle layer
    glColor3ub(0, 85, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(57, 34);
    glVertex2f(87, 34);
    glVertex2f(73, 67);
    glEnd();

// top layer
    glColor3ub(0, 120, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(59, 51);
    glVertex2f(85, 51);
    glVertex2f(73, 92);
    glEnd();

// Upper TREE 1

// trunk
    glColor3ub(70, 20, 10);
    glBegin(GL_POLYGON);
    glVertex2f(10, 90);
    glVertex2f(14, 90);
    glVertex2f(14, 104);
    glVertex2f(10, 104);
    glEnd();

// bottom layer
    glColor3ub(0, 55, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 102);
    glVertex2f(24, 102);
    glVertex2f(12, 122);
    glEnd();

// middle layer
    glColor3ub(0, 85, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(2, 114);
    glVertex2f(22, 114);
    glVertex2f(12, 136);
    glEnd();

// top layer
    glColor3ub(0, 120, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(4, 128);
    glVertex2f(20, 128);
    glVertex2f(12, 150);
    glEnd();

// Upper TREE 2

// trunk
    glColor3ub(70, 20, 10);
    glBegin(GL_POLYGON);
    glVertex2f(33, 69);
    glVertex2f(38, 69);
    glVertex2f(38, 86);
    glVertex2f(33, 86);
    glEnd();

// bottom layer
    glColor3ub(0, 55, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(20, 86);
    glVertex2f(51, 86);
    glVertex2f(35, 114);
    glEnd();

// middle layer
    glColor3ub(0, 85, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(22, 99);
    glVertex2f(48, 99);
    glVertex2f(35, 129);
    glEnd();

// top layer
    glColor3ub(0, 120, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(24, 114);
    glVertex2f(46, 114);
    glVertex2f(35, 154);
    glEnd();

// Upper TREE 3

// trunk
    glColor3ub(70, 20, 10);
    glBegin(GL_POLYGON);
    glVertex2f(60, 75.5);
    glVertex2f(65, 75.5);
    glVertex2f(65, 92.5);
    glVertex2f(60, 92.5);
    glEnd();

// bottom layer
    glColor3ub(0, 55, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(47, 92.5);
    glVertex2f(78, 92.5);
    glVertex2f(62, 120.5);
    glEnd();

// middle layer
    glColor3ub(0, 85, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(49, 105.5);
    glVertex2f(75, 105.5);
    glVertex2f(62, 135.5);
    glEnd();

// top layer
    glColor3ub(0, 120, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(51, 120.5);
    glVertex2f(73, 120.5);
    glVertex2f(62, 160.5);
    glEnd();
}
void drawSky()
{
    glBegin(GL_QUADS);

    if(dayState == 1)
    {
        glColor3f(0.02f, 0.75f, 0.95f);   // top blue
        glVertex2f(0, 500);
        glVertex2f(500, 500);

        glColor3f(0.55f, 0.95f, 1.0f);    // bottom light cyan
        glVertex2f(500, 276);
        glVertex2f(0, 276);
    }
    else if(dayState == 2)
    {
        glColor3f(0.02f + 0.23f * sunsetLevel,
                  0.75f - 0.40f * sunsetLevel,
                  0.95f - 0.30f * sunsetLevel);
        glVertex2f(0, 500);
        glVertex2f(500, 500);

        glColor3f(0.55f + 0.45f * sunsetLevel,
                  0.95f - 0.40f * sunsetLevel,
                  1.00f - 0.88f * sunsetLevel);
        glVertex2f(500, 276);
        glVertex2f(0, 276);
    }
    else
    {
        glColor3f(0.01f, 0.01f, 0.03f);   // top almost black
        glVertex2f(0, 500);
        glVertex2f(500, 500);

        glColor3f(0.07f, 0.09f, 0.12f);   // bottom dark blue-gray
        glVertex2f(500, 276);
        glVertex2f(0, 276);
    }

    glEnd();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
    // Boat 1
    case 'a': //run
        boatRun = 1;
        break;

    case 'x': //stop
        boatRun = 0;
        break;

    case 'f': //fast
        boatSpeed += 0.2f;
        break;

    case 's': //slow
        if (boatSpeed > 0.2f)
            boatSpeed -= 0.2f;
        break;

    case 'z': //normal
        boatSpeed = defaultSpeed;
        break;


    // Boat 2
    case 'g':   // run
        boat2Run = 1;
        break;

    case 'h':   // stop
        boat2Run = 0;
        break;

    case 'j':   // fast
        boat2Speed += 0.2f;
        break;

    case 'k':   // slow
        if (boat2Speed > 0.2f)
            boat2Speed -= 0.2f;
        break;

    case 'l':   // normal
        boat2Speed = boat2DefaultSpeed;
        break;

    case 'p':
        planeActive = 1;
        planeDir    = 1;
        planeX      = -150.0f;
        planeY      = 380.0f;   // starting height
        planeScale  = 0.9f;
        break;

    case 'o':
        planeActive = 1;
        planeDir    = -1;
        planeX      = 650.0f;
        planeY      = 380.0f;
        planeScale  = 0.9f;
        break;

    case 'm':   // sunset start
        if(isRaining)
        {
            sunsetRun = 0;
            dayRun = 0;
            nightRun = 0;

            if(dayState == 3)
                sunY = -100.0f;

            break;
        }

        dayState = 2;
        sunsetRun = 1;
        nightRun = 0;
        dayRun = 0;

        sunY = 440.0f;
        sunsetLevel = 0.0f;
        break;

    case 'n':   // night start
        dayState = 3;
        nightRun = 1;
        sunsetRun = 0;
        dayRun = 0;

        moonY = 520.0f;
        sunY = -100.0f;
        break;

    case 'd':   // day return
        dayState = 1;
        sunsetRun = 0;
        nightRun = 0;

        if(isRaining)
        {
            dayRun = 0;
            sunY = -100.0f;
            moonY = -60.0f;
            sunsetLevel = 0.0f;
        }
        else
        {
            dayRun = 1;
            sunY = 520.0f;
        }
        break;

    case 'r':   // rain start
        isRaining = 1;

        sunY = -100.0f;
        sunsetRun = 0;
        dayRun = 0;
        break;

    case '1':   // rain stop
        isRaining = 0;

        if(dayState == 1)
        {
            sunY = 440.0f;
            sunsetLevel = 0.0f;
        }
        else if(dayState == 2)
        {
            sunY = 300.0f;
            sunsetLevel = 1.0f;
        }
        break;

    case 't':   // AUTO lightning ON
        lightningMode = 1;
        break;

    case '2':   // COMPLETELY OFF
        lightningMode = 0;
        lightningVisible = 0;
        break;


    case '[':   // boat3 departs toward far bank
        if (boat3State == 0)
            boat3State = 1;
        break;

    case ']':   // boat3 returns to grass bank
        if (boat3State == 2)
            boat3State = 3;
        break;

    }

    glutPostRedisplay();
}
void update(int value)
{
    // lightning blinking logic
    // lightning blinking logic (REALISTIC FLASH)
    if (lightningMode == 1)
    {
        lightningCounter++;

        // short flash ON
        if (lightningCounter % 150 == 0)
            lightningVisible = 1;

        // quick OFF (after few frames)
        if (lightningCounter % 150 == 5)
            lightningVisible = 0;
    }

    if (sunsetRun == 1)
    {
        if (sunY > 300.0f)
            sunY -= 0.4f;

        if (sunsetLevel < 1.0f)
            sunsetLevel += 0.004f;
    }

    if (nightRun == 1)
    {
        if (moonY > 440.0f)
            moonY -= 0.5f;
        else
        {
            moonY = 440.0f;
            nightRun = 0;
        }
    }

    if (dayRun == 1)
    {
        if (moonY > -60.0f)
            moonY -= 0.5f;

        if (sunY > 440.0f)
            sunY -= 0.5f;

        if (moonY <= -60.0f && sunY <= 440.0f)
        {
            moonY = -60.0f;
            sunY = 440.0f;
            sunsetLevel = 0.0f;
            dayRun = 0;
        }
    }
    if (boatRun == 1)
    {
        boatX += boatDir * boatSpeed;

        if (boatX < -180.0f)
        {
            boatDir = 1;
        }

        if (boat3State == 1 || boat3State == 2)
        {
            if (boatX > 280.0f)
                boatDir = -1;
        }
        else
        {
            if (boatX > 420.0f)
                boatDir = -1;
        }
    }

// Bottom realistic boat movement
    if (boat2Run == 1)
    {
        boat2X += boat2Dir * boat2Speed;

        if (boat2X < -430.0f)
        {
            boat2Dir = 1;
        }

        if (boat2X > 170.0f)
        {
            boat2Dir = -1;
        }
    }

    // Cloud automatic movement
    cloudX += cloudSpeed;

    if (cloudX > 550.0f)
    {
        cloudX = 0.0f;
    }


    // Birds automatic movement
    birdX += birdSpeed;

    if (birdX > 550.0f)
    {
        birdX = 0.0f;
    }

// Birds wing up-down animation
    if (wingUp == 1)
    {
        birdWing += 0.25f;

        if (birdWing >= 8.0f)
            wingUp = 0;
    }
    else
    {
        birdWing -= 0.25f;

        if (birdWing <= -3.0f)
            wingUp = 1;
    }
    // Fire flicker animation
    if(fireUp == 1)
    {
        fireScale += 0.02f;

        if(fireScale >= 1.25f)
            fireUp = 0;
    }
    else
    {
        fireScale -= 0.02f;

        if(fireScale <= 0.85f)
            fireUp = 1;
    }
// Plane movement and scale animation
    if (planeActive)
    {
        planeX += planeDir * 1.2f;
        planeY += 0.15f;   // removed planeDir — always drifts upward

        if (planeScale > 0.08f)
            planeScale -= 0.0012f;   // faster shrink so zoom-out is visible
        else
            planeActive = 0;
    }

    if(isRaining == 1)
    {
        rainY += 8;

        if(rainY > 500)
            rainY = 0;

        sunsetRun = 0;

        if(dayState != 3)
            sunY = -100.0f;

        skyR = 0.25f;
        skyG = 0.30f;
        skyB = 0.35f;
    }
    else
    {
        skyR = 0.53f;
        skyG = 0.81f;
        skyB = 0.98f;
    }

    if (boat3State == 1)
    {
        float t = (boat3Y - BOAT3_START_Y) / (BOAT3_END_Y - BOAT3_START_Y);
        boat3Y    += boat3Speed * 0.5f;
        boat3X     = BOAT3_START_X + t * (BOAT3_END_X - BOAT3_START_X);
        boat3Scale = BOAT3_START_S + t * (BOAT3_END_S - BOAT3_START_S);

        if (boat3Y >= BOAT3_END_Y)
        {
            boat3Y     = BOAT3_END_Y;
            boat3X     = BOAT3_END_X;
            boat3Scale = BOAT3_END_S;
            boat3State = 2;
        }
    }
    else if (boat3State == 3)
    {
        float t = (boat3Y - BOAT3_END_Y) / (BOAT3_START_Y - BOAT3_END_Y);
        boat3Y    -= boat3Speed * 0.5f;
        boat3X     = BOAT3_END_X + t * (BOAT3_START_X - BOAT3_END_X);
        boat3Scale = BOAT3_END_S + t * (BOAT3_START_S - BOAT3_END_S);

        if (boat3Y <= BOAT3_START_Y)
        {
            boat3Y     = BOAT3_START_Y;
            boat3X     = BOAT3_START_X;
            boat3Scale = BOAT3_START_S;
            boat3State = 0;
        }
    }


    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}


// MONAMI's Part


void drawGrass(float x, float y)
{
    // grass ground
    glColor3ub(0, 80, 0);
    glBegin(GL_POLYGON);
    glVertex2f(x,     y);
    glVertex2f(x+20,  y);
    glVertex2f(x+20,  y+3);
    glVertex2f(x,     y+3);
    glEnd();

    // left leaf
    glBegin(GL_TRIANGLES);
    glVertex2f(x,     y+3);
    glVertex2f(x+10,  y+3);
    glVertex2f(x-2,   y+15);
    glEnd();

    // middle leaf
    glBegin(GL_TRIANGLES);
    glVertex2f(x+5,   y+3);
    glVertex2f(x+15,  y+3);
    glVertex2f(x+10,  y+20);
    glEnd();

    // right leaf
    glBegin(GL_TRIANGLES);
    glVertex2f(x+13,  y+3);
    glVertex2f(x+20,  y+3);
    glVertex2f(x+22,  y+15);
    glEnd();

    // flower (orange circle)
    glColor3ub(255, 165, 0);

    float cx = x + 10;
    float cy = y + 20;
    float r  = 5;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(int i = 0; i <= 30; i++)
    {
        float angle = 2 * PI * i / 30;
        glVertex2f(cx + r*cos(angle),
                   cy + r*sin(angle));
    }
    glEnd();
}
void drawFlower(float x, float y, float height)
{
    // STEM (lower)
    glColor3ub(123, 63, 0);
    glBegin(GL_POLYGON);
    glVertex2f(x,     y);
    glVertex2f(x+2,   y);
    glVertex2f(x+2,   y + height);
    glVertex2f(x,     y + height);
    glEnd();

    // STEM JOINT (small circle)
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(123, 63, 0);
    glVertex2f(x+1, y + height + 1);
    for(int i = 0; i <= 20; i++)
        glVertex2f(x + 1 + 1.5*cos(2*PI*i/20),
                   y + height + 1 + 1.5*sin(2*PI*i/20));
    glEnd();

    // FLOWER CENTER
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(255, 100, 0);
    glVertex2f(x+1, y + height + 6);
    for(int i = 0; i <= 20; i++)
        glVertex2f(x + 1 + 4*cos(2*PI*i/20),
                   y + height + 6 + 4*sin(2*PI*i/20));
    glEnd();

    // PETALS
    glColor3ub(255,255,255);
    float px = x + 1;
    float py = y + height + 6;

    glBegin(GL_TRIANGLES);
    glVertex2f(px, py);
    glVertex2f(px-2, py+6);
    glVertex2f(px-8, py+2);
    glVertex2f(px, py);
    glVertex2f(px+2, py+6);
    glVertex2f(px+8, py+2);
    glVertex2f(px, py);
    glVertex2f(px+6, py);
    glVertex2f(px+2, py-6);
    glVertex2f(px, py);
    glVertex2f(px-6, py);
    glVertex2f(px-2, py-6);
    glEnd();
}


//right side boat

void drawRealisticBoat(float x, float y)
{
    // WATER SHADOW
    glColor3ub(40, 110, 160);
    glBegin(GL_POLYGON);
    glVertex2f(x + 5,  y - 3);
    glVertex2f(x + 65, y - 3);
    glVertex2f(x + 60, y - 7);
    glVertex2f(x + 10, y - 7);
    glEnd();

    // BOAT HULL (DARK WOOD)
    glColor3ub(92, 64, 51);   // dark brown
    glBegin(GL_POLYGON);
    glVertex2f(x + 5,  y);
    glVertex2f(x + 65, y);
    glVertex2f(x + 75, y + 12);
    glVertex2f(x + 60, y + 22);
    glVertex2f(x + 10, y + 22);
    glVertex2f(x - 5,  y + 12);
    glEnd();

    // UPPER RIM (LIGHT WOOD)
    glColor3ub(123, 85, 60);
    glBegin(GL_POLYGON);
    glVertex2f(x + 10, y + 18);
    glVertex2f(x + 60, y + 18);
    glVertex2f(x + 57, y + 21);
    glVertex2f(x + 13, y + 21);
    glEnd();

    // INNER SEATS
    glColor3ub(80, 50, 30);
    glBegin(GL_POLYGON);
    glVertex2f(x + 18, y + 8);
    glVertex2f(x + 30, y + 8);
    glVertex2f(x + 30, y + 14);
    glVertex2f(x + 18, y + 14);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x + 40, y + 8);
    glVertex2f(x + 52, y + 8);
    glVertex2f(x + 52, y + 14);
    glVertex2f(x + 40, y + 14);
    glEnd();

    // MAST (WOOD)
    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
    glVertex2f(x + 35, y + 22);
    glVertex2f(x + 38, y + 22);
    glVertex2f(x + 38, y + 95);
    glVertex2f(x + 35, y + 95);
    glEnd();

    // MAST STRIPES
    glColor3ub(200, 230, 240);
    for (int i = 0; i < 6; i++)
    {
        glBegin(GL_POLYGON);
        glVertex2f(x + 35, y + 30 + i*10);
        glVertex2f(x + 38, y + 30 + i*10);
        glVertex2f(x + 38, y + 34 + i*10);
        glVertex2f(x + 35, y + 34 + i*10);
        glEnd();
    }

    // LEFT STRIPES
    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
    glVertex2f(355,167);
    glVertex2f(385,167);
    glVertex2f(385,169);
    glVertex2f(355,169);
    glEnd();



    // RIGHT STRIPES

    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
    glVertex2f(387,167);
    glVertex2f(424,167);
    glVertex2f(424,169);
    glVertex2f(387,169);
    glEnd();

    // LEFT PAAL

    glColor3ub(255, 255, 255);
    glBegin(GL_TRIANGLES);
    glVertex2f(363,169);
    glVertex2f(385,169);
    glVertex2f(385,210);

    glEnd();

    // RIGHT PAAL

    glColor3ub(128, 32, 32);
    glBegin(GL_TRIANGLES);
    glVertex2f(387,169);
    glVertex2f(418,169);
    glVertex2f(387,230);

    glEnd();

    // MAROON BORDER

    glColor3ub(128, 32, 32);
    glBegin(GL_POLYGON);
    glVertex2f(361,169);
    glVertex2f(363,169);
    glVertex2f(385,210);
    glVertex2f(383,210);
    glEnd();

}


//river shading
void drawRiverGradient()
{
    glBegin(GL_QUADS);

    if(dayState == 3) //night view on
    {
        glColor3ub(20, 55, 60);
        glVertex2f(0, 214);
        glVertex2f(500, 214);

        glColor3ub(10, 30, 35);
        glVertex2f(500, 110);
        glVertex2f(0, 110);
    }
    else //day view
    {
        glColor3ub(0, 180, 220);
        glVertex2f(0, 214);
        glVertex2f(500, 214);

        glColor3ub(0, 120, 180);
        glVertex2f(500, 110);
        glVertex2f(0, 110);
    }

    glEnd();
}

void drawPlane()
{
    // FUSELAGE (main body)
    glColor3ub(230, 230, 230);
    glBegin(GL_POLYGON);
    glVertex2f(-60, 0);
    glVertex2f( 60, 5);
    glVertex2f( 60, -5);
    glVertex2f(-60, -5);
    glEnd();

    // NOSE CONE
    glColor3ub(200, 200, 200);
    glBegin(GL_POLYGON);
    glVertex2f(60,  5);
    glVertex2f(60, -5);
    glVertex2f(80,  0);
    glEnd();

    // TAIL FIN (vertical)
    glColor3ub(180, 50, 50);
    glBegin(GL_POLYGON);
    glVertex2f(-50, 5);
    glVertex2f(-35, 5);
    glVertex2f(-38, 25);
    glVertex2f(-55, 25);
    glEnd();

    // TAIL WING (horizontal stabilizer)
    glColor3ub(180, 50, 50);
    glBegin(GL_POLYGON);
    glVertex2f(-55, 0);
    glVertex2f(-30, 0);
    glVertex2f(-35, -12);
    glVertex2f(-60, -12);
    glEnd();

    // MAIN WINGS
    glColor3ub(180, 50, 50);
    glBegin(GL_POLYGON);
    glVertex2f( 10,  5);
    glVertex2f( 30,  5);
    glVertex2f( 10, 40);
    glVertex2f(-15, 40);
    glEnd();

    // WING UNDERSIDE
    glColor3ub(180, 50, 50);
    glBegin(GL_POLYGON);
    glVertex2f( 10, -5);
    glVertex2f( 30, -5);
    glVertex2f( 10, -35);
    glVertex2f(-15, -35);
    glEnd();

    // WINDOWS (3 small circles)
    glColor3ub(150, 220, 255);
    for (int i = 0; i < 3; i++)
    {
        float wx = 30.0f - i * 18.0f;
        float wy = 3.0f;
        float wr = 4.0f;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(wx, wy);
        for (int j = 0; j <= 20; j++)
        {
            float angle = 2 * PI * j / 20;
            glVertex2f(wx + wr * cos(angle), wy + wr * sin(angle));
        }
        glEnd();
    }

}

void drawBoat3()
{
    // Hull — simple trapezoid boat shape
    glColor3ub(92, 64, 51);
    glBegin(GL_POLYGON);
    glVertex2f(-28,  0);
    glVertex2f( 28,  0);
    glVertex2f( 22,  9);
    glVertex2f(-22,  9);
    glEnd();

    // Upper rim (lighter wood)
    glColor3ub(139, 90, 43);
    glBegin(GL_POLYGON);
    glVertex2f(-28,  7);
    glVertex2f( 28,  7);
    glVertex2f( 24, 10);
    glVertex2f(-24, 10);
    glEnd();

    // Mast
    glColor3ub(101, 67, 33);
    glBegin(GL_POLYGON);
    glVertex2f(-1.5f, 10);
    glVertex2f( 1.5f, 10);
    glVertex2f( 1.5f, 50);
    glVertex2f(-1.5f, 50);
    glEnd();

    // Sail
    glColor3ub(255, 255, 200);
    glBegin(GL_TRIANGLES);
    glVertex2f( 1.5f, 50);
    glVertex2f( 1.5f, 14);
    glVertex2f(26.0f, 28);
    glEnd();

    // Sail shadow/depth
    glColor3ub(220, 200, 130);
    glBegin(GL_TRIANGLES);
    glVertex2f( 1.5f, 50);
    glVertex2f( 1.5f, 14);
    glVertex2f(-18.0f, 30);
    glEnd();
}


// FARUKI's Part
void drawRain()
{
    glColor3f(0.85f, 0.9f, 1.0f);
    glLineWidth(1.2f);

    glBegin(GL_LINES);

    for(int i = 0; i < 70; i++)
    {
        float x = (i * 37) % 500;
        float y = 500 - ((i * 25 + (int)rainY) % 500);

        glVertex2f(x, y);
        glVertex2f(x - 4, y - 15);
    }

    glEnd();

    glLineWidth(1.0f);
}



void drawMountains()
{
    float r1, g1, b1; // highlight / light side
    float r2, g2, b2; // shadow / dark side
    float r3, g3, b3; // deepest shadow

    if(dayState == 3) // NIGHT
    {
        r1 = 0.12f;
        g1 = 0.36f;
        b1 = 0.16f;   // visible moonlight green
        r2 = 0.04f;
        g2 = 0.18f;
        b2 = 0.08f;   // dark green shadow
        r3 = 0.01f;
        g3 = 0.08f;
        b3 = 0.04f;   // deepest shadow
    }
    else if(dayState == 2) // SUNSET
    {
        r1 = 0.36f;
        g1 = 0.56f;
        b1 = 0.28f;
        r2 = 0.22f;
        g2 = 0.34f;
        b2 = 0.16f;
        r3 = 0.10f;
        g3 = 0.20f;
        b3 = 0.08f;
    }
    else // DAY
    {
        r1 = 0.30f;
        g1 = 0.70f;
        b1 = 0.30f;
        r2 = 0.20f;
        g2 = 0.50f;
        b2 = 0.20f;
        r3 = 0.05f;
        g3 = 0.25f;
        b3 = 0.05f;
    }

    // RIGHT SIDE
    glColor3f(r1, g1, b1);
    glBegin(GL_TRIANGLES);
    glVertex2f(300, 276);
    glVertex2f(420, 276);
    glVertex2f(370, 380);
    glEnd();

    glColor3f(r2, g2, b2);
    glBegin(GL_TRIANGLES);
    glVertex2f(350, 276);
    glVertex2f(450, 420);
    glVertex2f(410, 276);
    glEnd();

    glColor3f(r1, g1, b1);
    glBegin(GL_TRIANGLES);
    glVertex2f(410, 276);
    glVertex2f(450, 420);
    glVertex2f(500, 276);
    glEnd();

    glColor3f(r2, g2, b2);
    glBegin(GL_TRIANGLES);
    glVertex2f(460, 276);
    glVertex2f(500, 276);
    glVertex2f(520, 380);
    glEnd();

    // MIDDLE
    glColor3f(r2, g2, b2);
    glBegin(GL_TRIANGLES);
    glVertex2f(120, 274);
    glVertex2f(240, 274);
    glVertex2f(190, 380);
    glEnd();

    glColor3f(r3, g3, b3);
    glBegin(GL_TRIANGLES);
    glVertex2f(170, 276);
    glVertex2f(270, 420);
    glVertex2f(230, 276);
    glEnd();

    glColor3f(r1, g1, b1);
    glBegin(GL_TRIANGLES);
    glVertex2f(230, 276);
    glVertex2f(270, 420);
    glVertex2f(320, 276);
    glEnd();

    // LEFT
    glColor3f(r2, g2, b2);
    glBegin(GL_TRIANGLES);
    glVertex2f(-60, 274);
    glVertex2f(60, 274);
    glVertex2f(10, 380);
    glEnd();

    glColor3f(r3, g3, b3);
    glBegin(GL_TRIANGLES);
    glVertex2f(-10, 276);
    glVertex2f(140, 276);
    glVertex2f(90, 420);
    glEnd();

    glColor3f(r1, g1, b1);
    glBegin(GL_TRIANGLES);
    glVertex2f(50, 276);
    glVertex2f(90, 420);
    glVertex2f(140, 276);
    glEnd();

    glColor3f(r2, g2, b2);
    glBegin(GL_TRIANGLES);
    glVertex2f(80, 275);
    glVertex2f(200, 275);
    glVertex2f(140, 360);
    glEnd();
}
void drawTriangleTree(float x, float y)
{
    // tree trunk
    glColor3ub(70, 45, 25);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + 3, y);
    glVertex2f(x + 3, y + 15);
    glVertex2f(x, y + 15);
    glEnd();

    // bottom layer
    glColor3ub(0, 60, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 10, y + 12);
    glVertex2f(x + 13, y + 12);
    glVertex2f(x + 1.5, y + 32);
    glEnd();

    // middle layer
    glColor3ub(0, 85, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 8, y + 24);
    glVertex2f(x + 11, y + 24);
    glVertex2f(x + 1.5, y + 48);
    glEnd();

    // top layer
    glColor3ub(0, 120, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 6, y + 38);
    glVertex2f(x + 9, y + 38);
    glVertex2f(x + 1.5, y + 62);
    glEnd();
}
void drawRightForest()
{

    drawTriangleTree(340, 250);
    drawTriangleTree(375, 252);
    drawTriangleTree(410, 250);
    drawTriangleTree(445, 252);
    drawTriangleTree(480, 250);

    drawTriangleTree(355, 248);
    drawTriangleTree(390, 246);
    drawTriangleTree(425, 248);
    drawTriangleTree(460, 250);
    drawTriangleTree(495, 248);
    drawTriangleTree(510, 248);
}

//house color changing functions for night view

void setRoundTreeTrunkColor()
{
    if(dayState == 3)
        glColor3ub(55, 30, 18);
    else if(dayState == 2)
        glColor3ub(115, 70, 35);
    else
        glColor3ub(139, 90, 43);
}

void setRoundTreeDarkLeafColor()
{
    if(dayState == 3)
        glColor3ub(10, 55, 20);
    else if(dayState == 2)
        glColor3ub(45, 120, 35);
    else
        glColor3ub(34, 110, 34);
}

void setRoundTreeMainLeafColor()
{
    if(dayState == 3)
        glColor3ub(20, 85, 30);
    else if(dayState == 2)
        glColor3ub(70, 145, 45);
    else
        glColor3ub(60, 160, 40);
}

void setRoundTreeHighlightColor()
{
    if(dayState == 3)
        glColor3ub(40, 115, 45);
    else if(dayState == 2)
        glColor3ub(95, 175, 55);
    else
        glColor3ub(90, 195, 55);
}
void drawCircleTree(float x, float y)
{
    // trunk
    setRoundTreeTrunkColor();
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + 5, y);
    glVertex2f(x + 5, y + 20);
    glVertex2f(x, y + 20);
    glEnd();

    if(dayState == 3)
    {
        drawCircle(x - 6, y + 24, 8, 0.04f, 0.22f, 0.06f);
        drawCircle(x + 3, y + 28, 10, 0.06f, 0.30f, 0.08f);
        drawCircle(x + 12, y + 24, 8, 0.04f, 0.22f, 0.06f);
        drawCircle(x + 3, y + 36, 7, 0.10f, 0.40f, 0.12f);
    }
    else if(dayState == 2)
    {
        drawCircle(x - 6, y + 24, 8, 0.25f, 0.55f, 0.18f);
        drawCircle(x + 3, y + 28, 10, 0.32f, 0.65f, 0.20f);
        drawCircle(x + 12, y + 24, 8, 0.25f, 0.55f, 0.18f);
        drawCircle(x + 3, y + 36, 7, 0.45f, 0.75f, 0.25f);
    }
    else
    {
        float r = 0.235f, g = 0.627f, b = 0.157f;
        drawCircle(x - 6, y + 24, 8, r, g, b);
        drawCircle(x + 3, y + 28, 10, r, g, b);
        drawCircle(x + 12, y + 24, 8, r, g, b);
        drawCircle(x + 3, y + 36, 7, r, g, b);
    }
}


void setHouseWallColor()
{
    if(dayState == 3)
        glColor3ub(170, 135, 105);
    else if(dayState == 2)
        glColor3ub(205, 150, 105);
    else
        glColor3ub(220, 180, 140);
}

void setHouseSideColor()
{
    if(dayState == 3)
        glColor3ub(155, 120, 90);
    else if(dayState == 2)
        glColor3ub(180, 125, 90);
    else
        glColor3ub(200, 160, 120);
}

void setHouseRoofColor()
{
    if(dayState == 3)
        glColor3ub(135, 45, 45);
    else if(dayState == 2)
        glColor3ub(150, 65, 45);
    else
        glColor3ub(170, 60, 60);
}

void setHouseDoorWindowColor()
{
    if(dayState == 3)
        glColor3ub(90, 50, 30);
    else if(dayState == 2)
        glColor3ub(90, 55, 35);
    else
        glColor3ub(120, 70, 40);
}

// MORIUM's Part
void drawCircle(float x, float y, float r)
{
    glBegin(GL_POLYGON);
    for(int i = 0; i < 100; i++)
    {
        float angle = 2 * 3.1416 * i / 100;
        glVertex2f(x + cos(angle) * r, y + sin(angle) * r);
    }
    glEnd();
}
void drawLeftForest()
{

    if(dayState == 3)
        glColor3ub(60, 40, 20);
    else
        glColor3ub(101, 67, 33);

    glBegin(GL_POLYGON);
    glVertex2f(70, 260);
    glVertex2f(78, 260);
    glVertex2f(78, 320);
    glVertex2f(70, 320);
    glEnd();

    if(dayState == 3)
        glColor3ub(0, 90, 0);
    else
        glColor3ub(0, 160, 0);

    drawCircle(74, 340, 18);
    drawCircle(58, 330, 14);
    drawCircle(90, 330, 14);
    drawCircle(74, 322, 14);



    if(dayState == 3)
        glColor3ub(60, 40, 20);
    else
        glColor3ub(101, 67, 33);

    glBegin(GL_POLYGON);
    glVertex2f(48, 260);
    glVertex2f(56, 260);
    glVertex2f(56, 315);
    glVertex2f(48, 315);
    glEnd();

    if(dayState == 3)
        glColor3ub(0, 80, 0);
    else
        glColor3ub(0, 140, 0);

    drawCircle(52, 332, 16);
    drawCircle(36, 322, 12);
    drawCircle(68, 322, 12);
    drawCircle(52, 315, 12);



    if(dayState == 3)
        glColor3ub(60, 40, 20);
    else
        glColor3ub(101, 67, 33);

    glBegin(GL_POLYGON);
    glVertex2f(28, 260);
    glVertex2f(36, 260);
    glVertex2f(36, 310);
    glVertex2f(28, 310);
    glEnd();

    if(dayState == 3)
        glColor3ub(0, 70, 0);
    else
        glColor3ub(0, 130, 0);

    drawCircle(32, 325, 14);
    drawCircle(18, 315, 10);
    drawCircle(46, 315, 10);
    drawCircle(32, 308, 10);



    if(dayState == 3)
        glColor3ub(60, 40, 20);
    else
        glColor3ub(101, 67, 33);

    glBegin(GL_POLYGON);
    glVertex2f(5, 260);
    glVertex2f(13, 260);
    glVertex2f(13, 305);
    glVertex2f(5, 305);
    glEnd();

    if(dayState == 3)
        glColor3ub(0, 60, 0);
    else
        glColor3ub(0, 120, 0);

    drawCircle(9, 318, 13);
    drawCircle(-5, 308, 10);
    drawCircle(23, 308, 10);
    drawCircle(9, 300, 10);
}
void drawHouse()
{
    // FRONT WALL
    setHouseWallColor();
    glBegin(GL_POLYGON);
    glVertex2f(90, 260);
    glVertex2f(125, 260);
    glVertex2f(125, 290);
    glVertex2f(90, 290);
    glEnd();

    // RIGHT SIDE WALL
    setHouseSideColor();
    glBegin(GL_POLYGON);
    glVertex2f(125, 260);
    glVertex2f(145, 270);
    glVertex2f(145, 300);
    glVertex2f(125, 290);
    glEnd();

    // FRONT ROOF
    setHouseRoofColor();
    glBegin(GL_TRIANGLES);
    glVertex2f(90, 290);
    glVertex2f(125, 290);
    glVertex2f(107.5, 312);
    glEnd();

    // SIDE ROOF
    if(dayState == 3)
        glColor3ub(110, 35, 35);
    else if(dayState == 2)
        glColor3ub(120, 45, 35);
    else
        glColor3ub(140, 40, 40);

    glBegin(GL_POLYGON);
    glVertex2f(125, 290);
    glVertex2f(145, 300);
    glVertex2f(127.5, 312);
    glVertex2f(107.5, 312);
    glEnd();

    // DOOR
    setHouseDoorWindowColor();
    glBegin(GL_POLYGON);
    glVertex2f(102, 260);
    glVertex2f(112, 260);
    glVertex2f(112, 275);
    glVertex2f(102, 275);
    glEnd();

    // LEFT WINDOW
    glBegin(GL_POLYGON);
    glVertex2f(94, 278);
    glVertex2f(102, 278);
    glVertex2f(102, 286);
    glVertex2f(94, 286);
    glEnd();

    // RIGHT WINDOW
    glBegin(GL_POLYGON);
    glVertex2f(112, 278);
    glVertex2f(120, 278);
    glVertex2f(120, 286);
    glVertex2f(112, 286);
    glEnd();

    // SIDE WINDOW
    glBegin(GL_POLYGON);
    glVertex2f(130, 280);
    glVertex2f(138, 282);
    glVertex2f(138, 290);
    glVertex2f(130, 288);
    glEnd();
}
void drawFrontRoad()
{

    glColor3ub(70, 75, 80);
    glBegin(GL_POLYGON);
    // road started and connected with door's right side
    glVertex2f(0,   220);
    glVertex2f(120, 260);

    // connected with door's left side & road ended
    glVertex2f(97, 260);
    glVertex2f(0, 240);

    glEnd();

}


//ARPITA's part

void drawLightning(float x)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.5f);

    glBegin(GL_LINE_STRIP);

    glVertex2f(x, 500);
    glVertex2f(x - 10, 480);
    glVertex2f(x - 5, 440);
    glVertex2f(x + 15, 420);
    glVertex2f(x, 400);

    glEnd();
}
void drawHouse2(float ox, float oy)
{
    // Roof
    if(dayState == 3)
        glColor3ub(150, 75, 25);     // night adjusted from original
    else if(dayState == 2)
        glColor3ub(230, 125, 45);    // sunset warm
    else
        glColor3ub(210, 105, 30);    // original day

    glBegin(GL_POLYGON);
    glVertex2d(ox + 0,  oy + 33);
    glVertex2d(ox + 30, oy + 33);
    glVertex2d(ox + 23, oy + 48);
    glVertex2d(ox + 9,  oy + 48);
    glEnd();

    // Main wall
    if(dayState == 3)
        glColor3ub(170, 115, 75);
    else if(dayState == 2)
        glColor3ub(230, 150, 90);
    else
        glColor3ub(244, 164, 96);

    glBegin(GL_POLYGON);
    glVertex2d(ox + 0,  oy + 18);
    glVertex2d(ox + 26, oy + 18);
    glVertex2d(ox + 26, oy + 33);
    glVertex2d(ox + 0,  oy + 33);
    glEnd();

    // Bottom beam
    if(dayState == 3)
        glColor3ub(95, 50, 30);
    else if(dayState == 2)
        glColor3ub(145, 75, 40);
    else
        glColor3ub(160, 82, 45);

    glBegin(GL_POLYGON);
    glVertex2d(ox + 0,  oy + 18);
    glVertex2d(ox + 27, oy + 18);
    glVertex2d(ox + 27, oy + 15);
    glVertex2d(ox + 0,  oy + 15);
    glEnd();

    // Door
    if(dayState == 3)
        glColor3ub(95, 50, 30);
    else if(dayState == 2)
        glColor3ub(145, 75, 40);
    else
        glColor3ub(160, 82, 45);

    glBegin(GL_POLYGON);
    glVertex2d(ox + 12, oy + 18);
    glVertex2d(ox + 20, oy + 18);
    glVertex2d(ox + 20, oy + 27);
    glVertex2d(ox + 12, oy + 27);
    glEnd();
}


void drawHouse1(float ox, float oy)
{
    // Roof
    if(dayState == 3)
        glColor3ub(95, 50, 30);
    else if(dayState == 2)
        glColor3ub(145, 75, 40);
    else
        glColor3ub(160, 82, 45);

    glBegin(GL_POLYGON);
    glVertex2d(ox + 0,  oy + 33);
    glVertex2d(ox + 41, oy + 33);
    glVertex2d(ox + 33, oy + 48);
    glVertex2d(ox + 8,  oy + 48);
    glEnd();

    // Left wall
    if(dayState == 3)
        glColor3ub(180, 150, 115);
    else if(dayState == 2)
        glColor3ub(240, 195, 140);
    else
        glColor3ub(255, 222, 173);

    glBegin(GL_POLYGON);
    glVertex2d(ox + 3,  oy + 12);
    glVertex2d(ox + 15, oy + 12);
    glVertex2d(ox + 15, oy + 33);
    glVertex2d(ox + 3,  oy + 33);
    glEnd();

    // Left roof triangle
    if(dayState == 3)
        glColor3ub(180, 150, 115);
    else if(dayState == 2)
        glColor3ub(240, 195, 140);
    else
        glColor3ub(255, 222, 173);

    glBegin(GL_TRIANGLES);
    glVertex2d(ox + 3,  oy + 33);
    glVertex2d(ox + 15, oy + 33);
    glVertex2d(ox + 8,  oy + 43);
    glEnd();

    // Right wall
    if(dayState == 3)
        glColor3ub(155, 125, 90);
    else if(dayState == 2)
        glColor3ub(210, 165, 115);
    else
        glColor3ub(222, 184, 135);

    glBegin(GL_POLYGON);
    glVertex2d(ox + 15, oy + 12);
    glVertex2d(ox + 38, oy + 12);
    glVertex2d(ox + 38, oy + 33);
    glVertex2d(ox + 15, oy + 33);
    glEnd();

    // Bottom beam
    if(dayState == 3)
        glColor3ub(95, 50, 30);
    else if(dayState == 2)
        glColor3ub(145, 75, 40);
    else
        glColor3ub(160, 82, 45);

    glBegin(GL_POLYGON);
    glVertex2d(ox + 3,  oy + 12);
    glVertex2d(ox + 38, oy + 12);
    glVertex2d(ox + 38, oy + 9);
    glVertex2d(ox + 3,  oy + 9);
    glEnd();

    // Door
    if(dayState == 3)
        glColor3ub(95, 50, 30);
    else if(dayState == 2)
        glColor3ub(145, 75, 40);
    else
        glColor3ub(160, 82, 45);

    glBegin(GL_POLYGON);
    glVertex2d(ox + 23, oy + 12);
    glVertex2d(ox + 29, oy + 12);
    glVertex2d(ox + 29, oy + 26);
    glVertex2d(ox + 23, oy + 26);
    glEnd();

    // Window
    if(dayState == 3)
        glColor3ub(95, 50, 30);
    else if(dayState == 2)
        glColor3ub(145, 75, 40);
    else
        glColor3ub(160, 82, 45);

    glBegin(GL_POLYGON);
    glVertex2d(ox + 6,  oy + 27);
    glVertex2d(ox + 11, oy + 27);
    glVertex2d(ox + 11, oy + 20);
    glVertex2d(ox + 6,  oy + 20);
    glEnd();
}
void drawFilledCircle(float cx, float cy, float d, int r, int g, int b)
{
    glColor3ub(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(int i = 0; i <= 100; i++)
    {
        float angle = 2 * PI * i / 100;
        glVertex2f(cx + d * cos(angle),
                   cy + d * sin(angle));
    }
    glEnd();
}
void drawRoundTree(float x, float y)
{
    // trunk
    setRoundTreeTrunkColor();
    glBegin(GL_POLYGON);
    glVertex2f(x + 12, y);
    glVertex2f(x + 18, y);
    glVertex2f(x + 17, y + 17);
    glVertex2f(x + 12, y + 17);
    glEnd();

    // dark back layer
    if(dayState == 3)
    {
        drawFilledCircle(x + 2,  y + 27, 9,  8,  55, 18);
        drawFilledCircle(x + 9,  y + 24, 10, 8,  55, 18);
        drawFilledCircle(x + 16, y + 25, 9,  8,  55, 18);
        drawFilledCircle(x + 22, y + 27, 8,  8,  55, 18);
        drawFilledCircle(x + 6,  y + 32, 9,  8,  55, 18);
        drawFilledCircle(x + 14, y + 34, 10, 8,  55, 18);
        drawFilledCircle(x + 22, y + 32, 8,  8,  55, 18);

        // main green layer
        drawFilledCircle(x + 4,  y + 28, 8,  18, 85, 28);
        drawFilledCircle(x + 10, y + 25, 9,  18, 85, 28);
        drawFilledCircle(x + 17, y + 26, 8,  18, 85, 28);
        drawFilledCircle(x + 23, y + 28, 7,  18, 85, 28);
        drawFilledCircle(x + 7,  y + 33, 8,  18, 85, 28);
        drawFilledCircle(x + 15, y + 35, 9,  18, 85, 28);
        drawFilledCircle(x + 23, y + 33, 8,  18, 85, 28);
        drawFilledCircle(x + 11, y + 39, 7,  18, 85, 28);
        drawFilledCircle(x + 19, y + 38, 7,  18, 85, 28);

        // bright highlight layer
        drawFilledCircle(x + 11, y + 30, 6,  45, 125, 45);
        drawFilledCircle(x + 17, y + 29, 6,  45, 125, 45);
        drawFilledCircle(x + 14, y + 35, 6,  45, 125, 45);
        drawFilledCircle(x + 9,  y + 36, 5,  45, 125, 45);
        drawFilledCircle(x + 20, y + 34, 5,  45, 125, 45);
        drawFilledCircle(x + 15, y + 26, 5,  45, 125, 45);
    }
    else
    {
        // original day/sunset look
        drawFilledCircle(x + 2,  y + 27, 9,  34, 110, 34);
        drawFilledCircle(x + 9,  y + 24, 10, 34, 110, 34);
        drawFilledCircle(x + 16, y + 25, 9,  34, 110, 34);
        drawFilledCircle(x + 22, y + 27, 8,  34, 110, 34);
        drawFilledCircle(x + 6,  y + 32, 9,  34, 110, 34);
        drawFilledCircle(x + 14, y + 34, 10, 34, 110, 34);
        drawFilledCircle(x + 22, y + 32, 8,  34, 110, 34);

        drawFilledCircle(x + 4,  y + 28, 8,  60, 160, 40);
        drawFilledCircle(x + 10, y + 25, 9,  60, 160, 40);
        drawFilledCircle(x + 17, y + 26, 8,  60, 160, 40);
        drawFilledCircle(x + 23, y + 28, 7,  60, 160, 40);
        drawFilledCircle(x + 7,  y + 33, 8,  60, 160, 40);
        drawFilledCircle(x + 15, y + 35, 9,  60, 160, 40);
        drawFilledCircle(x + 23, y + 33, 8,  60, 160, 40);
        drawFilledCircle(x + 11, y + 39, 7,  60, 160, 40);
        drawFilledCircle(x + 19, y + 38, 7,  60, 160, 40);

        drawFilledCircle(x + 11, y + 30, 6,  90, 195, 55);
        drawFilledCircle(x + 17, y + 29, 6,  90, 195, 55);
        drawFilledCircle(x + 14, y + 35, 6,  90, 195, 55);
        drawFilledCircle(x + 9,  y + 36, 5,  90, 195, 55);
        drawFilledCircle(x + 20, y + 34, 5,  90, 195, 55);
        drawFilledCircle(x + 15, y + 26, 5,  90, 195, 55);
    }
}



void display()
{

    if(isRaining && dayState != 3)
        glClearColor(0.25f, 0.30f, 0.35f, 1.0f);
    else
        glClearColor(skyR, skyG, skyB, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    if(!(isRaining && dayState != 3))
    {
        drawSky();
    }

    drawSun();

    if(dayState == 3)
    {
        drawMoon();
    }

    // Cloud movement
    glPushMatrix();
    glTranslatef(cloudX, 0.0f, 0.0f);
    drawAllClouds();
    glPopMatrix();
// Extra copy for smooth looping
    glPushMatrix();
    glTranslatef(cloudX - 550.0f, 0.0f, 0.0f);
    drawAllClouds();
    glPopMatrix();

    // Plane
    if (planeActive)
    {
        glPushMatrix();
        glTranslatef(planeX, planeY, 0.0f);  // was 400.0f, now uses planeY
        glScalef(planeScale, planeScale, 1.0f);

        // flip horizontally when flying right-to-left
        if (planeDir == -1)
            glScalef(-1.0f, 1.0f, 1.0f);

        drawPlane();
        glPopMatrix();
    }

    // Birds movement
    if(dayState != 3)
    {
        glPushMatrix();
        glTranslatef(birdX, 0.0f, 0.0f);
        if(!isRaining)
        {
            drawBirds();
        }


        glPopMatrix();

        glPushMatrix();
        glTranslatef(birdX - 550.0f, 0.0f, 0.0f);
        if(!isRaining)
        {
            drawBirds();
        }

        glPopMatrix();
    }
    // Main background and river
    drawLowerGround();
    drawRiver();
    drawUpperGround();
    drawRiverGradient();
    drawTopStones();


    //grass locations
    drawGrass(170, 0);
    drawGrass(200, 0);
    drawGrass(230, 0);
    drawGrass(260, 0);
    // flower field (side by side)
// flower field (closer + different heights)
    for (float x = 300; x <= 480; x += 18)   // small gap to closer flowers
    {
        float height = 22 + (int)(x) % 15;   // different flower lengths
        drawFlower(x, 0, height);
    }
    drawMountains();
    drawLeftForest();
    drawHouse();
    drawFrontRoad();
    drawRightForest();

    //Tree drawing Arpita's Part
    drawRoundTree(160.0f,  260.0f);   // left small area
    drawRoundTree(185.0f,  260.0f);   // left small area
    drawRoundTree(220.0f, 260.0f);   // right
    drawRoundTree(205.0f, 270.0f);   // middle
    drawRoundTree(235.0f, 270.0f);   // middle
    drawRoundTree(250.0f, 260.0f);   // right
    drawRoundTree(275.0f, 270.0f);   // middle
    drawRoundTree(290.0f, 260.0f);   // right
    drawRoundTree(265.0f, 260.0f);   // right

    //House drawing Arpita's part
    drawHouse2(210.0f, 240.0f);
    drawHouse1(180.0f,  240.0f);
    drawHouse1(260.0f,  240.0f);


    // Foreground details
    drawBottomStones();

    //Boat

    glPushMatrix();

    glTranslatef(boatX, 0.0f, 0.0f);

// if boat move right side,mirror
    if (boatDir == 1)
    {
        glTranslatef(129.0f, 0.0f, 0.0f);
        glScalef(-1.0f, 1.0f, 1.0f);
        glTranslatef(-129.0f, 0.0f, 0.0f);
    }


    drawBoat();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(boat2X, 0.0f, 0.0f);
    drawRealisticBoat(350, 140);
    glPopMatrix();

    // Boat 3 — perspective crossing (draw before top stones so far bank covers it)
    glPushMatrix();
    glTranslatef(boat3X, boat3Y, 0.0f);
    glScalef(boat3Scale, boat3Scale, 1.0f);
    drawBoat3();
    glPopMatrix();

    drawTent();
    drawWood();

    if(dayState == 3&& !isRaining)
    {
        drawFire();
    }
    drawTrees();
    if(isRaining)
    {
        drawRain();
    }

    if (lightningMode == 1 && lightningVisible)
    {
        drawLightning(100);  // left
        drawLightning(250);  // center
        drawLightning(400);  // right
    }

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(1200, 700);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Village scenario & Camping");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
