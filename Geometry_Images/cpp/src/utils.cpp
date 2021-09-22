#ifndef __UTILS__
#define __UTILS__
#include <math.h>
#include "typedef.hpp"
/**
 * Convert an x pixel coordinate into an x image coordinate
 * xPixel:  an x pixel coordinate
 */
float p2i_x(int xPixel)
{
    return (2 / (float)GW) * xPixel + 1 / (float)GW - 1;
}

/**
 * Convert a y pixel coordinate into a y image coordinate
 * yPixel:  a y pixel coordinate
 */
float p2i_y(int yPixel)
{
    return (2 / (float)GH) * yPixel + 1 / (float)GH - 1;
}

/**
 * Convert an x image coordinate into an x world coordinate
 * xImage:  an x image coordinate
 */
float i2w_x(float xImage)
{
    return ((float)GW / (float)GH) * xImage;
}

/**
 * Convert a y image coordinate into a y world coordinate
 * yImage:  a y image coordinate
 */
float i2w_y(float yImage)
{
    return yImage;
}

/**
 * Convert an x pixel coordinate into an x world coordinate
 * xPixel:  an x pixel coordinate
 */
float p2w_x(int xPixel)
{
    return i2w_x(p2i_x(xPixel));
}

/**
 * Convert a y pixel coordinate into a y world coordinate
 * yPixel:  a y pixel coordinate
 */
float p2w_y(int yPixel)
{
    return i2w_y(p2i_y(yPixel));
}

/**
 * Computes the dot product of two 3D vectors.
 * v1:  first vector
 * v2:  second vector
 */
float dotProd(Vector3 v1, Vector3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/**
 * Computes the cross product of two 3D vectors.
 * a:  first vector
 * b:  second vector
 * c:  resulting cross product of a and b
 * Postcondition:  c will have the cross product of a and b.
 */
void crossProd(Vector3 a, Vector3 b, Vector3 &c)
{
    c.x = a.y * b.z - a.z * b.y;
    c.y = a.z * b.x - a.x * b.z;
    c.z = a.x * b.y - a.y * b.x;
}

/**
 * Computes the angle between two vectors in radians.
 * v1:  first vector
 * v2:  second vector
 */
float getAngle(Vector3 v1, Vector3 v2)
{
    float len1 = sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
    float len2 = sqrt(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);

    return acos(dotProd(v1, v2) / (len1 * len2));
}

/**
 * Converts an angle in radians to degrees.
 * radian:  the angle to convert
 */
float radToDeg(float radian)
{
    return radian * 180 / PI;
}

/**
 * Calculate the normal on the face given by t.
 * t:  Triangle for normal calculation.
 * Postcondition:  t->normal will contain the normal for t's face.
 */
void calcNormal(Tri *t)
{
    Vector3 w1;
    Vector3 w2;

    w1.x = Vertices[t->v3 - 1]->x - Vertices[t->v2 - 1]->x;
    w1.y = Vertices[t->v3 - 1]->y - Vertices[t->v2 - 1]->y;
    w1.z = Vertices[t->v3 - 1]->z - Vertices[t->v2 - 1]->z;

    w2.x = Vertices[t->v1 - 1]->x - Vertices[t->v2 - 1]->x;
    w2.y = Vertices[t->v1 - 1]->y - Vertices[t->v2 - 1]->y;
    w2.z = Vertices[t->v1 - 1]->z - Vertices[t->v2 - 1]->z;
    crossProd(w1, w2, t->normal);
}

/**
 * Gets a vector given a pixel coordinate.  The resulting vector will be
 * based on a unit sphere.
 * v:  Vector from the origin to the given coordinates.
 * x:  x coordinate
 * y:  y coordinate
 * Precondition: x and y must be in pixel coordinates.
 * Postcondition:  v will store a vector from the origin to the given
 * set of coordinates (translated into world coordinates).
 */
void getVector(Vector3 &v, int x, int y)
{
    // Length of the vector
    float length = sqrt(p2w_x(x) * p2w_x(x) + p2w_y(y) * p2w_y(y));

    // IF the length reaches outside the unit sphere, project it onto the
    // surface of the unit sphere
    if (length > 1.)
    {
        v.x = p2w_x(x) / length;
        v.y = (p2w_y(GH) - 1 - p2w_y(y)) / length;
        v.z = 0.;
    }
    // ELSE the vector is inside the unit sphere
    else
    {
        v.x = p2w_x(x);
        v.y = p2w_y(GH) - 1 - p2w_y(y);
        v.z = sqrt(1 - p2w_x(x) * p2w_x(x) - p2w_y(y) * p2w_y(y));
    }
}

void calcEdges()
{
    int j;
    bool found;

    for (int i = 0; i < Triangles.size(); i++)
    {
        found = false;
        for (j = 0; j < cutPathEdges.size(); j++)
        {
            if ((cutPathEdges[j]->v1 == Triangles[i]->v1 && cutPathEdges[j]->v2 == Triangles[i]->v2) || (cutPathEdges[j]->v1 == Triangles[i]->v2 && cutPathEdges[j]->v2 == Triangles[i]->v1))
                found = true;
        }
        if (!found)
            cutPathEdges.push_back(new Edge(Triangles[i]->v1, Triangles[i]->v2, Vector3(1, 0, 0)));

        found = false;
        for (j = 0; j < cutPathEdges.size(); j++)
        {
            if ((cutPathEdges[j]->v1 == Triangles[i]->v2 && cutPathEdges[j]->v2 == Triangles[i]->v3) || (cutPathEdges[j]->v1 == Triangles[i]->v3 && cutPathEdges[j]->v2 == Triangles[i]->v2))
                found = true;
        }
        if (!found)
            cutPathEdges.push_back(new Edge(Triangles[i]->v2, Triangles[i]->v3, Vector3(1, 0, 0)));

        found = false;
        for (j = 0; j < cutPathEdges.size(); j++)
        {
            if ((cutPathEdges[j]->v1 == Triangles[i]->v1 && cutPathEdges[j]->v2 == Triangles[i]->v3) || (cutPathEdges[j]->v1 == Triangles[i]->v3 && cutPathEdges[j]->v2 == Triangles[i]->v1))
                found = true;
        }
        if (!found)
            cutPathEdges.push_back(new Edge(Triangles[i]->v1, Triangles[i]->v3, Vector3(1, 0, 0)));
    }
}

void copyGeometry()
{
    int i;
    for (i = 0; i < Triangles.size(); i++)
    {
        originalTriangles.push_back(new Tri(Triangles[i]->v1, Triangles[i]->v2, Triangles[i]->v3));
    }

    for (i = 0; i < Vertices.size(); i++)
    {
        originalVertices.push_back(new Vector3(Vertices[i]->x, Vertices[i]->y, Vertices[i]->z));
    }

    for (i = 0; i < VPoints.size(); i++)
    {
        originalVPoints.push_back(new Point3(Vector3(VPoints[i]->x, VPoints[i]->y, VPoints[i]->z)));
    }
}

void makeMyNormalImage()
{
    int myX = 0, myY = 0;

    for (int i = 0; i < TextureNormalImage->sizeX * TextureNormalImage->sizeY * 3; i += 3)
    {
        mynormalimage[myX][myY].r = ((float)((unsigned char)(TextureNormalImage->data[i]))) / 255.0;
        mynormalimage[myX][myY].g = ((float)((unsigned char)(TextureNormalImage->data[i + 1]))) / 255.0;
        mynormalimage[myX][myY].b = ((float)((unsigned char)(TextureNormalImage->data[i + 2]))) / 255.0;

        if (myX < 64)
        {
            myX++;
        }
        else
        {
            i += 1341;
            myX = 0;
            myY++;
        }

        if (myY > 64)
        {
            break;
        }
    }
}

void makeMyImage()
{
    int myX = 0, myY = 0;

    for (int i = 0; i < TextureImage->sizeX * TextureImage->sizeY * 3; i += 3)
    {
        myimage[myX][myY].r = ((float)((unsigned char)(TextureImage->data[i]))) / 255.0;
        myimage[myX][myY].g = ((float)((unsigned char)(TextureImage->data[i + 1]))) / 255.0;
        myimage[myX][myY].b = ((float)((unsigned char)(TextureImage->data[i + 2]))) / 255.0;

        myimage2[myX][myY].r = ((float)((unsigned char)(TextureImage->data[i]))) / 255.0;
        myimage2[myX][myY].g = ((float)((unsigned char)(TextureImage->data[i + 1]))) / 255.0;
        myimage2[myX][myY].b = ((float)((unsigned char)(TextureImage->data[i + 2]))) / 255.0;

        myimage3[myX][myY].r = ((float)((unsigned char)(TextureImage->data[i]))) / 255.0;
        myimage3[myX][myY].g = ((float)((unsigned char)(TextureImage->data[i + 1]))) / 255.0;
        myimage3[myX][myY].b = ((float)((unsigned char)(TextureImage->data[i + 2]))) / 255.0;

        if (myX < 64)
        {
            myX++;
        }
        else
        {
            i += 1341;
            myX = 0;
            myY++;
        }

        if (myY > 64)
        {
            break;
        }
    }
}

/**
 * Calculate the normals for every vertex
 */
void calcAllVertexNormals()
{
    // For each face
    for (int i = 0; i < Triangles.size(); i++)
    {
        // Add that face's normal to each vertex's normal
        VPoints[Triangles[i]->v1 - 1]->normal.x += Triangles[i]->normal.x;
        VPoints[Triangles[i]->v1 - 1]->normal.y += Triangles[i]->normal.y;
        VPoints[Triangles[i]->v1 - 1]->normal.z += Triangles[i]->normal.z;

        VPoints[Triangles[i]->v2 - 1]->normal.x += Triangles[i]->normal.x;
        VPoints[Triangles[i]->v2 - 1]->normal.y += Triangles[i]->normal.y;
        VPoints[Triangles[i]->v2 - 1]->normal.z += Triangles[i]->normal.z;

        VPoints[Triangles[i]->v3 - 1]->normal.x += Triangles[i]->normal.x;
        VPoints[Triangles[i]->v3 - 1]->normal.y += Triangles[i]->normal.y;
        VPoints[Triangles[i]->v3 - 1]->normal.z += Triangles[i]->normal.z;
    }

    // For each vertex
    for (int j = 0; j < Vertices.size(); j++)
    {
        // Normalize that normal
        float length = sqrt(VPoints[j]->normal.x * VPoints[j]->normal.x + VPoints[j]->normal.y * VPoints[j]->normal.y + VPoints[j]->normal.z * VPoints[j]->normal.z);
        VPoints[j]->normal.x /= length;
        VPoints[j]->normal.y /= length;
        VPoints[j]->normal.z /= length;
    }
}

/**
 * Calculate the normals for every vertex (reconstructed mesh)
 */
void calcAllNewVertexNormals()
{
    // For each face
    for (int i = 0; i < newTriangles.size(); i++)
    {
        // Add that face's normal to each vertex's normal
        newVPoints[newTriangles[i]->v1 - 1]->normal.x += newTriangles[i]->normal.x;
        newVPoints[newTriangles[i]->v1 - 1]->normal.y += newTriangles[i]->normal.y;
        newVPoints[newTriangles[i]->v1 - 1]->normal.z += newTriangles[i]->normal.z;

        newVPoints[newTriangles[i]->v2 - 1]->normal.x += newTriangles[i]->normal.x;
        newVPoints[newTriangles[i]->v2 - 1]->normal.y += newTriangles[i]->normal.y;
        newVPoints[newTriangles[i]->v2 - 1]->normal.z += newTriangles[i]->normal.z;

        newVPoints[newTriangles[i]->v3 - 1]->normal.x += newTriangles[i]->normal.x;
        newVPoints[newTriangles[i]->v3 - 1]->normal.y += newTriangles[i]->normal.y;
        newVPoints[newTriangles[i]->v3 - 1]->normal.z += newTriangles[i]->normal.z;
    }

    // For each vertex
    for (int j = 0; j < newVertices.size(); j++)
    {
        // Normalize that normal
        float length = sqrt(newVPoints[j]->normal.x * newVPoints[j]->normal.x + newVPoints[j]->normal.y * newVPoints[j]->normal.y + newVPoints[j]->normal.z * newVPoints[j]->normal.z);
        newVPoints[j]->normal.x /= length;
        newVPoints[j]->normal.y /= length;
        newVPoints[j]->normal.z /= length;
    }
}

#endif