
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

double paraboloid (double x)
{
    return -x * x + 2 * x + 2;
}

int main (int argc, char** argv)
{
    if (argc < 1)
        return 1;

    MapParser parser;
    std::cout << std::endl;
    std::cout << "Function learning test program" << std::endl;
    std::cout << "Learning curve: polynomial" << std::endl;
    std::cout << std::endl;

    try
    {
        parser.parse_file (argv[1]);
        Network* network = parser.retrieve_network ();
        std::ofstream fs_plot_err;
        std::ofstream fs_plot_res;
        std::ofstream fs_plot_fun;

        fs_plot_err.open ("err_surf.data");
        fs_plot_res.open ("res_surf.data");
        fs_plot_fun.open ("fun_surf.data");


        size_t icount = network->inputs_count ();
        size_t ocount = network->outputs_count ();

        if (icount != 1 && ocount != 1)
        {
            std::cout << std::endl <<
                "WARNING : the provided neural map must" <<
                " have 1 input and one output." << std::endl;
            std::cout << std::endl;
        }

        double* inputs = new double[icount];
        double* outputs = new double[ocount];

        for (unsigned iter = 0; iter < 1000; ++iter)
        {
            double acc_err = 0.;
            for (double x = -10.; x - 10. <= 0; x += 0.2f)
            {
                inputs[0] = x;
                network->interpolate (outputs, inputs);
                double val = paraboloid (x);
                double err = outputs[0] - val;
                err *= err;
                acc_err += err;
                //fs_plot_res << x << " " << outputs[0] << std::endl;
                //network->train (outputs, inputs);
                //fs_plot_fun << x << " " << val << std::endl;
            }
            acc_err /= 101.;
            fs_plot_err << iter << " " << acc_err << std::endl;
            outputs[0] = acc_err;
            for (double x = -10.; x - 10. <= 0; x += 0.2f)
            {
                inputs[0] = x;
                network->train (outputs, inputs);
            }
        }

        fs_plot_err.close ();
        fs_plot_res.close ();
        fs_plot_fun.close ();

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
