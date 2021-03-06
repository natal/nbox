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


/* This file is for testing purposes only */

#include <iostream>
#include "../../src/headers/network.hh"
#include "../../src/headers/exceptions.hh"
#include "../../src/headers/parser.hh"
#include <fstream>
#include <string>
#include <stack>

using namespace nbx;

int main (int argc, char** argv)
{
    if (argc < 1)
        return 1;

    MapParser parser;
    std::cout << "Test program" << std::endl;

    try
    {
        parser.parse_file (argv[1]);
        Network* network = parser.build_network (1., "sigmoid");
        WeightParser wp (network);
        // wp.load_weights ("weights.wt");



        size_t icount = network->inputs_count ();
        size_t ocount = network->outputs_count ();
        double* inputs = new double[icount];
        double* outputs = new double[ocount];

        char choice = 'y';

        while (choice == 'y')
        {
            for (unsigned ipt = 0; ipt < icount; ipt++)
            {
                std::cout << "Enter input no " << ipt << std::endl;
                std::cin >> inputs[ipt];
            }
            network->interpolate(outputs, inputs);

            std::cout << "inputs : " << std::endl;
            for (unsigned i = 0; i < icount - 1; i++)
                std::cout << inputs[i] << ", ";
            std::cout << inputs[icount - 1] << std::endl;

            std::cout << "outputs : " << std::endl;
            for (unsigned i = 0; i < ocount - 1; i++)
                std::cout << outputs[i] << ", ";
            std::cout << outputs[ocount - 1] << std::endl;

            std::cout << "Train ? [y/n]" << std::endl;
            std::cin >> choice;
            if (choice == 'y')
            {
                for (unsigned opt = 0; opt < ocount; opt++)
                {
                    std::cout << "Enter desired output no " << opt << std::endl;
                    std::cin >> outputs[opt];
                }
                network->train_bp (outputs, inputs);
            }
        }

        std::cout << "Save to .dot ? [y/n]" << std::endl;
        std::cin >> choice;
        if (choice == 'y')
        {
            std::ofstream fs;
            fs.precision(2);
            fs << std::fixed;
            std::string rel_path;
            std::cout << "Save to file (Will erase file if already exists): ";
            std::cin >> rel_path;
            std::cout << std::endl;
            fs.open ((rel_path + ".dot").c_str());
            fs << "digraph \"neural map\"" << std::endl;
            fs << "{" << std::endl;
            network->dotify (fs);
            fs << "}" << std::endl;
            fs.close ();
        }
        //network->save_weights ("weights.wt");
        delete[] inputs;
        delete[] outputs;
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what () << std::endl;
        return 1;
    }
    return 0;
}
