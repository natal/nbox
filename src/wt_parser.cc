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


WeightParser::WeightParser (Network* network)
    : network_ (network)
{
}

void WeightParser::parse_line_ (const char* line, size_t len, size_t line_nb)
{
    const char sequence[3] = {'(', ',', ')'};
    const char* it = sequence;
    double vals[3] = {0.};
    double* it_v = vals;
    for (size_t i = 0; i < len; i++, line++)
    {
        if (*line != ' ')
        {
            if (*line != *it)
            {
                std::stringstream sstream (std::stringstream::in | std::stringstream::out);
                sstream << "unexpected '" << *line << "' at line " << line_nb;
                throw SyntaxErrorException (sstream.str ());
            }
            std::stringstream sstream (std::stringstream::in | std::stringstream::out);
            sstream << line;
            sstream >> *it_v;
            it_v++;
            it++;
        }
    }
    network_->weight_set (vals[0], vals[1], vals[2]);
}

void WeightParser::load_weights (const char* file)
{
    std::ifstream filestream;
    filestream.open (file);
    std::string line;
    size_t line_nb = 0;


    while (getline (filestream, line))
    {
        parse_line_ (line.c_str (), line.length (), line_nb);
        line_nb++;
    }
}
