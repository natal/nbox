
/* Interface code for dealing with text properties.
   Copyright (C) 2011-2012 Free Software Foundation, Inc.  This file is part of nbox.

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

double paraboloid (double x)
{
    return sin (x)* 0.5 + 0.5;
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
        Network* network = parser.build_network (1., "sigmoid");


        WeightParser wt(network);
        wt.load_weights("weights2.wt");


        std::ofstream fs_plot_err;
        std::ofstream fs_plot_res;
        std::ofstream fs_plot_fun;

        fs_plot_err.open ("err_surf.data");
        fs_plot_res.open ("res_surf.data");
        fs_plot_fun.open ("fun_surf.data");

        size_t icount = network->inputs_count ();
        size_t ocount = network->outputs_count ();
        double* inputs = new double[icount];
        double* outputs = new double[ocount];

/*
        if (icount != 1 and ocount != 1)
        {
            std::cout << std::endl <<
                "WARNING : the provided neural map must" <<
                " have 1 input and one output." << std::endl;
            std::cout << std::endl;
        }


        double nb_iter = 5000.;
        double delta = 0.01 / (2 * nb_iter);
        for (unsigned iter = 0; iter < nb_iter; ++iter)
        {
            double acc_err = 0.;
            for (double x = -10.; x - 10. <= 0; x += 0.2f)
            {
                inputs[0] = x;
                network->interpolate (outputs, inputs);
                double val = paraboloid (x);
                double err = val - outputs[0];
                err *= err;
                acc_err += err;
            }

            acc_err /= 101.;
            std::cout << "SME : " << acc_err << std::endl;
            fs_plot_err << iter << " " << acc_err << std::endl;
            network->adjust_rate (delta);

            for (double x = -10.; x - 10. <= 0; x += 0.2f)
            {
                inputs[0] = x;
                double val = paraboloid (x);
                network->train_bp (&val, inputs);
            }
        }
        */

        float st1 = 0;
        float st2 = 0;
        for (double x = -20.; x - 20. <= 0; x += 0.2f)
        {
            inputs[0] = st1;
            inputs[1] = st2;
            network->interpolate (outputs, inputs);
            fs_plot_res << x << " " << outputs[0] << std::endl;
            fs_plot_fun << x << " " << paraboloid (x) << std::endl;
            st2 = st1;
            st1 = outputs[0];
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
            fs.precision(7);
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
