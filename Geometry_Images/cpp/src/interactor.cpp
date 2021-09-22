#ifndef __INTERACTOR__
#define __INTERACTOR__
#include "typedef.hpp"
#include "utils.cpp"

/**
 * Print mouse and keyboard functions to the console.
 */
void printHelp()
{
    printf("Look at the HUD!\n");
}

/**
 * This function provides the keyboard callback.
 * key:  the key that was pressed
 * x:  ?
 * y:  ?
 */
void keyboard(unsigned char key, int x, int y)
{
    int i;
    switch (key)
    {
    case ',':
    case '<':
        eyez += 0.1;
        break;
    case '.':
    case '>':
        eyez -= 0.1;
        break;
    // Reset the object back to its original position
    case 'r':
    case 'R':
        scaleFactor = 1;
        xTrans = yTrans = 0;
        theta = 0;

        for (i = 0; i < 16; i++)
        {
            myMatrix[i] = 0.0;
        }

        myMatrix[0] = myMatrix[5] = myMatrix[10] = myMatrix[15] = 1.0;
        break;
    // Draw edges on reconstructed mesh
    case 'e':
    case 'E':
        if (recreating)
        {
            drawReconEdges = !drawReconEdges;
        }
        break;
    // Help
    case 'h':
    case 'H':
        printHelp();
        break;
    case 32:
        if (!removedEandT)
        {
            createInitialCutPart1();
        }
        else if (!removedVandE)
        {
            createInitialCutPart2();
        }
        else if (!recreatedMesh)
        {
            recreating = true;
            recreateMesh();
        }
        else
        {
        }
        break;
    case 'a':
    case 'A':
        if (!removedEandT && stepping_through == false)
        {
            stepping_through = true;
            glutTimerFunc(0, stepThroughFirstPart, 0);
            //keyboard(32, x, y);
        }
        break;
    case 's':
    case 'S':
        if (!removedVandE && stepping_through == false)
        {
            stepping_through = true;
            glutTimerFunc(0, stepThroughSecondPart, 0);
            //keyboard(32, x, y);
        }
        break;
    case 'd':
    case 'D':
        while (!recreatedMesh && stepping_through == false)
        {
            stepping_through = true;
            recreating = true;
            glutTimerFunc(0, stepThroughReconstruction, 0);
        }
        break;
    case 'f':
    case 'F':
        if (recreatedMesh)
        {
            cullingon = !cullingon;
            if (cullingon)
            {
                //cull();
            }
        }
        break;
    // Move up
    case '8':
        cubey += CULLING_STEP_SIZE;
        cull();
        break;
    // Move right
    case '6':
        cubex += CULLING_STEP_SIZE;
        cull();
        break;
    // Move left
    case '4':
        cubex -= CULLING_STEP_SIZE;
        cull();
        break;
    // Move down
    case '2':
        cubey -= CULLING_STEP_SIZE;
        cull();
        break;
    // Move towards camera
    case '3':
        cubez += CULLING_STEP_SIZE;
        cull();
        break;
    // Move away from camera
    case '9':
        cubez -= CULLING_STEP_SIZE;
        cull();
        break;
    // Quit
    case 'q':
    case 'Q':
        exit(EXIT_SUCCESS);
        break;
    }
    //glutPostRedisplay();
}

/**
 * Calculate the translation given a coordinate of mouse movement.
 * int x:  the x position of the mouse
 * int y:  the y position of the mouse
 * Postcondition:  the translation parameters will be updated depending on how
 * the mouse moved.
 */
void mouseTranslate(int x, int y)
{
    float xTemp = 2 * (p2w_x(x) - p2w_x(xTri));
    float yTemp = 2 * (p2w_y(GH) - 1 - p2w_y(y) - (p2w_y(GH) - 1 - p2w_y(yTri)));

    if (user_object == OBJECT_MESH)
    {
        if (xTrans + xTemp < MAX_TRANSLATE && xTrans + xTemp > MIN_TRANSLATE)
        {
            xTrans += xTemp;
        }
        if (yTrans + yTemp < MAX_TRANSLATE && yTrans + yTemp > MIN_TRANSLATE)
        {
            yTrans += yTemp;
        }
    }
    else // user_object == OBJECT_LIGHT
    {
        light_pos[0] += xTemp;
        light_pos[1] += yTemp;
    }
    xTri = x;
    yTri = y;
}

/**
 * Calculate the scaling factor given a coordinate of mouse movement.
 * int x:  the x position of the mouse
 * int y:  the y position of the mouse
 * Postcondition:  scaleFactor will be updated depending on how the mouse
 * moved.
 */
void mouseScale(int x, int y)
{
    // Distances from the cube's center to each 'point'
    float initialDistance, finalDistance;

    // The initial distance is the distance from the center of the cube to
    // the mouse click
    initialDistance = sqrt((xw - xTrans) * (xw - xTrans) + (yw - yTrans) * (yw - yTrans));
    // The final distance is the distance from the center of the cube to
    // the mouse drag
    finalDistance = sqrt((p2w_x(x) - xTrans) * (p2w_x(x) - xTrans) +
                         ((p2w_y(GH) - 1 - p2w_y(y)) - yTrans) * ((p2w_y(GH) - 1 - p2w_y(y)) - yTrans));

    // IF the user is shrinking the cube
    if (initialDistance > finalDistance && scaleFactor - (initialDistance - finalDistance) >= MIN_SCALE)
    {
        scaleFactor -= initialDistance - finalDistance;
    }
    // ELSE IF the user is enlarging the cube
    else if (finalDistance > initialDistance && scaleFactor + (finalDistance - initialDistance) <= MAX_SCALE)
    {
        scaleFactor += finalDistance - initialDistance;
    }
    xw = p2w_x(x);
    yw = p2w_y(GH) - 1 - p2w_y(y);
}

/**
 * Calculate the axis and degree of rotation given a coordinate of mouse
 * movement.
 * int x:  the x position of the mouse
 * int y:  the y position of the mouse
 * Postcondition:  vFinal, vCross, and theta will be updated depending on how
 * the mouse moved.
 */
void mouseRotate(int x, int y)
{
    getVector(vFinal, x, y);
    crossProd(vInitial, vFinal, vCross);
    theta = radToDeg(getAngle(vInitial, vFinal));
    if (__DEBUG == 1)
    {
        printf("theta = %f\n", theta);
    }
}

/**
 * This function provides the mouse callback.
 * button:  the mouse button that triggered the callback (l or r)
 * state:  the state of the button (up or down)
 * x:  the x position of the mouse
 * y:  the y position of the mouse
 */
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        // IF the left button is clicked
        if (state == GLUT_DOWN)
        {
            if (__DEBUG == 1)
            {
                printf("mouse clicked at %d %d\n", x, y);
            }

            if (user_transform == USER_TRANSLATE)
            {
                xTri = x;
                yTri = y;
            }
            else if (user_transform == USER_SCALE)
            {
                if (user_object == OBJECT_MESH)
                {
                    xw = p2w_x(x);
                    yw = p2w_y(GH) - 1 - p2w_y(y);
                }
            }
            else if (user_transform == USER_ROTATE)
            {
                if (user_object == OBJECT_MESH)
                {
                    getVector(vInitial, x, y);
                    vFinal = vInitial;
                }
            }
        }
    }
}

/**
 * This function provides the mouse movement callback.
 * int x:  the x position of the mouse
 * int y:  the y position of the mouse
 */
void mouseMove(int x, int y)
{
    if (__DEBUG == 1)
    {
        printf("mouse moved at %d %d\n", x, y);
    }

    if (user_transform == USER_TRANSLATE)
    {
        mouseTranslate(x, y);
    }

    if (user_transform == USER_SCALE)
    {
        if (user_object == OBJECT_MESH)
        {
            mouseScale(x, y);
        }
    }

    if (user_transform == USER_ROTATE)
    {
        if (user_object == OBJECT_MESH)
        {
            mouseRotate(x, y);
        }
    }
}

// ----------- menus ----------
/**
 * This function defines a right-click context menu.  Available choices
 * are material type, shading type, object to manipulate, and transformation
 * type.  Each of these choices has a related submenu with more detailed
 * choices.
 * int value:  the menu selection chosen by the user
 **/
void main_menu(int value)
{
    // There are no atomic high-level choices in the main menu
}

/**
 * This function defines the behavior of the "Material type" submenu.  It
 * allows the user to select from "Gold", "Turquoise", and "Emerald". After
 * the selection has been made, the wireframe object will now be colored
 * according to the selection.
 * int value:  the menu selection chosen by the user
 **/
void material_submenu(int value)
{
    user_material = value;
    switch (value)
    {
    case MATERIAL_GOLD:
        materials(gold);
        break;
    case MATERIAL_TURQUOISE:
        materials(turquoise);
        break;
    case MATERIAL_EMERALD:
        materials(emerald);
        break;
    default:
        printf("User selected emerald material.\n");
        break;
    }
    if (__DEBUG == 1)
    {
        if (value == MATERIAL_GOLD)
        {
            printf("User selected gold material.\n");
        }
        else if (value == MATERIAL_TURQUOISE)
        {
            printf("User selected turquoise material.\n");
        }
        else if (value == MATERIAL_EMERALD)
        {
            printf("User selected emerald material.\n");
        }
        else
        {
            printf("Invalid material chosen.\n");
        }
    }
}

/**
 * This function defines the behavior of the "Shade type" submenu.  It
 * allows the user to select from "Wireframe", "Fill", "Flat" and "Smooth".
 * After the selection has been made, the wireframe object will now be
 * shaded according to the selection.
 * int value:  the menu selection chosen by the user
 **/
void shade_submenu(int value)
{
    user_shade = value;
    switch (value)
    {
    case SHADE_WIREFRAME:
        break;
    case SHADE_FILL:
        break;
    case SHADE_FLAT:
        glShadeModel(GL_FLAT);
        break;
    case SHADE_SMOOTH:
        glShadeModel(GL_SMOOTH);
        break;
    }
    if (__DEBUG == 1)
    {
        if (value == SHADE_WIREFRAME)
        {
            printf("User selected to display only wireframe.\n");
        }
        else if (value == SHADE_FILL)
        {
            printf("User selected to display filled in polygons.\n");
        }
        else if (value == SHADE_FLAT)
        {
            printf("User selected flat polygon shading.\n");
        }
        else if (value == SHADE_SMOOTH)
        {
            printf("User selected smooth polygon shading.\n");
        }
        else
        {
            printf("Invalid shading style chosen.\n");
        }
    }
}

/**
 * This function defines the behavior of the "Object to manipulate" submenu.
 * It allows the user to select from "Mesh" and "Light".  After the selection
 * has been made, either the wireframe object or the point light source will
 * be manipulable.
 * int value:  the menu selection chosen by the user
 **/
void object_submenu(int value)
{
    user_object = value;
    if (__DEBUG == 1)
    {
        if (value == OBJECT_MESH)
        {
            printf("User selected to manipulate the mesh.\n");
        }
        else if (value == OBJECT_LIGHT)
        {
            printf("User selected to manipulate the light.\n");
        }
        else
        {
            printf("Invalid object to manipulate chosen.\n");
        }
    }
}

/**
 * This function defines the behavior of the "Transformation type" submenu.
 * It allows the user to select from "Translate", "Scale", "Rotate", and
 * "No transform".  After the selection has been made, either the wireframe
 * object or the point light source will be transformable with the mouse.
 * int value:  the menu selection chosen by the user
 **/
void transform_submenu(int value)
{
    user_transform = value;
    if (__DEBUG == 1)
    {
        if (value == USER_TRANSLATE)
        {
            printf("User chose to translate.\n");
        }
        else if (value == USER_SCALE)
        {
            printf("User chose to scale.\n");
        }
        else if (value == USER_ROTATE)
        {
            printf("User chose to rotate.\n");
        }
        else if (value == USER_NULL)
        {
            printf("User chose not to transform.\n");
        }
        else
        {
            printf("Invalid user choice.\n");
        }
    }
}

/**
 * This function will create the right-click context menus.
 **/
void doMenus()
{
    // Identifiers for the menus
    int menu, submenu1, submenu2, submenu3, submenu4;

    // Create the material submenu
    submenu1 = glutCreateMenu(material_submenu);
    glutAddMenuEntry("Gold", MATERIAL_GOLD);
    glutAddMenuEntry("Turquoise", MATERIAL_TURQUOISE);
    glutAddMenuEntry("Emerald", MATERIAL_EMERALD);

    // Create the shading submenu
    submenu2 = glutCreateMenu(shade_submenu);
    glutAddMenuEntry("Wireframe", SHADE_WIREFRAME);
    glutAddMenuEntry("Fill", SHADE_FILL);
    glutAddMenuEntry("Flat", SHADE_FLAT);
    glutAddMenuEntry("Smooth", SHADE_SMOOTH);

    // Create the object to manipulate submenu
    submenu3 = glutCreateMenu(object_submenu);
    glutAddMenuEntry("Mesh", OBJECT_MESH);
    glutAddMenuEntry("Light", OBJECT_LIGHT);

    // Create the transformation submenu
    submenu4 = glutCreateMenu(transform_submenu);
    glutAddMenuEntry("Translate", USER_TRANSLATE);
    glutAddMenuEntry("Scale", USER_SCALE);
    glutAddMenuEntry("Rotate", USER_ROTATE);
    glutAddMenuEntry("No transform", USER_NULL);

    // Create the main menu
    menu = glutCreateMenu(main_menu);
    glutAddSubMenu("Material type", submenu1);
    glutAddSubMenu("Shading type", submenu2);
    glutAddSubMenu("Object to manipulate", submenu3);
    glutAddSubMenu("Transform type", submenu4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void stepThroughFirstPart(int x)
{
    createInitialCutPart1();

    if (!removedEandT)
    {
        glutTimerFunc(0, stepThroughFirstPart, 0);
    }
    else
    {
        stepping_through = false;
    }
}

void stepThroughSecondPart(int x)
{
    createInitialCutPart2();

    if (!removedVandE)
    {
        glutTimerFunc(0, stepThroughSecondPart, 0);
    }
    else
    {
        stepping_through = false;
    }
}

void stepThroughReconstruction(int x)
{
    recreateMesh();

    if (!recreatedMesh)
    {
        glutTimerFunc(0, stepThroughReconstruction, 0);
    }
    else
    {
        stepping_through = false;
    }
}
#endif