#ifndef __TYPEDEF__
#define __TYPEDEF__

#include <fstream>
#include <vector>
#include <GLUT/glut.h>

#define FLT_MIN 1.1754E-38F
#define FLT_MAX 1.1754E+38F
/** Constants */
// Debugging flag (print out variable values to the console)
const int __DEBUG = 0;
const float PI = 3.14159265358979323846; // PI
const float normScale = 0.01;     // Scaling factor for normals
const float MAX_SCALE = 3.0;      // Maximum scale factor
const float MIN_SCALE = 0.2;      // Minimum scale factor
const float MAX_TRANSLATE = 2.0;  // Maximum translation window
const float MIN_TRANSLATE = -2.0; // Minimum translation window

const int MATERIAL_GOLD = 1;      // Gold surface material
const int MATERIAL_TURQUOISE = 2; // Turqoise surface material
const int MATERIAL_EMERALD = 3;   // Emerald surface material

const int SHADE_WIREFRAME = 4;    // No shading (just display wireframe)
const int SHADE_FILL = 5;         // Fill in polygons
const int SHADE_FLAT = 6;         // Flat (constant) polygon shading
const int SHADE_SMOOTH = 7;       // Smooth (Gouraud) polygon shading

const int OBJECT_MESH = 8;        // User will manipulate the mesh
const int OBJECT_LIGHT = 9;       // User will manipulate the light

const int USER_TRANSLATE = 10;     // User will translate the figure
const int USER_SCALE = 11;        // User will scale the figure
const int USER_ROTATE = 12;       // User will rotate the figure
const int USER_NULL = 13;         // User doesn't wish to transform

/** Flags */
int user_material;  // User's choice of which surface material to use
int user_object;    // User's choice of which object to manipulate
int user_transform; // User's choice of transformation
int user_shade;     // User's choice of how to shade the mesh
bool removedEandT;  // Removed all edges and triangles (cutting part 1)
bool removedVandE;  // Removed all vertices and edges (cutting part 2)
bool finishedInitialCut; // Completed initial mesh cut
bool recreating;    // true if drawing triangles of reconstructed mesh
bool recreatedMesh; // true if mesh has already been reconstructed from GIM
bool stepping_through; // true to prevent multiple simultaneous simulations
bool cullingon;
bool drawReconEdges;

float eyez;

/** Other global variables */
GLfloat myMatrix[16];// Rotation matrix
int GW;             // Global width
int GH;             // Global height
float lightScale;   // Light scaling factor
float scaleFactor;  // Uniform scaling factor
float theta;        // Rotation angle
float xw;           // Mouse click x position in world coordinates
float yw;           // Mouse click y position in world coordinates
int xTri;           // Current figure x position in pixel coordinates
int yTri;           // Current figure y position in pixel coordinates
float xTrans;       // Translation of x movement in world coordinates
float yTrans;       // Translation of y movement in world coordinates
float cubex;
float cubey;
float cubez;
const float CULLING_STEP_SIZE = 0.05;

/*
 * Data structure for the image used for texture mapping.
 */
typedef struct Image {
  unsigned long sizeX;  // Width of image
  unsigned long sizeY;  // Height of image
  char *data;           // RGB values of image
} Image;

Image *TextureImage;    // Holds loaded textures
Image *TextureNormalImage;    // Holds loaded textures

/*
 * Data structure for RGB values.  0 <= r, g, b <= 1.
 */
typedef struct RGB {
  float r;  // Red value
  float g;  // Green value
  float b;  // Blue value
} RGB;

int GIMxInd; // X-coordinate for stepping through mesh reconstruction from GIM
int GIMyInd; // Y-coordinate for stepping through mesh reconstruction from GIM


RGB myimage[65][65]; // Holds a pixel-by-pixel representation of a GIM, loaded from a texture
RGB myimage2[65][65]; // For culling
RGB myimage3[65][65]; // For restoring before culling
RGB mynormalimage[65][65];
std::vector<int> pixelToTri[65][65];

struct GIMQuadTreeNode {
    RGB value;

    int size;
    int startPosX; // Upper-left X
    int startPosY; // Upper-left Y

    /* Quads */
    GIMQuadTreeNode * upperLeft;
    GIMQuadTreeNode * upperRight;
    GIMQuadTreeNode * lowerLeft;
    GIMQuadTreeNode * lowerRight;

    GIMQuadTreeNode(int in_size, int in_startPosX, int in_startPosY) {
        size = in_size;
        startPosX = in_startPosX;
        startPosY = in_startPosY;
    }
};

GIMQuadTreeNode * quadTree;
GIMQuadTreeNode * createQuadTree(int size, int startX, int startY, RGB grid[65][65]);
void cull();

RGB* pixel;  // Temporary holder for a bitmap pixel
int ImageLoad(char *filename, Image *image);
GLvoid LoadTexture(char* image_file, int tex_id);
int NormalImageLoad(char *filename, Image *image);
GLvoid LoadTextureNormal(char* image_file, int tex_id);
void init_tex();
void init();
void makeMyImage();
void makeMyNormalImage();

/**
 * Vector3 is a structure that stores 3D points.
 */
typedef struct Vector3
{
    float x;
    float y;
    float z;

    Vector3(float in_x, float in_y, float in_z) : x(in_x), y(in_y), z(in_z) {}
    Vector3() {}
} Vector3;

/**
 * Point3 is a structure that stores 3D points and normals to those points.
 */
typedef struct Point3
{
    float x;
    float y;
    float z;
    Vector3 normal;

    Point3(Vector3 v_in) : x(v_in.x), y(v_in.y), z(v_in.z)
    {
        normal.x = normal.y = normal.z = 0;
    }
    Point3() {}
} Point3;

/**
 * Tri is a structure that stores triangles whose shape is determined by
 * three vertices.
 */
typedef struct Tri{
    int v1;
    int v2;
    int v3;
    Vector3 normal; // Normal to the face of the triangle
    Vector3 color;
    bool drawn; // true if not culled

    Tri(int in_v1, int in_v2, int in_v3) : v1(in_v1), v2(in_v2), v3(in_v3),
        normal(0, 1, 0), drawn(true) {}
    Tri() : normal(0, 1, 0) {}
} Tri;

/* Mesh variables */
// STL vector to store all the triangles in the mesh
std::vector<Tri *> Triangles;
// STL vector to store all the vertices in the mesh
std::vector<Vector3 *> Vertices;
// STL vector to store all the points in the mesh (redundancy so that normals
// to vertices can be stored)
std::vector<Point3 *> VPoints;

/* Cut-path variables */
typedef struct Edge {
    int v1;
    int v2;
    Vector3 color;

    Edge(int in_v1, int in_v2) : v1(in_v1), v2(in_v2) {}
    Edge(int in_v1, int in_v2, Vector3 in_color) : v1(in_v1), v2(in_v2), color(in_color) {}
} Edge;

std::vector<Edge *> cutPathEdges;
std::vector<Point3 *> originalVPoints;
std::vector<Tri *> originalTriangles;
std::vector<Vector3 *> originalVertices;

std::vector<Tri *> newTriangles;
// STL vector to store all the vertices in the mesh
std::vector<Vector3 *> newVertices;
std::vector<Point3 *> newVPoints;

Vector3 vInitial;   // Initial vector from the origin to the user click
Vector3 vFinal;     // Final vector from the origin to where the user moved
Vector3 vCross;     // Cross product of vInitial and vFinal (in that order)

/** Globals for computing the center point and extent of the model */
Vector3 center;
float max_x, max_y, max_z, min_x, min_y, min_z;
float max_extent;

/** Globals for lighting */
GLfloat light_pos[4] = {1.0, 1.0, 1.5, 1.0}; // Light position

// White light color
GLfloat light_amb[4] = {0.6, 0.6, 0.6, 1.0};  // Light ambience component
GLfloat light_diff[4] = {0.6, 0.6, 0.6, 1.0}; // Light diffuse component
GLfloat light_spec[4] = {0.8, 0.8, 0.8, 1.0}; // Light specular component

int mat = 0;        // Color value from file

/**
 * materialStruct defines the lighting components for materials.
 */
typedef struct materialStruct
{
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess[1];
} materialStruct;

materialStruct gold = {
    {0.24725, 0.1995, 0.0745, 1.0},
    {0.75164, 0.60648, 0.22648, 1.0},
    {0.628281, 0.555802, 0.366065, 1.0},
    {0.4}
};

materialStruct turquoise = {
    {0.1, 0.18725, 0.1745, 1.0},
    {0.396, 0.74151, 0.69102, 1.0},
    {0.297254, 0.30829, 0.306678, 1.0},
    {0.1}
};

materialStruct emerald = {
    {0.0215, 0.1745, 0.0215, 1.0},
    {0.07568, 0.61424, 0.07568, 1.0},
    {0.633, 0.727811, 0.633, 1.0},
    {0.6}
};

/** Forward function declarations */
void crossProd(Vector3 a, Vector3 b, Vector3 &c);
void readLine(char* str);
void readStream(std::istream& is);
void drawTri(Tri * t);
void drawObjects();
void display();
/* Forward declarations */
void createInitialCutPart1();
void createInitialCutPart2();
void recreateMesh();
void reshape(int w, int h);

void stepThroughFirstPart(int x);
void stepThroughSecondPart(int x);
void stepThroughReconstruction(int x);

void materials(materialStruct materials);

#endif