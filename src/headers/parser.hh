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


/*
 ** file: parser.hh
 ** author: benjamin
 ** created on 29/12/11 at 07:12
 */

#ifndef PARSER_HH_
# define PARSER_HH_
# include <iostream>
# include <cassert>
# include <sstream>
# include <fstream>
# include <cstring>
# include "network.hh"
# include "exceptions.hh"

#define NB_TOKENS 2

namespace nbx
{
    class MapParser
    {
        public:
            MapParser ();
            ~MapParser ();
            void parse_file (const char* file);
            Network* build_network (double init_eta, const std::string& kern);

        private:
            void parse_neuron_ (const char* line);
            void parse_input_ (const char* line);
            Network::neuralMap neural_map_;
            std::vector<unsigned> first_layer_;
            char to_lower_ (char c);

            typedef void (MapParser::*parse_mthd) (const char*);
            class TkInfo
            {
                public:
                    TkInfo ()
                        : func (0), token (0)
                    {
                    }
                    TkInfo (parse_mthd f, const char* tk)
                        : func (f), token (tk)
                    {
                    }
                    parse_mthd func;
                    const char* token;
            };
            void next_command_ (const char* line, size_t size);
            TkInfo tk_info[NB_TOKENS];
    };

    class WeightParser
    {
        public:
            WeightParser (Network* network);
            void load_weights (const char* file);
        private:
            void parse_line_ (const char* line, size_t len, size_t line_nb);
            Network* network_;
    };
}

#endif /* !PARSER_HH_ */
