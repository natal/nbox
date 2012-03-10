#include <iostream>
#include "../../src/headers/network.hh"
#include "../../src/headers/exceptions.hh"
#include "../../src/headers/parser.hh"
#include <fstream>
#include <string>
#include <stack>
#include <ctime>
#include <cstring>

#define MAX_MSG_LENGTH 1000
#define THRESHOLD(val) (val >= 0.5 ? 1. : 0.)
#define MIN(a,b) (a > b ? b : a)

using namespace nbx;

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

static void check_opt (int argc)
{
    std::stringstream sstream;
    if (argc < 4)
    {
        sstream << "Usage: ./test [test_data_file]"
            << " [neural_map_file] [weight_output_file]"
            << std::endl;
        throw OptionErrorException (sstream.str ().c_str ());
    }
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
    if (nn_in != file_in && nn_out != file_out)
    {
        std::stringstream sstream;
        sstream << std::endl <<
            "ERROR : the provided neural map must" <<
            " have " << file_in << " inputs and " <<
            file_out << " outputs." << std::endl;
        throw OptionErrorException (sstream.str ().c_str ());
    }
}

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

static void print_vects (double* a,
                         double* b,
                         size_t al,
                         size_t bl)
{
    for (size_t i = 0; i < al - 1; i++)
        std::cout << a[i] << " ";
    std::cout << a[al - 1] << " ";
    for (size_t i = 0; i < bl - 1; i++)
        std::cout << b[i] << " ";
    std::cout << b[bl - 1] << std::endl;
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
        check_opt (argc);

        std::ifstream fs_data;
        fs_data.open (argv[1]);
        if (!fs_data.is_open ())
        {
            std::cerr << "Error: Could not open testing data" << std::endl;
            return 1;
        }

        size_t in_size = 0;
        size_t out_size = 0;
        size_t max_samples = 0;

        fs_data >> in_size;
        fs_data >> out_size;
        fs_data >> max_samples;

        // Network building

        parser.parse_file (argv[2]);
        Network* network = parser.build_network (1., "sigmoid");

        size_t icount = network->inputs_count ();
        size_t ocount = network->outputs_count ();

        check_io_sizes (in_size, out_size, icount, ocount);

        WeightParser w_parser (network);
        w_parser.load_weights (argv[3]);

        double* inputs = new double[icount];
        double* labels = new double[ocount];
        double* outputs = new double[ocount];


        std::cout << "Testing " << max_samples
            << " samples" << std::endl;

        // Learning phase

        double cur_val = 0.;
        size_t component = 0;
        size_t eff_nb_data = 0.;
        unsigned nb_samples = max_samples;
        size_t vec_size[2] = {icount, ocount};
        int io = 0;
        double square_mean_err = 0.;

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
                    network->interpolate (outputs, inputs);
                    square_mean_err += sqme (labels, outputs, ocount);
                    print_vects (inputs, outputs, icount, ocount);
                    eff_nb_data++;
                    nb_samples--;
                }
                // reinitialisation of vector
                component = 0;
                io = !io;
            }
        }
        square_mean_err /= eff_nb_data;

        std::cout << "Effectively tested "
            << eff_nb_data << " samples"
            << std::endl;

        std::cout << "Square mean error: " << square_mean_err << std::endl;

        fs_data.close ();
        delete[] inputs;
        delete[] labels;
        delete[] outputs;

    }
    catch (std::exception& ex)
    {
        std::cerr << ex.what () << std::endl;
        return 1;
    }
    return 0;
}
