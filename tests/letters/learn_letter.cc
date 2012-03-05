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
#define MAX_TEST 100

double threshold (double val)
{
    if (val >= 0.5)
        return 1.;
    return 0.;
}

void test_network (Network* network);

int main (int argc, char** argv)
{
    if (argc < 1)
        return 1;

    srand (time (NULL));

    MapParser parser;
    std::cout << std::endl;
    std::cout << "Letter learning test program" << std::endl;
    std::cout << std::endl;
    try
    {
        parser.parse_file (argv[1]);
        Network* network = parser.retrieve_network ();

        std::ifstream fs_letters;
        std::ifstream fs_background;
        fs_letters.open ("letters/lettre.nbox");
        fs_background.open ("letters/fond.nbox");
        if (!fs_letters.is_open () || !fs_background.is_open ())
        {
            std::cerr << "could not open data" << std::endl;
            return 1;
        }

        size_t vec_size = 0;
        size_t err = 0;
        size_t nbl_letters = 0;
        size_t nbl_bg = 0;

        fs_letters >> vec_size;
        fs_background >> err;
        if (err - vec_size != 0)
        {
            std::cerr << "Vector sizes don't match" << std::endl;
            return 1;
        }

        fs_letters >> nbl_letters;
        fs_background >> nbl_bg;

        size_t icount = network->inputs_count ();
        size_t ocount = network->outputs_count ();

        if (icount != vec_size && ocount != 1)
        {
            std::cout << std::endl <<
                "WARNING : the provided neural map must" <<
                " have " << vec_size << " inputs and one output."
                << std::endl;
            std::cout << std::endl;
        }

        double label1 = 1.;
        double label2 = 0.;

        double* inputs_l = new double[icount];
        double* inputs_b = new double[icount];


        std::cout << "Learning from " << nbl_letters << " letter samples" << std::endl;
        std::cout << "Learning from " << nbl_bg << " backg samples" << std::endl;

        // Learning phase



        for (size_t iter = 0; iter < 200; iter++)
        {
            double cur_val_l = 0.;
            double cur_val_b = 0.;
            size_t comp = 0;
            size_t nb_vec = 0.;
            unsigned max_samples = nbl_letters + nbl_bg;
            double delta_rate = 1. / (400 * (double)max_samples);
            size_t old_progress = -1;

            while (fs_background >> cur_val_b && fs_letters >> cur_val_l
                    && max_samples > 0)
            {
                inputs_l[comp] = cur_val_l;
                inputs_b[comp++] = cur_val_b;
                if (comp >= vec_size)
                {
                    network->train_bp (&label1, inputs_l);
                    network->train_bp (&label2, inputs_b);
                    nb_vec += 2;
                    comp = 0;
                    max_samples--;
                    network->adjust_rate (delta_rate);
                    size_t progress = (size_t)(((double)nb_vec * 50.) / (double)nbl_letters);
                    if (progress != old_progress)
                    {
                        std::cout << progress << "% done on epoch no " << iter << std::endl;
                        old_progress = progress;
                    }
                }
            }
            std::cout << "Learned from " << nb_vec << " samples" << std::endl;
            std::cout << (size_t)iter * 0.5f << "% epoch done" << std::endl;

            fs_letters.close ();
            fs_background.close ();
            fs_letters.open ("letters/lettre.nbox");
            fs_background.open ("letters/fond.nbox");
        }

        network->save_weights ("weights.wt");

        test_network (network);

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

        fs_letters.close ();
        fs_background.close ();
        delete[] inputs_l;
        delete[] inputs_b;
    }
    catch (NoPerceptronException ex)
    {
        std::cout << ex.what () << std::endl;
        return 1;
    }
    return 0;
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
