#ifndef __RENDER__
#define __RENDER__
#include <iostream>
#include "typedef.hpp"
#include "utils.cpp"

//general intialization for opengl for depth and texture mapping
void init()
{
    //   glEnable(GL_DEPTH_TEST);
    //   glEnable(GL_TEXTURE_2D);
    max_x = max_y = max_z = FLT_MIN;
    min_x = min_y = min_z = FLT_MAX;
    center.x = 0;
    center.y = 0;
    center.z = 0;
    scaleFactor = 3.6;
    max_extent = 1.0;
    lightScale = 1;
    xw = yw = 0;
    user_material = MATERIAL_GOLD;
    user_object = OBJECT_MESH;
    user_transform = USER_ROTATE;
    user_shade = SHADE_SMOOTH;
    myMatrix[0] = myMatrix[5] = myMatrix[10] = myMatrix[15] = 1.0;
    theta = 0;
    vInitial = Vector3(0., 0., 0.);
    xTri = GW / 2;
    yTri = GH / 2;
    removedEandT = false;
    removedVandE = false;
    finishedInitialCut = false;
    recreating = false;
    recreatedMesh = false;
    cullingon = false;
    cubex = cubey = cubez = 0.0;
    eyez = 3.0;

    stepping_through = false;

    GIMxInd = 0;
    GIMyInd = 0;
}

//initialization for texture mapping
void init_tex()
{
    //glBindTexture(GL_TEXTURE_2D, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

//routines to load in a bmp files - must be 2^nx2^m and a 24bit bmp
GLvoid LoadTexture(char *image_file, int texID)
{

    TextureImage = (Image *)malloc(sizeof(Image));
    if (TextureImage == NULL)
    {
        printf("Error allocating space for image");
        exit(1);
    }
    std::cout << "trying to load " << image_file << std::endl;
    if (!ImageLoad(image_file, TextureImage))
    {
        exit(1);
    }
    /*  2d texture, level of detail 0 (normal), 3 components (red, green, blue),            */
    /*  x size from image, y size from image,                                              */
    /*  border 0 (normal), rgb color data, unsigned byte data, data  */
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 TextureImage->sizeX, TextureImage->sizeY,
                 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); /*  cheap scaling when image bigger than texture */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); /*  cheap scaling when image smalled than texture*/
}

GLvoid LoadTextureNormal(char *image_file, int texID)
{

    TextureNormalImage = (Image *)malloc(sizeof(Image));
    if (TextureNormalImage == NULL)
    {
        printf("Error allocating space for image");
        exit(1);
    }
    std::cout << "trying to load " << image_file << std::endl;
    if (!NormalImageLoad(image_file, TextureNormalImage))
    {
        exit(1);
    }
    /*  2d texture, level of detail 0 (normal), 3 components (red, green, blue),            */
    /*  x size from image, y size from image,                                              */
    /*  border 0 (normal), rgb color data, unsigned byte data, data  */
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 TextureNormalImage->sizeX, TextureNormalImage->sizeY,
                 0, GL_RGB, GL_UNSIGNED_BYTE, TextureNormalImage->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); /*  cheap scaling when image bigger than texture */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); /*  cheap scaling when image smalled than texture*/
}
/**
 * Set up a specific material.
 * materials:  the material to setup.
 */
void materials(materialStruct materials)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, materials.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materials.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materials.specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, materials.shininess);
}

void renderBitmapCharacter(float x, float y, float z, void *font, char *string)
{
    char *c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

/**
 * Initialization calls for opengl for static light.  Lighting will still have
 * to be enabled in order for this to work.
 */
void init_lighting()
{
    // Turn on light0
    glEnable(GL_LIGHT0);

    // Set up the diffuse, ambient and specular components for the light
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);

    // Specify our lighting model as 1 normal per face
    glShadeModel(GL_FLAT);
    glEnable(GL_LIGHTING);
}

/**
 *  Set the light's position.
 */
void pos_light()
{
    glMatrixMode(GL_MODELVIEW);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}

/**
 * Drawing routine to draw triangles as wireframe.
 * t:  Triangle to draw.
 */
void drawTria(Tri *t)
{
    float length = sqrt(t->normal.x * t->normal.x + t->normal.y * t->normal.y + t->normal.z * t->normal.z);
    glShadeModel(GL_SMOOTH);
    /*if(user_shade == SHADE_FILL)
        glColor3f(0.0, 0.0, 0.5);
    if(user_shade == SHADE_FLAT)
        glNormal3f(t->normal.x/length*lightScale/max_extent,
            t->normal.y/length*lightScale/max_extent,
            t->normal.z/length*lightScale/max_extent);*/

    //if(user_shade == SHADE_WIREFRAME)
    //    glBegin(GL_LINE_LOOP);
    //else
    glBegin(GL_POLYGON);

    // Note that the vertices are indexed starting at 0, but the triangles
    // index them starting from 1, so we must offset by -1!!!
    //if(user_shade == SHADE_SMOOTH)
    if (!recreating)
    {

        glNormal3f(VPoints[t->v1 - 1]->normal.x * lightScale / max_extent,
                   VPoints[t->v1 - 1]->normal.y * lightScale / max_extent,
                   VPoints[t->v1 - 1]->normal.z * lightScale / max_extent);
        glVertex3f(Vertices[t->v1 - 1]->x,
                   Vertices[t->v1 - 1]->y,
                   Vertices[t->v1 - 1]->z);

        //if(user_shade == SHADE_SMOOTH)
        glNormal3f(VPoints[t->v2 - 1]->normal.x * lightScale / max_extent,
                   VPoints[t->v2 - 1]->normal.y * lightScale / max_extent,
                   VPoints[t->v2 - 1]->normal.z * lightScale / max_extent);
        glVertex3f(Vertices[t->v2 - 1]->x,
                   Vertices[t->v2 - 1]->y,
                   Vertices[t->v2 - 1]->z);

        //if(user_shade == SHADE_SMOOTH)
        glNormal3f(VPoints[t->v3 - 1]->normal.x * lightScale / max_extent,
                   VPoints[t->v3 - 1]->normal.y * lightScale / max_extent,
                   VPoints[t->v3 - 1]->normal.z * lightScale / max_extent);
        glVertex3f(Vertices[t->v3 - 1]->x,
                   Vertices[t->v3 - 1]->y,
                   Vertices[t->v3 - 1]->z);
        glEnd();
    }
    else
    {
        glNormal3f(newVPoints[t->v1 - 1]->normal.x * lightScale / max_extent,
                   newVPoints[t->v1 - 1]->normal.y * lightScale / max_extent,
                   newVPoints[t->v1 - 1]->normal.z * lightScale / max_extent);
        glVertex3f(newVertices[t->v1 - 1]->x,
                   newVertices[t->v1 - 1]->y,
                   newVertices[t->v1 - 1]->z);

        //if(user_shade == SHADE_SMOOTH)
        glNormal3f(newVPoints[t->v2 - 1]->normal.x * lightScale / max_extent,
                   newVPoints[t->v2 - 1]->normal.y * lightScale / max_extent,
                   newVPoints[t->v2 - 1]->normal.z * lightScale / max_extent);
        glVertex3f(newVertices[t->v2 - 1]->x,
                   newVertices[t->v2 - 1]->y,
                   newVertices[t->v2 - 1]->z);

        //if(user_shade == SHADE_SMOOTH)
        glNormal3f(newVPoints[t->v3 - 1]->normal.x * lightScale / max_extent,
                   newVPoints[t->v3 - 1]->normal.y * lightScale / max_extent,
                   newVPoints[t->v3 - 1]->normal.z * lightScale / max_extent);
        glVertex3f(newVertices[t->v3 - 1]->x,
                   newVertices[t->v3 - 1]->y,
                   newVertices[t->v3 - 1]->z);
        glEnd();
    }
}

void drawGIM()
{
    glDisable(GL_LIGHTING);
    glPointSize(1.0);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, GW, 0.0, GH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_POINTS);
    if (!recreatedMesh)
    {
        for (int i = 0; i < 65; i++)
        {
            for (int j = 0; j < 65; j++)
            {
                glColor3f(myimage[i][j].r, myimage[i][j].g, myimage[i][j].b);
                glVertex2f(i + 735 - 60, j + 95);
            }
        }
    }
    else
    {
        for (int i = 0; i < 65; i++)
        {
            for (int j = 0; j < 65; j++)
            {
                glColor3f(myimage2[i][j].r, myimage2[i][j].g, myimage2[i][j].b);
                glVertex2f(i + 735 - 60, j + 95);
            }
        }
    }
    glEnd();

    glPopMatrix();

    glEnable(GL_LIGHTING);

    reshape(GW, GH);
}

void drawNormalGIM()
{
    glDisable(GL_LIGHTING);
    glPointSize(1.0);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, GW, 0.0, GH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBegin(GL_POINTS);
    for (int i = 0; i < 65; i++)
    {
        for (int j = 0; j < 65; j++)
        {
            glColor3f(mynormalimage[i][j].r, mynormalimage[i][j].g, mynormalimage[i][j].b);
            glVertex2f(i + 735 - 60, j + 6);
        }
    }
    glEnd();

    glPopMatrix();

    glEnable(GL_LIGHTING);

    reshape(GW, GH);
}

void drawEdge(Edge *e)
{
    glBegin(GL_LINES);
    glColor3f(e->color.x, e->color.y, e->color.z);
    glVertex3f(Vertices[e->v1 - 1]->x, Vertices[e->v1 - 1]->y, Vertices[e->v1 - 1]->z);
    glVertex3f(Vertices[e->v2 - 1]->x, Vertices[e->v2 - 1]->y, Vertices[e->v2 - 1]->z);
    glEnd();
}

/**
 * Draw a sphere.
 */
void drawSphere()
{
    glColor3f(1.0, 0.0, 0.0);
    glutWireSphere(0.35, 10, 10);
}

/**
 * Debugging routine which just draws the vertices of the mesh.
 */
void DrawAllVerts()
{
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_POINTS);

    for (int k = 0; k < Vertices.size(); k++)
    {
        glVertex3f(Vertices[k]->x, Vertices[k]->y, Vertices[k]->z);
    }

    glEnd();
}

void drawHUD()
{
    glDisable(GL_LIGHTING);

    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //text
    glColor3f(1.0, 1.0, 1.0);

    if (!removedEandT && !removedVandE)
    {
        static char str1[50] = {0};
        sprintf(str1, "Geometry Images Demo - Cutting Algorithm Part 1");
        renderBitmapCharacter(0.24, 0.97, 1.0, GLUT_BITMAP_HELVETICA_18, str1);
    }
    else if (removedEandT && !removedVandE)
    {
        static char str1[50] = {0};
        sprintf(str1, "Geometry Images Demo - Cutting Algorithm Part 2");
        renderBitmapCharacter(0.24, 0.97, 1.0, GLUT_BITMAP_HELVETICA_18, str1);
    }
    else
    {
        if (!recreating)
        {
            static char str1[50] = {0};
            sprintf(str1, "Geometry Images Demo - Finished Cut-Path");
            renderBitmapCharacter(0.26, 0.97, 1.0, GLUT_BITMAP_HELVETICA_18, str1);
        }
        else
        {
            if (!cullingon)
            {
                static char str1[50] = {0};
                sprintf(str1, "Geometry Images Demo - Mesh Reconstruction");
                renderBitmapCharacter(0.25, 0.97, 1.0, GLUT_BITMAP_HELVETICA_18, str1);
            }
            else
            {
                static char str1[50] = {0};
                sprintf(str1, "Geometry Images Demo - Culling");
                renderBitmapCharacter(0.33, 0.97, 1.0, GLUT_BITMAP_HELVETICA_18, str1);
            }
        }
    }

    if (!removedEandT && !removedVandE)
    {
        static char str2[50] = {0};
        sprintf(str2, "Color Key for cutting algorithm part 1:");
        renderBitmapCharacter(0.01, 0.92, 1.0, GLUT_BITMAP_9_BY_15, str2);
    }
    else if (removedEandT && !removedVandE)
    {
        static char str6[50] = {0};
        sprintf(str6, "Color Key for cutting algorithm part 2:");
        renderBitmapCharacter(0.01, 0.92, 1.0, GLUT_BITMAP_9_BY_15, str6);
    }
    else
    {
        if (!recreating)
        {
            static char str2[50] = {0};
            sprintf(str2, "Color Key for finished cut:");
            renderBitmapCharacter(0.01, 0.92, 1.0, GLUT_BITMAP_9_BY_15, str2);
        }
        else
        {
            static char str2[50] = {0};
            sprintf(str2, "Color Key for reconstruction:");
            renderBitmapCharacter(0.01, 0.92, 1.0, GLUT_BITMAP_9_BY_15, str2);

            static char str19[50] = {0};
            sprintf(str19, "Geometry Image");
            renderBitmapCharacter(0.81, 0.21, 1.0, GLUT_BITMAP_9_BY_15, str19);

            static char str20[50] = {0};
            sprintf(str20, "Normal Map");
            renderBitmapCharacter(0.83, 0.098, 1.0, GLUT_BITMAP_9_BY_15, str20);
        }
    }

    if (!(removedEandT && !removedVandE))
    {
        static char str3[50] = {0};
        glColor3f(0.8, 0.6, 0.0);
        sprintf(str3, "Mesh faces");
        renderBitmapCharacter(0.04, 0.89, 1.0, GLUT_BITMAP_9_BY_15, str3);
    }

    if (recreating && drawReconEdges)
    {
        static char str18[50] = {0};
        glColor3f(1.0, 0.0, 0.0);
        sprintf(str18, "Mesh edges");
        renderBitmapCharacter(0.04, 0.87, 1.0, GLUT_BITMAP_9_BY_15, str18);
    }
    if (cullingon)
    {
        static char str18[50] = {0};
        glColor3f(0.0, 1.0, 1.0);
        sprintf(str18, "Culling box");
        renderBitmapCharacter(0.04, 0.85, 1.0, GLUT_BITMAP_9_BY_15, str18);
    }

    if (!removedEandT && !removedVandE)
    {
        static char str4[50] = {0};
        glColor3f(1.0, 0.0, 0.0);
        sprintf(str4, "Mesh edges");
        renderBitmapCharacter(0.04, 0.87, 1.0, GLUT_BITMAP_9_BY_15, str4);

        static char str5[50] = {0};
        glColor3f(0.0, 1.0, 0.0);
        sprintf(str5, "Possible cut-path edges to be removed");
        renderBitmapCharacter(0.04, 0.85, 1.0, GLUT_BITMAP_9_BY_15, str5);
    }
    else if (removedEandT && !removedVandE)
    {
        static char str4[50] = {0};
        glColor3f(0.0, 1.0, 0.0);
        sprintf(str4, "Cut-path edges");
        renderBitmapCharacter(0.04, 0.89, 1.0, GLUT_BITMAP_9_BY_15, str4);

        static char str5[50] = {0};
        glColor3f(1.0, 0.6, 0.6);
        sprintf(str5, "Possible cut-path edges to be removed");
        renderBitmapCharacter(0.04, 0.87, 1.0, GLUT_BITMAP_9_BY_15, str5);
    }
    else
    {
        if (!recreating)
        {
            static char str4[50] = {0};
            glColor3f(0.0, 1.0, 0.0);
            sprintf(str4, "Cut-path edges");
            renderBitmapCharacter(0.04, 0.87, 1.0, GLUT_BITMAP_9_BY_15, str4);
        }
    }

    glColor3f(1.0, 1.0, 1.0);

    if (removedEandT && removedVandE)
    {
        if (!recreating)
        {
            static char str7[50] = {0};
            sprintf(str7, "Number of edges in cut-path: %d", cutPathEdges.size());
            renderBitmapCharacter(0.6, 0.92, 1.0, GLUT_BITMAP_9_BY_15, str7);
        }
        else
        {
            if (cullingon == false)
            {
                static char str7[50] = {0};
                sprintf(str7, "Number of triangles: %d", newTriangles.size());
                renderBitmapCharacter(0.7, 0.92, 1.0, GLUT_BITMAP_9_BY_15, str7);
            }
            else
            {
                int count = 0;

                static char str7[50] = {0};
                for (int i = 0; i < newTriangles.size(); i++)
                {
                    if (newTriangles[i]->drawn)
                    {
                        count++;
                    }
                }
                sprintf(str7, "Number of triangles drawn: %d", count);
                renderBitmapCharacter(0.63, 0.92, 1.0, GLUT_BITMAP_9_BY_15, str7);
            }
        }
    }
    else
    {
        static char str7[50] = {0};
        sprintf(str7, "Number of triangles: %d", Triangles.size());
        renderBitmapCharacter(0.7, 0.92, 1.0, GLUT_BITMAP_9_BY_15, str7);

        static char str8[50] = {0};
        sprintf(str8, "Number of edges: %d", cutPathEdges.size());
        renderBitmapCharacter(0.7, 0.90, 1.0, GLUT_BITMAP_9_BY_15, str8);
    }

    static char str9[50] = {0};
    sprintf(str9, "Controls:");
    renderBitmapCharacter(0.01, 0.19, 1.0, GLUT_BITMAP_9_BY_15, str9);

    static char str10[50] = {0};
    sprintf(str10, "A: Watch cutting algorithm part 1");
    renderBitmapCharacter(0.04, 0.16, 1.0, GLUT_BITMAP_9_BY_15, str10);

    static char str11[50] = {0};
    sprintf(str11, "S: Watch cutting algorithm part 2");
    renderBitmapCharacter(0.04, 0.14, 1.0, GLUT_BITMAP_9_BY_15, str11);

    static char str13[50] = {0};
    sprintf(str13, "D: Watch reconstruction algorithm");
    renderBitmapCharacter(0.04, 0.12, 1.0, GLUT_BITMAP_9_BY_15, str13);

    static char str14[50] = {0};
    sprintf(str14, "F: Turn on culling");
    renderBitmapCharacter(0.04, 0.10, 1.0, GLUT_BITMAP_9_BY_15, str14);

    static char str15[50] = {0};
    sprintf(str15, "NUMPAD: Move culling box");
    renderBitmapCharacter(0.04, 0.08, 1.0, GLUT_BITMAP_9_BY_15, str15);

    static char str16[50] = {0};
    sprintf(str16, "E: Show reconstructed mesh edges");
    renderBitmapCharacter(0.04, 0.06, 1.0, GLUT_BITMAP_9_BY_15, str16);

    static char str17[50] = {0};
    sprintf(str17, "< >: Camera Movement");
    renderBitmapCharacter(0.04, 0.04, 1.0, GLUT_BITMAP_9_BY_15, str17);

    static char str12[50] = {0};
    sprintf(str12, "SPACEBAR: Step through current algorithm manually");
    renderBitmapCharacter(0.04, 0.02, 1.0, GLUT_BITMAP_9_BY_15, str12);

    glPopMatrix();

    //go back to original viewing
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(60, (float)GW/(float)GH, 1.0, 300.0);
    gluPerspective(90.0, 1.0, 0.05, 15.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, GW, GH);

    glEnable(GL_LIGHTING);
}

/**
 * This function allows image scaling upon window reshape.
 * w:  width of the new window size
 * h:  height of the new window size
 */
void reshape(int w, int h)
{
    GW = w;
    GH = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(90.0, 1.0, 0.05, 15.0);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

void draw_edge(Vector3 start, Vector3 end)
{
    glBegin(GL_LINES);
    glVertex3f(start.x, start.y, start.z);
    glVertex3f(end.x, end.y, end.z);
    glEnd();
}

void draw_cube()
{
    Vector3 v1 = Vector3(cubex + 0, cubey + 0, cubez + 0);
    Vector3 v2 = Vector3(cubex + 0, cubey + 0, cubez + 1);
    Vector3 v3 = Vector3(cubex + 0, cubey + 1, cubez + 0);
    Vector3 v4 = Vector3(cubex + 0, cubey + 1, cubez + 1);
    Vector3 v5 = Vector3(cubex + 1, cubey + 0, cubez + 0);
    Vector3 v6 = Vector3(cubex + 1, cubey + 0, cubez + 1);
    Vector3 v7 = Vector3(cubex + 1, cubey + 1, cubez + 0);
    Vector3 v8 = Vector3(cubex + 1, cubey + 1, cubez + 1);
    draw_edge(v3, v4);
    draw_edge(v3, v7);
    draw_edge(v7, v8);
    draw_edge(v4, v8);
    draw_edge(v1, v5);
    draw_edge(v1, v2);
    draw_edge(v5, v6);
    draw_edge(v2, v6);
    draw_edge(v2, v4);
    draw_edge(v6, v8);
    draw_edge(v5, v7);
    draw_edge(v1, v3);
}

void drawReconstructedMeshEdges()
{
    glLineWidth(2.0);

    for (int i = 0; i < newTriangles.size(); i++)
    {
        draw_edge(*newVertices[newTriangles[i]->v1 - 1], *newVertices[newTriangles[i]->v2 - 1]);
        draw_edge(*newVertices[newTriangles[i]->v1 - 1], *newVertices[newTriangles[i]->v3 - 1]);
        draw_edge(*newVertices[newTriangles[i]->v2 - 1], *newVertices[newTriangles[i]->v3 - 1]);
    }

    glLineWidth(1.0);
}

/**
 * Draw the triangle mesh and sphere.
 */
void drawObjects()
{
    // Transforms for the mesh
    glPushMatrix();

    // IF rotating, update the rotation matrix
    if (user_transform == USER_ROTATE)
    {
        glPushMatrix();
        glLoadIdentity();
        glRotatef(theta, vCross.x, vCross.y, vCross.z);
        glGetFloatv(GL_MODELVIEW_MATRIX, myMatrix);
        glPopMatrix();
    }

    // Order of operations:  scale, rotate, then translate
    glTranslatef(xTrans, yTrans, 0);
    glMultMatrixf(myMatrix);
    if (!recreating)
    {
        glScalef(scaleFactor, scaleFactor, scaleFactor);
    }
    else
    {
    }

    // Scale object to window
    glScalef(1.0 / (float)max_extent, 1.0 / (float)max_extent, 1.0 / (float)max_extent);

    // Translate the object to the orgin
    glTranslatef(-(center.x), -(center.y), -(center.z));

    materials(gold);

    if (removedVandE)
    {
        if (!recreating)
        {
            // Draw the mesh with cut path
            for (int j = 0; j < originalTriangles.size(); j++)
            {
                drawTria(originalTriangles[j]);
            }
        }
        else
        {
            //printf("%d ", newTriangles.size());
            for (int j = 0; j < newTriangles.size(); j++)
            {
                if (newTriangles[j]->drawn)
                {
                    drawTria(newTriangles[j]);
                }
            }
            if (drawReconEdges)
            {
                glDisable(GL_LIGHTING);
                glColor3f(1, 0, 0);
                drawReconstructedMeshEdges();
                glEnable(GL_LIGHTING);
            }
            if (cullingon)
            {
                glDisable(GL_LIGHTING);
                glColor3f(0, 1, 1);
                draw_cube();
                glEnable(GL_LIGHTING);
            }
        }
    }

    // Draw the wireframe mesh
    for (int j = 0; j < Triangles.size(); j++)
    {
        drawTria(Triangles[j]);
    }

    glDisable(GL_LIGHTING);
    glLineWidth(4.0);

    if (!recreating)
    {
        for (int k = 0; k < cutPathEdges.size(); k++)
        {
            drawEdge(cutPathEdges[k]);
        }
    }
    glLineWidth(1.0);
    glEnable(GL_LIGHTING);

    glPopMatrix();

    // Transforms for the sphere
    glPushMatrix();

    // Draw the glut sphere behind the mesh
    glTranslatef(1.25, 0.0, -2.0);
    //drawSphere();
    if (removedVandE && recreating)
    {
        drawGIM();
        drawNormalGIM();
    }

    glPopMatrix();

    drawHUD();
}

/**
 * The display call back.  All drawing should be done in this function.
 */
void display()
{
    float numV = Vertices.size();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Set up the camera
    gluLookAt(0.0, 0.0, eyez, // Eye position
              0.0, 0.0, 0.0,  // Point we are looking at
              0.0, 1.0, 0.0); // Up vector

    // Position the light in the scene
    pos_light();
    drawObjects();
    glPopMatrix();

    glutSwapBuffers();

    glutPostRedisplay();
}
#endif