#include <iostream>
#include "../../src/headers/network.hh"
#include "../../src/headers/activ_fun.hh"
#include "../../src/headers/exceptions.hh"
#include "../../src/headers/parser.hh"
#include <fstream>
#include <string>
#include <stack>

double xor_fun (int x, int y)
{
    return  (double)(x != y);
}


int main (int argc, char** argv)
{
    if (argc < 1)
        return 1;

    MapParser parser;
    std::cout << std::endl;
    std::cout << "Function learning test program" << std::endl;
    std::cout << "Learning xor" << std::endl;
    std::cout << std::endl;

    try
    {
        parser.parse_file (argv[1]);
        Network* network = parser.retrieve_network ();
        std::ofstream fs_plot_err;

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

        double nb_iter = 10000.;
        double delta = 1. / (nb_iter);

        for (unsigned iter = 0; iter < nb_iter; ++iter)
        {
            double acc_err = 0.;
            for (int x = 0; x <= 1; x++)
            {
                for (int y = 0; y <= 1; y++)
                {
                    inputs[0] = x;
                    inputs[1] = y;
                    network->interpolate (outputs, inputs);
                    double val = xor_fun (x, y);
                    double err = val - outputs[0];
                    err *= err;
                    acc_err += err;
                }
            }

            acc_err /= 101.;
            fs_plot_err << iter << " " << acc_err << std::endl;
            network->adjust_rate (delta);

            for (int x = 0; x <= 1; x++)
            {
                for (int y = 0; y <= 1; y++)
                {
                    inputs[0] = x;
                    inputs[1] = y;
                    double val = xor_fun (x, y);
                    network->train_bp (&val, inputs);
                }
            }

        }
        std::cout << "Network trained" << std::endl;
        std::cout << std::endl;

        for (int x = 0; x <= 1; x++)
        {
            for (int y = 0; y <= 1; y++)
            {
                inputs[0] = x;
                inputs[1] = y;
                network->interpolate (outputs, inputs);
                std::cout << x << " xor " << y << " = ";
                std::cout << outputs[0] << " (" << xor_fun (x, y) << ") "<< std::endl;
            }
        }

        fs_plot_err.close ();

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
