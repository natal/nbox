#include <iostream>
#include "../../src/headers/network.hh"
#include "../../src/headers/exceptions.hh"
#include "../../src/headers/parser.hh"
#include <fstream>
#include <string>
#include <stack>
#include <ctime>
#include <cmath>
#include <cstring>
#include <exception>

#define MAX_MSG_LENGTH 1000
#define THRESHOLD(val) (val >= 0.5 ? 1. : 0.)
#define MIN(a,b) (a > b ? b : a)

using namespace nbx;

static double sqme (double* a,
                    double* b,
                    size_t l)
{
    double length = 0;
    for (size_t i = 0; i < l; i++)
    {
        double diff = b[i] - a[i];
        length += diff * diff;
    }
    return length;
}

class OptionErrorException: public std::exception
{
    public:
        OptionErrorException (const char* error_msg)
        {
            std::string s(error_msg);
            strncpy (msg_, s.c_str (), MIN (s.length (), MAX_MSG_LENGTH));
        }
        virtual const char* what () const throw ()
        {
            return msg_;
        }
    private:
        char msg_[MAX_MSG_LENGTH];
};

static void check_opt (int argc, char** argv)
{
    std::stringstream sstream;
    if (argc < 5 || (argv[1][0] != '-' && argv[1][1] != 'i'))
    {
        sstream << "Usage: ./learn [-iX] [data_file]"
                << " [neural_map_file] [weight_output_file]"
                << std::endl
                << "Where X is the number of iterations to "
                << "be performed in the learning process"
                << std::endl;

        throw OptionErrorException (sstream.str ().c_str ());
    }
}

static size_t get_nb_epochs (char* iter_option)
{
    long i = atol (iter_option + 2);
    const char* msg = "Option Error: not a valid number of iterations";
    if (i <= 0)
        throw OptionErrorException (msg);
    return i;
}

static void check_io_sizes (size_t file_in,
                            size_t file_out,
                            size_t nn_in,
                            size_t nn_out)
{

    if (!file_in)
        throw OptionErrorException ("Syntax Error: Data File, \
                                     Not a valid input size");
    if (!file_out)
        throw OptionErrorException ("Syntax Error: Data File, \
                                     Not a valid output size");
    if (!nn_in)
        throw OptionErrorException ("Syntax Error: Neural Map File, \
                                     Not a valid input size");
    if (!nn_out)
        throw OptionErrorException ("Syntax Error: Neural Map File, \
                                     Not a valid output size");
    if (nn_in != file_in || nn_out != file_out)
    {
        std::stringstream sstream;
        sstream << std::endl <<
            "ERROR : the provided neural map must" <<
            " have " << file_in << " inputs and " <<
            file_out << " outputs." << std::endl;
        throw OptionErrorException (sstream.str ().c_str ());
    }
}

static void save_to_dot (Network* network)
{
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
}


int main (int argc, char** argv)
{

    srand (time (NULL));

    MapParser parser;
    std::cout << std::endl;
    std::cout << "NBOX learning program" << std::endl;
    std::cout << std::endl;
    try
    {
        check_opt (argc, argv);

        std::ifstream fs_data;
        fs_data.open (argv[2]);
        if (!fs_data.is_open ())
        {
            std::cerr << "Error: Could not open data" << std::endl;
            return 1;
        }

        size_t in_size = 0;
        size_t out_size = 0;
        size_t max_samples = 0;

        fs_data >> in_size;
        fs_data >> out_size;
        fs_data >> max_samples;

        // Network building

        parser.parse_file (argv[3]);
        Network* network = parser.build_network (1., "sigmoid");

        size_t icount = network->inputs_count ();
        size_t ocount = network->outputs_count ();

        check_io_sizes (in_size, out_size, icount, ocount);

        double* inputs = new double[icount];
        double* labels = new double[ocount];
        double* outputs = new double[ocount];


        std::cout << "Learning from " << max_samples
                  << " samples" << std::endl;

        // Learning phase


        size_t max_epochs = get_nb_epochs (argv[1]);
        double delta_rate = 1. / (2. * (double)(max_epochs));

        for (size_t epoch = 0; epoch < max_epochs; epoch++)
        {
            double cur_val = 0.;
            size_t component = 0;
            size_t eff_nb_data = 0.;
            unsigned nb_samples = max_samples;
            size_t old_progress = -1;
            size_t vec_size[2] = {icount, ocount};
            int io = 0;
            double acc_err = 0;

            while (fs_data >> cur_val && nb_samples > 0)
            {
                if (!io)
                    inputs[component] = cur_val;
                else
                    labels[component] = cur_val;
                component++;
                if (component >= vec_size[io])
                {
                    // if we've just finished to load an output,
                    // we're ready to train
                    if (io)
                    {
                        network->train_bp (labels, inputs);
                        network->interpolate (outputs, inputs);
                        acc_err += sqme (labels, outputs, ocount);
                        eff_nb_data++;
                        nb_samples--;
                    }
                    // reinitialisation of vector
                    component = 0;
                    io = !io;

                    // progress displaying
                    size_t progress = eff_nb_data * 100.;
                    progress = floor ((double)progress / (double)max_samples);
                    if (progress != old_progress)
                    {
                        std::cout << progress << "% done on epoch no "
                                  << epoch << " out of "
                                  << max_epochs << std::endl;
                        old_progress = progress;
                    }
                }
            }
            network->adjust_rate (delta_rate);

            acc_err /= (double)eff_nb_data;

            std::cout << "Effectively learned from "
                      << eff_nb_data << " samples"
                      << std::endl;
            std::cout << "Square mean error : " << acc_err << std::endl;

            std::cout << (size_t)epoch * 100. / (double)max_epochs
                      << "% epoch done" << std::endl;

            fs_data.close ();
            fs_data.open (argv[2]);
            if (!fs_data.is_open ())
            {
                std::cerr << "Error: data file has been \
                              corrupted during training" << std::endl;
                return 1;
            }
            double dumb = 0;
            fs_data >> dumb;
            fs_data >> dumb;
            fs_data >> dumb;
        }

        std::cout << "Saving weight ouput file..." << std::endl;

        network->save_weights (argv[4]);

        std::cout << "Weight ouput file successfully saved" << std::endl;
        save_to_dot (network);

        fs_data.close ();
        delete[] inputs;
        delete[] outputs;
        delete[] labels;

    }
    catch (std::exception& ex)
    {
        std::cerr << ex.what () << std::endl;
        return 1;
    }
    return 0;
}
