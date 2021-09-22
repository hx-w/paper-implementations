#ifndef __CULLING__
#define __CULLING__

#include "typedef.hpp"
#include "utils.cpp"

bool insideBox(RGB value)
{
    return (value.r > cubex && value.r < cubex + 1
        && value.g > cubey && value.g < cubey + 1
        && value.b > cubez && value.b < cubez + 1);
}

bool insideBox(GIMQuadTreeNode * node)
{
    for(int i = node->startPosX; i < node->startPosX + node->size; i++)
    {
        for(int j = node->startPosY; j < node->startPosY + node->size; j++)
        {
            if(insideBox(myimage2[i][j]))
            {
                return true;
            }
        }
    }

    return false;
}

void traverseTree(GIMQuadTreeNode * node)
{
    // IF the pixels are inside the box, draw them
    if(node->size == 1)
    {
        if(insideBox(myimage2[node->startPosX][node->startPosY]))
        {
            for(int k = 0; k < pixelToTri[node->startPosX][node->startPosY].size(); k++)
            {
                newTriangles[pixelToTri[node->startPosX][node->startPosY][k]]->drawn = true;
            }
        }

        return;
    }
    else if(insideBox(node))
    {
        // Check each quad
        traverseTree(node->lowerLeft);
        traverseTree(node->lowerRight);
        traverseTree(node->upperLeft);
        traverseTree(node->upperRight);
    }
    // Not in box
    else
    {
        if(node->size == 65)
        {
            for(int i = node->startPosX; i < node->startPosX + node->size; i++)
            {
                for(int j = node->startPosY; j < node->startPosY + node->size; j++)
                {
                    myimage2[i][j].r = myimage2[i][j].g = myimage2[i][j].b = 0;
                }
            }
        }
        else
        {
            // Make myimage2 black
            for(int i = node->startPosX; i <= node->startPosX + node->size; i++)
            {
                for(int j = node->startPosY; j <= node->startPosY + node->size; j++)
                {
                    myimage2[i][j].r = myimage2[i][j].g = myimage2[i][j].b = 0;
                }
            }
        }
    }
}

void cull()
{
    // Restore myimage2 using myimage3
    for(int i = 0; i < 65; i++)
    {
        for(int j = 0; j < 65; j++)
        {
            myimage2[i][j] = myimage3[i][j];
        }
    }

    // Reset culling
    for(int i = 0; i < newTriangles.size(); i++)
    {
        newTriangles[i]->drawn = false;
    }

    // Traverse quadTree to determine what will be culled
    traverseTree(quadTree);
}

/* Tree starts from lower-left (like the image) */
GIMQuadTreeNode * createQuadTree(int size, int startX, int startY, RGB grid[65][65])
{
    GIMQuadTreeNode * node = new GIMQuadTreeNode(size, startX, startY);

    if(size == 1)
    {
        node->value.r = grid[startX][startY].r;
        node->value.g = grid[startX][startY].g;
        node->value.b = grid[startX][startY].b;
        
        node->upperLeft = NULL;
        node->upperRight = NULL;
        node->lowerLeft = NULL;
        node->lowerRight = NULL;
        
        return node;
    }
    else
    {
        // Create quads
        node->lowerLeft = createQuadTree(size/2, startX, startY, grid);
        node->lowerRight = createQuadTree(size/2, startX + size/2, startY, grid);
        node->upperLeft = createQuadTree(size/2, startX, startY + size/2, grid);
        node->upperRight = createQuadTree(size/2, startX + size/2, startY + size/2, grid);
        
        return node;
    }
}
#endif