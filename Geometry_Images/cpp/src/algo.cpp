#ifndef __ALGO__
#define __ALGO__
#include "typedef.hpp"
#include "utils.cpp"

void removeSeedTriangle()
{
    int randTri = ((float)rand()) / RAND_MAX * Triangles.size();
    int i = 0, j;
    std::vector<Tri *>::iterator triItr;

    for (triItr = Triangles.begin(); triItr != Triangles.end(); triItr++)
    {
        if (i == randTri)
        {
            for (j = 0; j < cutPathEdges.size(); j++)
            {
                if ((cutPathEdges[j]->v1 == Triangles[i]->v1 && cutPathEdges[j]->v2 == Triangles[i]->v2) || (cutPathEdges[j]->v1 == Triangles[i]->v2 && cutPathEdges[j]->v2 == Triangles[i]->v1))
                {
                    cutPathEdges[j]->color.x = 0;
                    cutPathEdges[j]->color.y = 1;
                }

                if ((cutPathEdges[j]->v1 == Triangles[i]->v2 && cutPathEdges[j]->v2 == Triangles[i]->v3) || (cutPathEdges[j]->v1 == Triangles[i]->v3 && cutPathEdges[j]->v2 == Triangles[i]->v2))
                {
                    cutPathEdges[j]->color.x = 0;
                    cutPathEdges[j]->color.y = 1;
                }

                if ((cutPathEdges[j]->v1 == Triangles[i]->v1 && cutPathEdges[j]->v2 == Triangles[i]->v3) || (cutPathEdges[j]->v1 == Triangles[i]->v3 && cutPathEdges[j]->v2 == Triangles[i]->v1))
                {
                    cutPathEdges[j]->color.x = 0;
                    cutPathEdges[j]->color.y = 1;
                }
            }
            Triangles.erase(triItr);
            break;
        }
        i++;
    }
}

void createInitialCutPart1()
{
    bool inMoreThanOneTriangle = false;
    bool foundEdge = false;
    int triInd, edgeInd;

    // IF there remains an edge e adjacent to only one triangle t
    //     remove e and t
    for (int i = 0; i < cutPathEdges.size(); i++)
    {
        foundEdge = false;
        for (int j = 0; j < Triangles.size(); j++)
        {
            if ((cutPathEdges[i]->v1 == Triangles[j]->v1 && cutPathEdges[i]->v2 == Triangles[j]->v2) || (cutPathEdges[i]->v1 == Triangles[j]->v2 && cutPathEdges[i]->v2 == Triangles[j]->v1))
            {
                if (!foundEdge)
                {
                    triInd = j;
                    foundEdge = true;
                }
                else
                {
                    inMoreThanOneTriangle = true;
                    break;
                }
            }
            else if ((cutPathEdges[i]->v1 == Triangles[j]->v2 && cutPathEdges[i]->v2 == Triangles[j]->v3) || (cutPathEdges[i]->v1 == Triangles[j]->v3 && cutPathEdges[i]->v2 == Triangles[j]->v2))
            {
                if (!foundEdge)
                {
                    triInd = j;
                    foundEdge = true;
                }
                else
                {
                    inMoreThanOneTriangle = true;
                    break;
                }
            }
            else if ((cutPathEdges[i]->v1 == Triangles[j]->v1 && cutPathEdges[i]->v2 == Triangles[j]->v3) || (cutPathEdges[i]->v1 == Triangles[j]->v3 && cutPathEdges[i]->v2 == Triangles[j]->v1))
            {
                if (!foundEdge)
                {
                    triInd = j;
                    foundEdge = true;
                }
                else
                {
                    inMoreThanOneTriangle = true;
                    break;
                }
            }
            else
            {
                // Do nothing
            }
        }

        // IF one edge found
        if (foundEdge && !inMoreThanOneTriangle)
        {
            edgeInd = i;
            break;
        }
        else
        {
            inMoreThanOneTriangle = false;
        }
    }

    // IF found one edge, remove that edge and triangle
    if (foundEdge && !inMoreThanOneTriangle)
    {
        std::vector<Tri *>::iterator triItr;
        std::vector<Edge *>::iterator edgeItr;
        int edgeNum = 0, triNum = 0;

        for (triItr = Triangles.begin(); triItr != Triangles.end(); triItr++)
        {
            // IF this is the triangle to remove
            if (triInd == triNum)
            {
                // Search for each pair of vertices in cutPathEdges
                for (int i = 0; i < cutPathEdges.size(); i++)
                {
                    if ((cutPathEdges[i]->v1 == Triangles[triInd]->v1 && cutPathEdges[i]->v2 == Triangles[triInd]->v2) || (cutPathEdges[i]->v1 == Triangles[triInd]->v2 && cutPathEdges[i]->v2 == Triangles[triInd]->v1))
                    {
                        cutPathEdges[i]->color.x = 0;
                        cutPathEdges[i]->color.y = 1;
                    }
                    else if ((cutPathEdges[i]->v1 == Triangles[triInd]->v2 && cutPathEdges[i]->v2 == Triangles[triInd]->v3) || (cutPathEdges[i]->v1 == Triangles[triInd]->v3 && cutPathEdges[i]->v2 == Triangles[triInd]->v2))
                    {
                        cutPathEdges[i]->color.x = 0;
                        cutPathEdges[i]->color.y = 1;
                    }
                    else if ((cutPathEdges[i]->v1 == Triangles[triInd]->v1 && cutPathEdges[i]->v2 == Triangles[triInd]->v3) || (cutPathEdges[i]->v1 == Triangles[triInd]->v3 && cutPathEdges[i]->v2 == Triangles[triInd]->v1))
                    {
                        cutPathEdges[i]->color.x = 0;
                        cutPathEdges[i]->color.y = 1;
                    }
                    else
                    {
                        // Do nothing
                    }
                }
                Triangles.erase(triItr);
                break;
            }
            triNum++;
        }

        for (edgeItr = cutPathEdges.begin(); edgeItr != cutPathEdges.end(); edgeItr++)
        {
            // IF this is the edge to remove
            if (edgeInd == edgeNum)
            {
                cutPathEdges.erase(edgeItr);
                break;
            }
            edgeNum++;
        }
    }
    else
    {
        removedEandT = true;
    }
}

void createInitialCutPart2()
{
    // IF there remains a vertex v adjacent to only one edge e
    //     remove v and e
    bool inMoreThanOneEdge = false;
    bool foundVertex = false;
    int vertInd, edgeInd;
    int count = 0;

    // IF there remains a vertex v adjacent to only one edge e
    //     remove v and e
    for (int i = 0; i < Vertices.size(); i++)
    {
        foundVertex = false;
        for (int j = 0; j < cutPathEdges.size(); j++)
        {
            if ((i == cutPathEdges[j]->v1) || (i == cutPathEdges[j]->v2))
            {
                if (!foundVertex)
                {
                    edgeInd = j;
                    foundVertex = true;
                }
                else
                {
                    inMoreThanOneEdge = true;
                    break;
                }
            }
            else
            {
                // Do nothing
            }
        }

        // IF one vertex found
        if (foundVertex && !inMoreThanOneEdge)
        {
            vertInd = i;
            break;
        }
        else
        {
            inMoreThanOneEdge = false;
        }
    }

    // IF found one edge, remove that edge and triangle
    if (foundVertex && !inMoreThanOneEdge)
    {
        std::vector<Vector3 *>::iterator vertItr;
        std::vector<Edge *>::iterator edgeItr;
        int edgeNum = 0, vertNum = 0;

        if (cutPathEdges.size() <= 2)
        {
            removedVandE = true;

            for (int i = 0; i < cutPathEdges.size(); i++)
            {
                cutPathEdges[i]->color.x = 0;
                cutPathEdges[i]->color.y = 1;
                cutPathEdges[i]->color.z = 0;
            }

            return;
        }

        for (edgeItr = cutPathEdges.begin(); edgeItr != cutPathEdges.end(); edgeItr++)
        {
            // IF this is the edge to remove
            if (edgeInd == edgeNum)
            {
                cutPathEdges.erase(edgeItr);
                break;
            }
            edgeNum++;
        }
    }
    else
    {
        removedVandE = true;
    }

    for (int i = 0; i < cutPathEdges.size(); i++)
    {
        bool v1flag = false;
        bool v2flag = false;

        for (int j = 0; j < cutPathEdges.size(); j++)
        {
            if (i != j)
            {
                if (cutPathEdges[i]->v1 == cutPathEdges[j]->v1)
                {
                    v1flag = true;
                }
                else if (cutPathEdges[i]->v1 == cutPathEdges[j]->v2)
                {
                    v1flag = true;
                }
                else if (cutPathEdges[i]->v2 == cutPathEdges[j]->v1)
                {
                    v2flag = true;
                }
                else if (cutPathEdges[i]->v2 == cutPathEdges[j]->v2)
                {
                    v2flag = true;
                }
                else
                {
                    // Do nothing
                }
            }
        }

        if (v1flag && v2flag)
        {
        }
        else
        {
            cutPathEdges[i]->color.x = 1;
            cutPathEdges[i]->color.y = .6;
            cutPathEdges[i]->color.z = .6;
        }
    }
}

void recreateMesh()
{
    //GIMxInd and GIMyInd are indices into the image 2d array of 256x256
    //myimage is the GIM
    float dist1 = sqrt((float)((myimage[GIMxInd][GIMyInd].r - myimage[GIMxInd + 1][GIMyInd + 1].r) * (myimage[GIMxInd][GIMyInd].r - myimage[GIMxInd + 1][GIMyInd + 1].r) + (myimage[GIMxInd][GIMyInd].g - myimage[GIMxInd + 1][GIMyInd + 1].g) * (myimage[GIMxInd][GIMyInd].g - myimage[GIMxInd + 1][GIMyInd + 1].g) + (myimage[GIMxInd][GIMyInd].b - myimage[GIMxInd + 1][GIMyInd + 1].b) * (myimage[GIMxInd][GIMyInd].b - myimage[GIMxInd + 1][GIMyInd + 1].b)));

    float dist2 = sqrt((float)((myimage[GIMxInd + 1][GIMyInd].r - myimage[GIMxInd][GIMyInd + 1].r) * (myimage[GIMxInd + 1][GIMyInd].r - myimage[GIMxInd][GIMyInd + 1].r) + (myimage[GIMxInd + 1][GIMyInd].g - myimage[GIMxInd][GIMyInd + 1].g) * (myimage[GIMxInd + 1][GIMyInd].g - myimage[GIMxInd][GIMyInd + 1].g) + (myimage[GIMxInd + 1][GIMyInd].b - myimage[GIMxInd][GIMyInd + 1].b) * (myimage[GIMxInd + 1][GIMyInd].b - myimage[GIMxInd][GIMyInd + 1].b)));

    // 0 (size - 3)
    newVertices.push_back(new Vector3(myimage[GIMxInd][GIMyInd].r, myimage[GIMxInd][GIMyInd].g, myimage[GIMxInd][GIMyInd].b));
    newVPoints.push_back(new Point3(Vector3(myimage[GIMxInd][GIMyInd].r, myimage[GIMxInd][GIMyInd].g, myimage[GIMxInd][GIMyInd].b)));
    newVPoints.at(newVPoints.size() - 1)->normal.x = mynormalimage[GIMxInd][GIMyInd].r;
    newVPoints.at(newVPoints.size() - 1)->normal.y = mynormalimage[GIMxInd][GIMyInd].g;
    newVPoints.at(newVPoints.size() - 1)->normal.z = mynormalimage[GIMxInd][GIMyInd].b;

    // 1 (size - 2)
    newVertices.push_back(new Vector3(myimage[GIMxInd + 1][GIMyInd].r, myimage[GIMxInd + 1][GIMyInd].g, myimage[GIMxInd + 1][GIMyInd].b));
    newVPoints.push_back(new Point3(Vector3(myimage[GIMxInd + 1][GIMyInd].r, myimage[GIMxInd + 1][GIMyInd].g, myimage[GIMxInd + 1][GIMyInd].b)));
    newVPoints.at(newVPoints.size() - 1)->normal.x = mynormalimage[GIMxInd + 1][GIMyInd].r;
    newVPoints.at(newVPoints.size() - 1)->normal.y = mynormalimage[GIMxInd + 1][GIMyInd].g;
    newVPoints.at(newVPoints.size() - 1)->normal.z = mynormalimage[GIMxInd + 1][GIMyInd].b;

    // 2 (size - 1)
    newVertices.push_back(new Vector3(myimage[GIMxInd][GIMyInd + 1].r, myimage[GIMxInd][GIMyInd + 1].g, myimage[GIMxInd][GIMyInd + 1].b));
    newVPoints.push_back(new Point3(Vector3(myimage[GIMxInd][GIMyInd + 1].r, myimage[GIMxInd][GIMyInd + 1].g, myimage[GIMxInd][GIMyInd + 1].b)));
    newVPoints.at(newVPoints.size() - 1)->normal.x = mynormalimage[GIMxInd][GIMyInd + 1].r;
    newVPoints.at(newVPoints.size() - 1)->normal.y = mynormalimage[GIMxInd][GIMyInd + 1].g;
    newVPoints.at(newVPoints.size() - 1)->normal.z = mynormalimage[GIMxInd][GIMyInd + 1].b;

    // 3 (size)
    newVertices.push_back(new Vector3(myimage[GIMxInd + 1][GIMyInd + 1].r, myimage[GIMxInd + 1][GIMyInd + 1].g, myimage[GIMxInd + 1][GIMyInd + 1].b));
    newVPoints.push_back(new Point3(Vector3(myimage[GIMxInd + 1][GIMyInd + 1].r, myimage[GIMxInd + 1][GIMyInd + 1].g, myimage[GIMxInd + 1][GIMyInd + 1].b)));
    newVPoints.at(newVPoints.size() - 1)->normal.x = mynormalimage[GIMxInd + 1][GIMyInd + 1].r;
    newVPoints.at(newVPoints.size() - 1)->normal.y = mynormalimage[GIMxInd + 1][GIMyInd + 1].g;
    newVPoints.at(newVPoints.size() - 1)->normal.z = mynormalimage[GIMxInd + 1][GIMyInd + 1].b;

    if (dist1 <= dist2)
    {
        // 0, 1, 3
        newTriangles.push_back(new Tri(newVertices.size() - 3, newVertices.size() - 2, newVertices.size()));
        // 0, 2, 3
        newTriangles.push_back(new Tri(newVertices.size() - 3, newVertices.size() - 1, newVertices.size()));
        pixelToTri[GIMxInd][GIMyInd].push_back(newTriangles.size() - 2);
        pixelToTri[GIMxInd][GIMyInd].push_back(newTriangles.size() - 1);
        pixelToTri[GIMxInd + 1][GIMyInd].push_back(newTriangles.size() - 2);
        pixelToTri[GIMxInd][GIMyInd + 1].push_back(newTriangles.size() - 1);
        pixelToTri[GIMxInd + 1][GIMyInd + 1].push_back(newTriangles.size() - 2);
        pixelToTri[GIMxInd + 1][GIMyInd + 1].push_back(newTriangles.size() - 1);
    }
    else
    {
        // 1, 0, 2
        newTriangles.push_back(new Tri(newVertices.size() - 2, newVertices.size() - 3, newVertices.size() - 1));
        // 1, 3, 2
        newTriangles.push_back(new Tri(newVertices.size() - 2, newVertices.size(), newVertices.size() - 1));
        pixelToTri[GIMxInd][GIMyInd].push_back(newTriangles.size() - 2);
        pixelToTri[GIMxInd + 1][GIMyInd].push_back(newTriangles.size() - 1);
        pixelToTri[GIMxInd + 1][GIMyInd].push_back(newTriangles.size() - 2);
        pixelToTri[GIMxInd][GIMyInd + 1].push_back(newTriangles.size() - 1);
        pixelToTri[GIMxInd][GIMyInd + 1].push_back(newTriangles.size() - 2);
        pixelToTri[GIMxInd + 1][GIMyInd + 1].push_back(newTriangles.size() - 1);
    }

    myimage[GIMxInd][GIMyInd].r = myimage[GIMxInd][GIMyInd].g = myimage[GIMxInd][GIMyInd].b = 1.0;
    mynormalimage[GIMxInd][GIMyInd].r = mynormalimage[GIMxInd][GIMyInd].g = mynormalimage[GIMxInd][GIMyInd].b = 1.0;

    if (GIMyInd == 63)
    {
        myimage[GIMxInd][GIMyInd + 1].r = myimage[GIMxInd][GIMyInd + 1].g = myimage[GIMxInd][GIMyInd + 1].b = 1.0;
        mynormalimage[GIMxInd][GIMyInd + 1].r = mynormalimage[GIMxInd][GIMyInd + 1].g = mynormalimage[GIMxInd][GIMyInd + 1].b = 1.0;
    }

    if (GIMxInd < 63)
    {
        GIMxInd++;
    }
    else
    {
        myimage[GIMxInd + 1][GIMyInd].r = myimage[GIMxInd + 1][GIMyInd].g = myimage[GIMxInd + 1][GIMyInd].b = 1.0;
        mynormalimage[GIMxInd + 1][GIMyInd].r = mynormalimage[GIMxInd + 1][GIMyInd].g = mynormalimage[GIMxInd + 1][GIMyInd].b = 1.0;

        GIMxInd = 0;
        GIMyInd++;
        if (GIMyInd > 63)
        {
            myimage[64][64].r = myimage[64][64].g = myimage[64][64].b = 1.0;
            mynormalimage[64][64].r = mynormalimage[64][64].g = mynormalimage[64][64].b = 1.0;

            recreatedMesh = true;
            printf("Finished reconstruction!\n");

            // Build the quad tree
            quadTree = createQuadTree(65, 0, 0, myimage2);
            printf("Built quad tree\n");
        }
    }
    //calcAllNewVertexNormals();
}
#endif