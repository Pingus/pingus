/***************************************************************************
                          caimagemanipulation.h  -  description
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

#ifndef CAIMAGEMANIPULATION_H
#define CAIMAGEMANIPULATION_H


#define ARAD 57.29577951308232

typedef unsigned int uint;

#ifdef WIN32
#define rint(a) ((int) (a+0.5f))
#endif

class CL_Surface;

/** Provides some functions fo manipulate images (CL_Surfaces).
    @author Andrew Mustun <andrew@qcad.org>
    @author Benjamin Winkler <bentsch@mail.com>
*/
class CAImageManipulation {
public: 
	CAImageManipulation();
	~CAImageManipulation();
	
  static CL_Surface* rotate( CL_Surface* surface, float angle, bool exact=true );
  static CL_Surface* flip( CL_Surface* surface, bool horizontal );
	static CL_Surface* changeHSV( CL_Surface* surface, int hue, int saturation, int value );
	
	static void rgbToHsv( int r, int g, int b,
	                      int *h, int *s, int *v );
	static void hsvToRgb( int h, int s, int v,
	                      int *r, int *g, int *b );
	
	static void getExactColor( unsigned char* data,
	                           int width, int height,
	                           float x, float y,
                             int *r, int *g, int *b, int *a,
                             bool exact=true );

  static int getCoordinateX( int width, int pos );
  static int getCoordinateY( int width, int pos );
  static int getDataPosition( int width, int x, int y );
  static float getDistance( float x1, float y1, float x2, float y2 );
};

#endif




