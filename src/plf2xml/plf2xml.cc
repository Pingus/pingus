//  $Id: plf2xml.cc,v 1.3 2000/07/30 01:47:38 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <string>
#include <cstdio>
#include <gnome-xml/parser.h>

#include "../PingusError.hh"
#include "../PLFPLF.hh"
#include "../PSMParser.hh"
#include "plf2xml.hh"

std::string
encode_xml(const std::string& arg_str)
{
  ///xmlEncodeEntitiesReentrant()
  std::string str = arg_str;
  std::string::size_type i;
  //  std::cout << "encode_xml: " << str << std::endl;
 
  i = str.find("<");
  if (i != std::string::npos)
    str.replace(i, 1, "&lt;");

  i = str.find(">");
  if (i != std::string::npos)
    str.replace(i, 1, "&gt;");

  //  std::cout << "encode_xml-done: " << str << std::endl;

  return str;
}

void write_footer(PLF* plf)
{
  puts("\n</pingus-level>");
}

void write_header(PLF* plf)
{
  puts("<?xml version=\"1.0\"?>\n"
       "<pingus-level>\n");
}

void write_resdesc(ResDescriptor resdesc)
{
  printf("    <surface><resource type=\"datafile\">\n"
	 "      <resource-datafile>%s</resource-datafile>\n"
	 "      <resource-ident>%s</resource-ident>\n"
	 "    </resource></surface>\n",
	 resdesc.datafile.c_str(),
	 resdesc.res_name.c_str());
}

void
write_global(PLF* plf)
{
  std::cout  << "<global>\n"
	     << "  <author>" << encode_xml(plf->get_author()) << "</author>\n"
	     << "  <levelname>" << encode_xml(plf->get_levelname()) << "</levelname>\n"
	     << "  <description lang=\"en\">"
	     << encode_xml(plf->get_description())
	     << "</description>\n"
	     << " <number-of-pingus>" << plf->get_pingus() << "</number-of-pingus>\n"
	     << " <number-to-save>" << plf->get_number_to_save() << "</number-to-save>\n"
	     << " <width>" << plf->get_width() << "</width>\n"
    	     << " <height>" << plf->get_height() << "</height>\n"
	     << "</global>\n"
	     << std::endl;
}

void
write_startpos(PLF* plf)
{
  std::cout << "<start>\n"
	    << "  <position>\n"
	    << "    <x-pos>" << plf->get_startx() << "</x-pos>\n"
	    << "    <y-pos>" << plf->get_starty() << "</y-pos>\n"
	    << "  </position>\n"
	    << "</start>\n" << std::endl;
}

void
write_actions(PLF* plf)
{
  vector<ActionData> buttons = plf->get_actions();
  
  std::cout << "<action-list>\n";
  for (vector<ActionData>::iterator i = buttons.begin(); 
       i != buttons.end(); 
       i++)
    {
      std::cout << "  <" << i->name << ">" << i->number_of
		<< "  </" << i->name << ">" << std::endl;
    }
  std::cout << "</action-list>\n\n";
}

void
write_position(int x_pos, int y_pos, int z_pos)
{
  std::cout << "    <position>\n"
	    << "      <x-pos>" << x_pos << "</x-pos>\n"
	    << "      <y-pos>" << y_pos << "</y-pos>\n"
	    << "      <z-pos>" << z_pos << "</z-pos>\n"
	    << "    </position>\n";
}

void
write_position(int x_pos, int y_pos)
{
  std::cout << "    <position>\n"
	    << "      <x-pos>" << x_pos << "</x-pos>\n"
	    << "      <y-pos>" << y_pos << "</y-pos>\n"
	    << "    </position>\n";
}

void
write_direction(EntranceData::EntranceDirection dir)
{
  switch (dir)
    {
    case EntranceData::LEFT:
      std::cout << "    <direction>left</direction>\n" << std::endl;
      break;
    case EntranceData::RIGHT:
      std::cout << "    <direction>right</direction>\n" << std::endl;
      break;
    case EntranceData::MISC:
      std::cout << "    <direction>misc</direction>\n" << std::endl;
      break;
    }
}

void 
write_entrances(PLF* plf)
{
  vector<EntranceData> objects = plf->get_entrance();
  
  for (vector<EntranceData>::iterator i = objects.begin(); 
       i != objects.end(); 
       i++)
    {
      std::cout << "<entrance>\n"
		<< "  <type>" << i->name << "</type>\n"
		<< "  <release-rate>" << i->release_rate << "</release-rate>\n";
      write_direction(i->direction);
      write_position(i->pos.x_pos, i->pos.y_pos, i->pos.z_pos);
      
      std::cout << "</entrance>\n\n";
    }
}

void
write_exits(PLF* plf)
{
  vector<ExitData> exits = plf->get_exit();
  
  for (vector<ExitData>::iterator i = exits.begin(); 
       i != exits.end(); 
       i++)
    {
      std::cout << "<exit>\n";
      write_resdesc(i->desc);
      write_position(i->pos.x_pos, i->pos.y_pos, i->pos.z_pos);
      std::cout << "</exit>\n\n";
    }
}

void
write_hotspots(PLF* plf)
{
  vector<HotspotData> objects = plf->get_hotspot();
  
  for (vector<HotspotData>::iterator i = objects.begin(); 
       i != objects.end(); 
       i++)
    {
      std::cout << "<hotspot>\n";
      write_resdesc(i->desc);
      write_position(i->pos.x_pos, i->pos.y_pos, i->pos.z_pos);
      std::cout << "</hotspot>\n\n";
    }
}

void
write_traps(PLF* plf)
{
  vector<TrapData> objects = plf->get_traps();
  
  for (vector<TrapData>::iterator i = objects.begin(); 
       i != objects.end(); 
       i++)
    {
      std::cout << "<trap>\n"
		<< "  <type>" << i->type << "</type>\n";
      write_position(i->pos.x_pos, i->pos.y_pos, i->pos.z_pos);
      std::cout << "</trap>\n\n";
    }
}


void
write_liquid(PLF* plf)
{
  vector<LiquidData> objects = plf->get_liquids();
  
  for (vector<LiquidData>::iterator i = objects.begin(); 
       i != objects.end(); 
       i++)
    {
      std::cout << "<liquid>\n";
      write_resdesc(i->desc);
      write_position(i->pos.x_pos, i->pos.y_pos, i->pos.z_pos);
      std::cout << "  <width>" << i->width << "</width>" << std::endl;
      std::cout << "</liquid>\n\n";
    }
}

void
write_groundpieces(PSMParser* psm)
{
  vector<SurfaceData> objects = psm->get_surfaces();
  
  ///std::cout << "<groundpiece-list>" << std::endl; 
  for (vector<SurfaceData>::iterator i = objects.begin(); 
       i != objects.end(); 
       i++)
    {
      std::cout << "  <groundpiece type=\"" << i->type_str << "\">\n";
      write_resdesc(i->desc);
      write_position(i->pos.x_pos, i->pos.y_pos);
      std::cout << "  </groundpiece>\n\n";
    }
  ///std::cout << "</groundpiece-list>" << std::endl;
}

void
write_background(PLF* plf)
{
  BackgroundData data = plf->get_background();

  printf("<background>\n");
  write_resdesc(data.desc);
  printf("</background>\n\n");
}

int 
main(int argc, char* argv[])
{
  if (argc != 2)
    {
      puts("Usage: plf2xml FILE");
      exit(0);
    }
  else
    {
      try {
	PLF* plf;
	PSMParser* psm;
	std::string filename = argv[1];

	plf = new PLFPLF(filename);
	psm = new PSMParser();
	psm->parse(filename.substr(0, filename.size() - 4) + ".psm");

	write_header(plf);
	write_global(plf);
	write_startpos(plf);
	write_background(plf);
	write_actions(plf);

	write_entrances(plf);
	write_exits(plf);
	write_hotspots(plf);
	write_liquid(plf);
	write_traps(plf);
      
	write_groundpieces(psm);

	write_footer(plf);

	delete plf;
	delete psm;
      }

      catch (PingusError err) {
	std::cout << "PingusError: " << err.message << std::endl;
      }
    }
}

/* EOF */
