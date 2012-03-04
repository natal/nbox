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

// Hardcoded automata : -?[1-9]+(.[1-9])?(e-?[1-9]+)?
const char* lex_number (const char* line)
{
    const char* it = line;

    while (*it == ' ')
        it++;

    if (*it == '-')
        it++;

    size_t delta = 0;
    while (*it >= '0' && *it <= '9')
    {
        it++;
        delta++;
    }

    if (delta && *it == '.')
    {
        it++;
        while (*it >= '0' && *it <= '9')
            it++;
    }

    if (delta && (*it == 'e' || *it == 'E'))
    {
        it++;
        if (*it == '-')
            it++;
        delta = 0;
        while (*it >= '0' && *it <= '9')
        {
            it++;
            delta++;
        }
        if (!delta)
            it--;
    }
    return it;
}

void WeightParser::parse_line_ (const char* line, size_t len, size_t line_nb)
{
    const char sequence[3] = {'(', ',', ')'};
    const char* it = sequence;
    double vals[3] = {0.};
    double* it_v = vals;
    const char* line_pos = line;
    for (size_t i = 0; i < len; i++)
    {
        if (*line_pos != ' ')
        {
            if (*line_pos != *it)
            {
                std::stringstream sstream (std::stringstream::in | std::stringstream::out);
                sstream << "unexpected '" << *line_pos << "' at line " << line_nb;
                sstream << " character " << (i + 1);
                throw SyntaxErrorException (sstream.str ());
            }
            line_pos++;
            const char* p = lex_number (line_pos);
            *it_v = atof (line_pos);
            it_v++;
            line_pos = p;
            i = (p - line);
            it++;
        }
        else
            line_pos++;
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
