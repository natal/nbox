/* Interface code for dealing with text properties.
   Copyright (C) 2011-2012
   Free Software Foundation, Inc.

   This file is part of nbox.

   nbox is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   nbox is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with nbox; see the file COPYING.
   If not, see <http://www.gnu.org/licenses/>.  */


#include "headers/parser.hh"


nbx::MapParser::MapParser ()
{
  tk_info[0] = TkInfo (&nbx::MapParser::parse_input_, "input");
  tk_info[1] = TkInfo (&nbx::MapParser::parse_neuron_, "neuron");
}

nbx::MapParser::~MapParser ()
{
}

void nbx::MapParser::parse_file (const char* file)
{
  std::ifstream filestream;
  filestream.open (file);
  std::string line;


  while (getline (filestream, line))
    next_command_ (line.c_str (), line.length ());
}

nbx::Network* nbx::MapParser::build_network (double init_eta,
                                             const std::string& kern)
{
  Network* network = new Network (first_layer_,
                                  neural_map_,
                                  init_eta,
                                  kern);
  return network;
}

char nbx::MapParser::to_lower_ (char c)
{
  if (c >= 'A' && c <= 'Z')
    return c + ('a' - 'A');
  return c;
}

void nbx::MapParser::parse_neuron_ (const char* line)
{
  std::stringstream sstream (std::stringstream::in | std::stringstream::out);

  sstream << line;

  unsigned next = 0;

  neural_map_.push_back (new std::vector<unsigned> ());
  std::vector<unsigned>* cur_neuron = neural_map_.back ();
  while (sstream >> next)
    cur_neuron->push_back (next);
}

void nbx::MapParser::parse_input_ (const char* line)
{
  first_layer_.push_back (neural_map_.size ());
  parse_neuron_ (line);
}

void nbx::MapParser::next_command_ (const char* line, size_t size)
{
  size_t i = 0;

  std::vector<TkInfo>* old_candidats = new std::vector<TkInfo> ();
  std::vector<TkInfo>* new_candidats = new std::vector<TkInfo> ();

  for (size_t i = 0; i < NB_TOKENS; i++)
    old_candidats->push_back (tk_info[i]);

  // start parsing
  while (line[i] == ' ')
    i++;

  // keep track of the starting postion of the word encoutered
  // size_t start = i;

  do
  {
    while (!old_candidats->empty ())
    {
      TkInfo cur_tk = old_candidats->back ();
      old_candidats->pop_back ();
      if (*cur_tk.token == to_lower_ (line[i]))
        new_candidats->push_back (TkInfo (cur_tk.func, cur_tk.token + 1));
    }
    i++;
    old_candidats->clear ();

    //swapping new and old candidats
    std::vector<TkInfo>* tmp = old_candidats;
    old_candidats = new_candidats;
    new_candidats = tmp;

  } while (!old_candidats->empty () && i < size && line[i] != ' ');

  delete new_candidats;
  if (old_candidats->empty ())
  {
    delete old_candidats;
    throw UnknownTokenException ();
  }
  TkInfo top = old_candidats->back ();
  delete old_candidats;

  // call the appropriate parsing method
  (this->*(top.func)) (&line[i]);
}
