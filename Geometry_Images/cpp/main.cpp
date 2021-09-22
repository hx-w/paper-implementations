/**
 * Lum, Darrell and Zipkin, Greg
 * CSC 570 Final Project
 * Winter 2007, Z. Wood
 *
 * This program creates a cut path for a geometry image in real time.  The cut-path
 * edges are color-coded based on the current step in the algorithm.  The cut-path 
 * is then highlighted on the original mesh when the algorithm is finished. 
 
 * The second part of our program shows the steps for rendering a mesh from a 
 * geometry image and a normal map.  We show the program stepping through the 
 * geometry image pixels and draw the geometry at the same time.  Edges can be turned 
 * on and off by the user.  Once this is done, the user can turn on culling and 
 * move a culling box around to cull triangles and also see the results of the 
 * quad tree in the geometry image.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <map>
#include <time.h>
// #include <windows.h>
#include "src/stream.cpp"
#include "src/interactor.cpp"
#include "src/render.cpp"
#include "src/culling.cpp"
#include "src/algo.cpp"

using namespace std;

#pragma warning (disable:4018) // signed/unsigned mismatch
#pragma warning (disable:4996) // sprintf, sscanf, fopen_s depreciated
#pragma warning (disable:4267) // size_t to int, possible loss of data


int main( int argc, char** argv )
{
    srand(time(0));

    // Set up my window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Geometry Images: Cutting Algorithm Walkthrough + Rendering Walkthrough With Normal Map and Optional Culling");
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Register glut callback functions and menus
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc( mouse );
    glutMotionFunc( mouseMove );
    doMenus();

    // Enable z-buffe
    glEnable(GL_DEPTH_TEST);

    // init_tex();
    LoadTexture("../static/images/bunny4.bmp", 1);
    LoadTextureNormal("../static/images/bunnynormals.bmp", 2);

    // Initialization
    init();
    // Make sure a file to read is specified
    if (argc > 1)
    {
        cout << "file " << argv[1] << endl;
        // Read-in the mesh file specified
        ReadFile(argv[1]);

        // Once the file is parsed find out the maximum extent to center
        // and scale mesh
        max_extent = max_x - min_x;
        if (max_y - min_y > max_extent) max_extent = max_y - min_y;

        center.x = center.x / Vertices.size();
        center.y = center.y / Vertices.size();
        center.z = center.z / Vertices.size();
    }
    else
    {
        cout << "format is: meshparser filename" << endl;
    }
    calcAllVertexNormals();

    // Final project stuff
    copyGeometry();
    calcEdges();
    removeSeedTriangle();
    makeMyImage();
    makeMyNormalImage();
    init_lighting();
    glutMainLoop();
}