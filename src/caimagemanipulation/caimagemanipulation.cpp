/***************************************************************************
                          caimagemanipulation.cpp  -  description
                             -------------------
    begin                : Sun May 28 2000
    release              : Wed Jun 14 2000
    copyright            : (C) 2000 by Andrew Mustun, Benjamin Winkler
    email                : andrew@qcad.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <cmath>
#include <ClanLib/Display/Display/surface.h>
#include <ClanLib/Display/SurfaceProviders/canvas.h>
#include "caimagemanipulation.h"

/** Constructor.
*/
CAImageManipulation::CAImageManipulation()
{
}


/** Destructor.
*/
CAImageManipulation::~CAImageManipulation()
{
}



/** Returns a pointer to a new image, based on 'surface'
    but rotated by an angle of 'degrees' degrees.
    \param surface The original surface
    \param angle Rotation angle in degrees (clock-wise)
    \param exact true: Exact, smooth coloring (calculates color 'between' pixels)
                 false: Inexact but faster.
*/
CL_Surface*
CAImageManipulation::rotate( CL_Surface* surface,
                             float angle,
                             bool exact )
{
  int width = surface->get_width();
  int height = surface->get_height();

  // Create a temporary canvas which contains the original surface:
  CL_Canvas* tmpCan = new CL_Canvas( width, height );
  surface->put_target( 0,0, 0, tmpCan );

  // Create a new canvas which will contain the rotated surface:
  CL_Canvas* newCan = new CL_Canvas( width, height );

  // Calc size in bytes:
  int bpp = tmpCan->get_bytes_per_pixel();
  int size = width * height * bpp;

  // Only 24bit supported:
  //
  if( bpp==4 ) {

    newCan->lock();

    int i;             // i is the index for the rotated data
    int r, g, b, a;
    float xo, yo;      // Original pixel
    float xr, yr;      // Rotated pixel
    float cx, cy;      // Rotation center

    r=g=b=a=0;

    cx = (float)width/2;
    cy = (float)height/2;

    // Store original image:
    //
    unsigned char *oriData = (unsigned char*)tmpCan->get_data();
    unsigned char *newData = (unsigned char*)newCan->get_data();

    // needs to be calculated only once since angle doesn't change!!
    //
    float cos_angle = cos( angle/ARAD );     // rotating it clockwise (!) by angle,
    float sin_angle = sin( angle/ARAD );

    float x0;
    float y0;

    // Rotate:
    //
    for( i=0; i<size; i+=bpp ) {
      xr = getCoordinateX( width, i );
      yr = getCoordinateY( width, i );

      x0 = (xr - cx);         // rotation around origin, so i create a relative
      y0 = (yr - cy);         //   vector by subtracting (cx, cy) ...

      xo = cx +  x0 * cos_angle + y0 * sin_angle;  // ... and add it again after rotation
      yo = cy +  y0 * cos_angle - x0 * sin_angle;

      if( (int)xo>-0.5 && (int)xo<width+0.5 &&
          (int)yo>-0.5 && (int)yo<height+0.5   ) {

        getExactColor( oriData,
                       width, height,
                       xo, yo,
                       &r, &g, &b, &a,
                       exact );

        newData[i  ] = a;
        newData[i+1] = b;
        newData[i+2] = g;
        newData[i+3] = r;
      }
      else {
        newData[i  ] = 0;
        newData[i+1] = 0;
        newData[i+2] = 0;
        newData[i+3] = 0;
      }
    }
    newCan->unlock();
  }
  delete tmpCan;

  // Return new surface based on the canvas:
  //
  return CL_Surface::create( newCan );
}



/** Returns a pointer to a new image, based on 'surface'
    but fliped horizontal or vertical.
    \param horizontal true: Flip horizontal (left to right)
                      false: Flip vertical (upsidedown)
*/
CL_Surface*
CAImageManipulation::flip( CL_Surface* surface, bool horizontal )
{
  // Create a canvas which contains the original surface:
  //
  CL_Canvas* can = new CL_Canvas( surface->get_width(),
                                  surface->get_height() );

  surface->put_target( 0,0, 0, can );

  // Calc size in bytes:
  //
  int bpp = can->get_bytes_per_pixel();
  int size = surface->get_width() * surface->get_height() * bpp;
  int width = surface->get_width();

  // Only 24bit supported:
  //
  if( bpp==4 ) {

    can->lock();

    unsigned char *data = (unsigned char*)can->get_data();
    unsigned char tmp;
    int i, k, x, ind, offs;

    if( horizontal ) {
      for( i=0; i<size; i+=bpp*width ) {
        for( x=0; x<width/2; ++x ) {
          offs = (width-1-x)*bpp;  // Offset to mirrored pixel
          for( k=0; k<bpp; ++k ) {
            ind = i+x*bpp+k;
            tmp            = data[ind];
            data[ind]      = data[i+offs+k];
            data[i+offs+k] = tmp;
          }
        }
      }
    }
    else {
      int rows = bpp * width;
      for( i=0; i<size/2; i+=bpp ) {
        ind = size-(rows*(i/rows))-(rows-i%rows);
        for( k=0; k<bpp; ++k ) {
          tmp         = data[i+k];
          data[i+k]   = data[ind+k];
          data[ind+k] = tmp;
        }
      }
    }
    can->unlock();
  }

  // Return new surface based on the canvas:
  //
  return CL_Surface::create( can );
}



/** Returns a pointer to a new image, based on 'surface'
    but with changed hue. Changing the hue means to "rotate"
    the color spectrum. You can read more about the HSV color
    model on the Internet.
    This method is used to change the color of a sprite
    (e.g. a car or another object). I sudggest to make the
    basic images in a red color-spectrum and create all
    other colors from it.
    For examples, please visit the Trophy homepage developer
    corner (http://)
    \param hue Changing of hue: 0-360
    \param saturation Changing of saturation: -100...100
    \param value Changing of value (Color intensity): -100...100
*/
CL_Surface*
CAImageManipulation::changeHSV( CL_Surface* surface, 
                                int hue, int saturation, int value )
{
  // Create a canvas which contains the original surface:
  //
  CL_Canvas* can = new CL_Canvas( surface->get_width(),
                                  surface->get_height() );

  surface->put_target( 0,0, 0, can );

  // Calc size in bytes:
  //
  int bpp = can->get_bytes_per_pixel();
  int size = surface->get_width() * surface->get_height() * bpp;

  // Only 24bit supported:
  //
  if( bpp==4 ) {

    // Change hue:
    //
    can->lock();

    unsigned char *data = (unsigned char*)can->get_data();
    int r, g, b, a;
    int h, s, v;

    for( int i=0; i<size; i+=bpp ) {
      r = data[i+3];
      g = data[i+2];
      b = data[i+1];
      a = data[i];

      if( a!=0 && (r!=g || r!=b || g!=b) ) {
        rgbToHsv( r, g, b, &h, &s, &v );

        h += hue;
        s += saturation;
        v += value;
        if( h>360 ) h-=360;
        if( s>255 ) s =255;
        if( v>255 ) v =255;
        if( s<  0 ) s =  0;
        if( v<  0 ) v =  0;

        hsvToRgb( h, s, v, &r, &g, &b );

        data[i+3] = (unsigned char)r;
        data[i+2] = (unsigned char)g;
        data[i+1] = (unsigned char)b;
      }

    }
    can->unlock();
  }

  // Return new surface based on the canvas:
  //
  return CL_Surface::create( can );
}



/** Converts an RGB color to a HSV color.
*/
void
CAImageManipulation::rgbToHsv( int r, int g, int b,
                               int *h, int *s, int *v )
{
  int max = r;				// maximum RGB component
  int whatmax = 0;		// r=>0, g=>1, b=>2
  if( g > max ) {
    max = g;
    whatmax = 1;
  }
  if( b > max ) {
    max = b;
    whatmax = 2;
  }

  int min = r;				// find minimum value
  if( g < min ) min = g;
  if( b < min ) min = b;
  int delta = max-min;
  *v = max;
  *s = max ? (510*delta+max)/(2*max) : 0;

  if( *s == 0 ) {
    *h = -1;				// undefined hue
  }
  else {
	  switch( whatmax ) {
	    case 0:				// red is max component
	      if( g >= b )
          *h = (120*(g-b)+delta)/(2*delta);
        else
          *h = (120*(g-b+delta)+delta)/(2*delta) + 300;
        break;
      case 1:				// green is max component
        if( b > r )
          *h = 120 + (120*(b-r)+delta)/(2*delta);
        else
          *h = 60 + (120*(b-r+delta)+delta)/(2*delta);
        break;
      case 2:				// blue is max component
        if ( r > g )
          *h = 240 + (120*(r-g)+delta)/(2*delta);
        else
          *h = 180 + (120*(r-g+delta)+delta)/(2*delta);
        break;
    }
  }
}



/** Converts an HSV color to a RGB color.
*/
void
CAImageManipulation::hsvToRgb( int h, int s, int v,
                               int *r, int *g, int *b )
{
  *r=*g=*b = v;

  if( s == 0 || h == -1 ) {
    ;
  }
  else {
    if( (unsigned int)h >= 360 ) {
      h %= 360;
    }
    int f = h%60;
    h /= 60;
    int p = (2*v*(255-s)+255)/510;
    int q, t;
    if( h&1 ) {
      q = (unsigned int)(2*v*(15300-s*f)+15300)/30600;
      switch( h ) {
        case 1: *r=(int)q; *g=(int)v, *b=(int)p; break;
        case 3: *r=(int)p; *g=(int)q, *b=(int)v; break;
        case 5: *r=(int)v; *g=(int)p, *b=(int)q; break;
      }
    }
    else {
      t = (unsigned int)(2*v*(15300-(s*(60-f)))+15300)/30600;
      switch( h ) {
        case 0: *r=(int)v; *g=(int)t, *b=(int)p; break;
        case 2: *r=(int)p; *g=(int)v, *b=(int)t; break;
        case 4: *r=(int)t; *g=(int)p, *b=(int)v; break;
      }
    }
  }
}



/** Gets the 'exact' color of a position in the image.
    The position can be 'between' pixels. The color gets
    calculated as an intelligent mixture of the hit pixel
    and the pixels around.
    \param data Pointer to the image data (must be 4 bytes per pixel)
    \param x X-Position
    \param y Y-Position
    \param width Image width in pixels
    \param height Image height in pixels
    \param r The red color part gets written into this memory
    \param g The green color part gets written into this memory
    \param b The blue color part gets written into this memory
    \param a The alpha color part gets written into this memory
    \param exact true: Interpolate the color 'between' pixels
                 false: No interpolation but faster.
*/
void
CAImageManipulation::getExactColor( unsigned char* data,
	                                  int width, int height,
	                                  float x, float y,
                                    int *r, int *g, int *b, int *a,
                                    bool exact )
{
  if( exact ) {
    int ix = (int)rint(x);  // rounded position
    int iy = (int)rint(y);  //
    float xPos = x-ix;      // Exact relative position to center of pixel[4]
    float yPos = y-iy;      //
    float rv, gv, bv, av;   // New color values

    int xOffsets[] = { -1,  0,  1,  -1,  0,  1,  -1,  0,  1 };
    int yOffsets[] = { -1, -1, -1,   0,  0,  0,   1,  1,  1 };

    rv = gv = bv = av = 0;

    float totalDistance = 0.0;
    float distance;
    int index;

    for( int c=0; c<9; ++c ) {
      if( ix+xOffsets[c]<width && ix+xOffsets[c]>=0 &&
          iy+yOffsets[c]<height && iy+yOffsets[c]>=0   ) {
        index = getDataPosition( width, ix+xOffsets[c], iy+yOffsets[c] );
        distance = getDistance( xPos, yPos, xOffsets[c], yOffsets[c] );

        if( distance<1.0 ) {
          rv += (1.0 - distance) * data[ index+3 ];
          gv += (1.0 - distance) * data[ index+2 ];
          bv += (1.0 - distance) * data[ index+1 ];
          av += (1.0 - distance) * data[ index   ];
          totalDistance += (1.0 - distance);
        }
      }
    }

    *r = (int)rint(rv / totalDistance);
    *g = (int)rint(gv / totalDistance);
    *b = (int)rint(bv / totalDistance);
    *a = (int)rint(av / totalDistance);
  }

  // not exact alternative solution (much faster but not as smooth):
  //
  else {
    int index = getDataPosition( width, (int) x, (int) y );

    *r = data[ index+3 ];
    *g = data[ index+2 ];
    *b = data[ index+1 ];
    *a = data[ index   ];
  }
}



/** Gets a x coordinate from a data position in a 4bpp image.
    \param pos   The position (index of unsigned char* array)
    \param width Image width in pixels
*/
int
CAImageManipulation::getCoordinateX( int width, int pos )
{
  return ((int)(floor((float)pos/4)))%width;
}



/** Gets a y coordinate from a data position in a 4bpp image.
    \param pos   The position (index of unsigned char* array)
    \param width Image width in pixels
*/
int
CAImageManipulation::getCoordinateY( int width, int pos )
{
  return (int)floor((float)(pos/4)/width);
}



/** Gets the data position (index of unsigned char* array) from
    a given x/y coordinate
    \param x X-Coordinate
    \param y Y-Coordinate
    \param width Image width in pixels
*/
int
CAImageManipulation::getDataPosition( int width, int x, int y )
{
  return (y * width + x) * 4;
}



/** Returns the distance from one point to an other.
*/
float
CAImageManipulation::getDistance( float x1, float y1, float x2, float y2 )
{
  float xd2 = (x2-x1)*(x2-x1);
  float yd2 = (y2-y1)*(y2-y1);
  float d   = xd2+yd2;
  if(d<0.0) d*=-1.0;
  return(sqrt(d));
}




// EOF

