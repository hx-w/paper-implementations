#ifndef __STREAM__
#define __STREAM__
#include <iostream>
#include "typedef.hpp"
#include "utils.cpp"

/**
 * Open a file for reading coordinates.
 * filename:  the file to open.
 */
void ReadFile(char *filename)
{
  printf("Reading coordinates from %s\n", filename);

  std::ifstream in_f(filename);
  if (!in_f)
  {
    printf("Could not open file %s\n", filename);
  }
  else
  {
    readStream(in_f);
  }
}

/**
 * Process the input stream from an input stream.
 * is:  the stream to read from.
 */
void readStream(std::istream &is)
{
  char str[256];
  for (; is;)
  {
    is >> std::ws;
    is.get(str, sizeof(str));
    if (!is)
      break;
    is.ignore(9999, '\n');
    readLine(str);
  }
}

/**
 * Process each line of input and save vertices and faces appropriately.
 * str:  Input line to process.
 */
void readLine(char *str)
{
  int indx = 0, vi;
  float x, y, z;
  float r, g, b;
  int mat;

  if (str[0] == '#')
  {
    return;
  }

  // Read a vertex or face
  if (str[0] == 'V' && !strncmp(str, "Vertex ", 7))
  {
    Vector3 *v;
    Point3 *p;
    assert(sscanf(str, "Vertex %d %g %g %g", &vi, &x, &y, &z) == 4);
    v = new Vector3(x, y, z);
    p = new Point3(*v);

    // Store the vertex
    Vertices.push_back(v);
    VPoints.push_back(p);
    // House keeping to display in center of the scene
    center.x += v->x;
    center.y += v->y;
    center.z += v->z;

    if (v->x > max_x)
      max_x = v->x;
    if (v->x < min_x)
      min_x = v->x;
    if (v->y > max_y)
      max_y = v->y;
    if (v->y < min_y)
      min_y = v->y;
    if (v->z > max_z)
      max_z = v->z;
    if (v->z < min_z)
      min_z = v->z;
  }
  else if (str[0] == 'F' && !strncmp(str, "Face ", 5))
  {
    Tri *t;
    t = new Tri();
    char *s = str + 4;
    int fi = -1;
    for (int t_i = 0;; t_i++)
    {
      while (*s && isspace(*s))
        s++;
      // If we reach the end of the line break out of the loop
      if (!*s)
        break;

      // Save the position of the current character
      char *beg = s;

      // Advance to next space
      while (*s && isdigit(*s))
        s++;

      // Covert the character to an integer
      int j = atoi(beg);

      // The first number we encounter will be the face index, don't
      //store it
      if (fi < 0)
      {
        fi = j;
        continue;
      }

      // Otherwise process the digit we've grabbed in j as a vertex index
      // The first number will be the face id the following are vertex ids
      if (t_i == 1)
        t->v1 = j;
      else if (t_i == 2)
        t->v2 = j;
      else if (t_i == 3)
        t->v3 = j;

      // If there is more data to process break out
      if (*s == '{')
        break;
    }
    // Possibly process colors if the mesh has colors
    if (*s && *s == '{')
    {
      char *s1 = s + 1;
      std::cout << "trying to parse color " << !strncmp(s1, "rgb", 3) << std::endl;

      // If we're reading off a color
      if (!strncmp(s1, "rgb=", 4))
      {
        // Grab the values of the string
        assert(sscanf(s1, "rgb=(%g %g %g) matid=%d", &r, &g, &b, &mat) == 4);
        t->color.x = r;
        t->color.x = g;
        t->color.x = b;
        std::cout << "set color to: " << r << " " << g << " " << b << std::endl;
      }
    }
    // Calculate the normal for the triangle
    calcNormal(t);

    // Store the triangle read in
    Triangles.push_back(t);
  }
}

/* BMP file loader loads a 24-bit bmp file only */

/*
* getint and getshort are help functions to load the bitmap byte by byte
*/
static unsigned int getint(FILE *fp)
{
  int c, c1, c2, c3;

  /*  get 4 bytes */
  c = getc(fp);
  c1 = getc(fp);
  c2 = getc(fp);
  c3 = getc(fp);

  return ((unsigned int)c) +
         (((unsigned int)c1) << 8) +
         (((unsigned int)c2) << 16) +
         (((unsigned int)c3) << 24);
}

static unsigned int getshort(FILE *fp)
{
  int c, c1;

  /* get 2 bytes*/
  c = getc(fp);
  c1 = getc(fp);

  return ((unsigned int)c) + (((unsigned int)c1) << 8);
}

/*  quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  */

int ImageLoad(char *filename, Image *image)
{
  FILE *file;
  unsigned long size;        /*  size of the image in bytes. */
  unsigned long i;           /*  standard counter. */
  unsigned short int planes; /*  number of planes in image (must be 1)  */
  unsigned short int bpp;    /*  number of bits per pixel (must be 24) */
  char temp;                 /*  used to convert bgr to rgb color. */

  /*  make sure the file is there. */
  if ((file = fopen(filename, "rb")) == NULL)
  {
    printf("File Not Found : %s\n", filename);
    return 0;
  }

  /*  seek through the bmp header, up to the width height: */
  fseek(file, 18, SEEK_CUR);

  /*  No 100% errorchecking anymore!!! */

  /*  read the width */ image->sizeX = getint(file);

  /*  read the height */
  image->sizeY = getint(file);

  /*  calculate the size (assuming 24 bits or 3 bytes per pixel). */
  size = image->sizeX * image->sizeY * 3;

  /*  read the planes */
  planes = getshort(file);
  if (planes != 1)
  {
    printf("Planes from %s is not 1: %u\n", filename, planes);
    return 0;
  }

  /*  read the bpp */
  bpp = getshort(file);
  if (bpp != 24)
  {
    printf("Bpp from %s is not 24: %u\n", filename, bpp);
    return 0;
  }

  /*  seek past the rest of the bitmap header. */
  fseek(file, 24, SEEK_CUR);

  /*  read the data.  */
  image->data = (char *)malloc(size);
  if (image->data == NULL)
  {
    printf("Error allocating memory for color-corrected image data");
    return 0;
  }

  if ((i = fread(image->data, size, 1, file)) != 1)
  {
    printf("Error reading image data from %s.\n", filename);
    return 0;
  }

  for (i = 0; i < size; i += 3)
  { /*  reverse all of the colors. (bgr -> rgb) */
    temp = image->data[i];
    image->data[i] = image->data[i + 2];
    image->data[i + 2] = temp;
  }

  fclose(file); /* Close the file and release the filedes */

  /*  we're done. */
  return 1;
}

int NormalImageLoad(char *filename, Image *image)
{
  FILE *file;
  unsigned long size;        /*  size of the image in bytes. */
  unsigned long i;           /*  standard counter. */
  unsigned short int planes; /*  number of planes in image (must be 1)  */
  unsigned short int bpp;    /*  number of bits per pixel (must be 24) */
  char temp;                 /*  used to convert bgr to rgb color. */

  /*  make sure the file is there. */
  if ((file = fopen(filename, "rb")) == NULL)
  {
    printf("File Not Found : %s\n", filename);
    return 0;
  }

  /*  seek through the bmp header, up to the width height: */
  fseek(file, 18, SEEK_CUR);

  /*  No 100% errorchecking anymore!!! */

  /*  read the width */ image->sizeX = getint(file);

  /*  read the height */
  image->sizeY = getint(file);

  /*  calculate the size (assuming 24 bits or 3 bytes per pixel). */
  size = image->sizeX * image->sizeY * 3;

  /*  read the planes */
  planes = getshort(file);
  if (planes != 1)
  {
    printf("Planes from %s is not 1: %u\n", filename, planes);
    return 0;
  }

  /*  read the bpp */
  bpp = getshort(file);
  if (bpp != 24)
  {
    printf("Bpp from %s is not 24: %u\n", filename, bpp);
    return 0;
  }

  /*  seek past the rest of the bitmap header. */
  fseek(file, 24, SEEK_CUR);

  /*  read the data.  */
  image->data = (char *)malloc(size);
  if (image->data == NULL)
  {
    printf("Error allocating memory for color-corrected image data");
    return 0;
  }

  if ((i = fread(image->data, size, 1, file)) != 1)
  {
    printf("Error reading image data from %s.\n", filename);
    return 0;
  }

  for (i = 0; i < size; i += 3)
  { /*  reverse all of the colors. (bgr -> rgb) */
    temp = image->data[i];
    image->data[i] = image->data[i + 2];
    image->data[i + 2] = temp;
  }

  fclose(file); /* Close the file and release the filedes */

  /*  we're done. */
  return 1;
}

#endif