/*  $Id: xml_plf.cxx,v 1.38 2003/04/19 10:23:17 torangan Exp $
 *
 *  Pingus - A free Lemmings clone
 *  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "xmlgz_reader.hxx"
#include <iostream>
#include <stdio.h>
#include <zlib.h>

using namespace std;

/** This file implements parsing of gzipped XML files.
 *  The main function for this is  xmlgzParseFile(),
 *  which should behave like libxml's  xmlParseFile() except that
 *  it is capable of handling both gzipped and raw XML.
 */

xmlDocPtr xmlgzParseFile(const char * filename) {
    gzFile in = NULL;
    char* buf = NULL; 
    xmlDocPtr doc = NULL;
    try {
	// 1: Open file.
	in = gzopen(filename, "rb"); // Works for both gzipped and raw files.
	
	if (!in) throw "Cannot open file.";
    
	// 2: Read file:
	int length;
	{
	    int bufsize = 65536; // For starters.
	    buf = (char*) malloc(bufsize);
	    if (!buf) throw "Allocation of buffer memory failed.";
	    int pos=0, status;
	    while (1) {
		status = gzread(in, buf+pos, bufsize-pos);
		if (status <= 0) break; // Error or EOF
		pos += status;
		if (pos==bufsize) {
		    int newbufsize = bufsize*2;
		    buf = (char*) realloc(buf, newbufsize);	
		    if (!buf) throw "Expansion of buffer memory failed.";
		    bufsize = newbufsize;
		}
	    }
	    if (status < 0) throw "Uncompression error.";
	    length = pos;
	}

	// 3: Parse:
	doc = xmlParseMemory(buf, length);
    } catch (const char* str) {
	cout << "Reading the file " << filename << " failed: " << str << endl;
    }

    // Finally, clean up:
    if (buf) free(buf);
    if (in) gzclose(in);
    return doc;
}

