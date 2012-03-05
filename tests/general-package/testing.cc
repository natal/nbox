#include <iostream>
#include "../../src/headers/network.hh"
#include "../../src/headers/activ_fun.hh"
#include "../../src/headers/exceptions.hh"
#include "../../src/headers/parser.hh"
#include <fstream>
#include <string>
#include <stack>
#include <ctime>

#define MAX_ITER 10000
#define THRESHOLD (val) (val >= 0.5 ? 1. : 0.)

int main (int argc, char** argv)
{
}

void test_network (Network* network)
{
        std::ifstream fs_letters;
        std::ifstream fs_background;
        fs_letters.open ("letters/lettre_test.nbox");
        fs_background.open ("letters/fond_test.nbox");

        if (!fs_letters.is_open () || !fs_background.is_open ())
        {
            std::cerr << "could not open test data" << std::endl;
            return;
        }

        size_t vec_size = 0;
        size_t err = 0;
        size_t nbl_letters = 0;
        size_t nbl_bg = 0;

        fs_letters >> vec_size;
        fs_background >> err;
        if (err - vec_size != 0)
        {
            std::cerr << "Vector sizes don't match in test data" << std::endl;
            return;
        }

        fs_letters >> nbl_letters;
        fs_background >> nbl_bg;

        size_t icount = network->inputs_count ();

        double* inputs_l = new double[icount];
        double* inputs_b = new double[icount];

        double output = 0.;

        double cur_val_l = 0.;
        double cur_val_b = 0.;
        size_t comp = 0;
        double success = 0;
        size_t nb_tests = 0;
        unsigned max_samples = nbl_letters;

        std::cout << "Testing on " << nbl_letters << " letter samples" << std::endl;
        std::cout << "Testing on " << nbl_bg << " backg samples" << std::endl;
        // testing phase letters
        while (fs_letters >> cur_val_l && max_samples > 0)
        {
            inputs_l[comp++] = cur_val_l;
            if (comp >= vec_size)
            {
                network->interpolate (&output, inputs_l);
                success += threshold (output);
                comp = 0;
                max_samples--;
                nb_tests++;
            }
        }

        comp = 0;
        max_samples = nbl_bg;

        while (fs_background >> cur_val_b && max_samples > 0)
        {
            inputs_b[comp++] = cur_val_b;
            if (comp >= vec_size)
            {
                network->interpolate (&output, inputs_b);
                success += (1 - threshold (output));
                comp = 0;
                max_samples--;
                nb_tests++;
            }
        }

        std::cout << "Tested " << nb_tests << " samples" << std::endl;
        std::cout << "\nSucceeded on : " << success << " tests" << std::endl;

        std::cout << "Success rate : " << ((double)success * 100.) / (double)nb_tests << "%" << std::endl;
        fs_letters.close ();
        fs_background.close ();
}
