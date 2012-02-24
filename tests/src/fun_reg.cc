
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
#include "../../src/headers/activ_fun.hh"
#include "../../src/headers/exceptions.hh"
#include "../../src/headers/parser.hh"
#include <fstream>
#include <string>
#include <stack>

double paraboloid (double x, double y)
{
    return cos(x * y);
}

int main (int argc, char** argv)
{
    if (argc < 1)
        return 1;

    MapParser parser;
    std::cout << std::endl;
    std::cout << "Function learning test program" << std::endl;
    std::cout << std::endl;

    try
    {
        parser.parse_file (argv[1]);
        Network* network = parser.retrieve_network ();
        std::ofstream fs_plot_err;
        std::ofstream fs_plot_res;

        fs_plot_err.open ("err_surf.data");


        size_t icount = network->inputs_count ();
        size_t ocount = network->outputs_count ();

        if (icount != 2 && ocount != 1)
        {
            std::cout << std::endl <<
                "WARNING : the provided neural map must" <<
                " have 2 inputs and one output." << std::endl;
            std::cout << std::endl;
        }

        double* inputs = new double[icount];
        double* outputs = new double[ocount];

        for (double x = -10.; x - 10. <= 0; x += 0.2f)
        {
            for (double y = -10.; y - 10. <= 0; y += 0.2f)
            {
                inputs[0] = x;
                inputs[1] = y;
                network->interpolate (outputs, inputs);
                double val = paraboloid(x, y);
                double err = outputs[0] - val;
                outputs[0] = val;
                network->train (outputs, inputs);
                fs_plot_err << x << " " << y << " " << err << std::endl;
            }
        }
        fs_plot_err.close ();

        char choice = 'n';

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
        delete[] inputs;
        delete[] outputs;
    }
    catch (NoPerceptronException ex)
    {
        std::cout << ex.what () << std::endl;
    }
    return 0;
}
