#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>


int main (int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./rtod [history size] [raw file]" << std::endl;
        return 1;
    }
    std::ifstream rawfile;

    struct stat filestatus;
    rawfile.open (argv[2]);
    stat(argv[2], &filestatus );



    char c = 0;
    size_t hist_size = atoi (argv[1]);
    double* hist = new double[hist_size];

    for (size_t i = 0; i < hist_size; i++)
        hist[i] = 0.;

    size_t cur_h = 0;
    long time_v = 0;

    std::cout << hist_size << std::endl;
    std::cout << 1 << std::endl;
    std::cout << (filestatus.st_size - 3) / 2 << std::endl;

    while (rawfile >> c)
    {
        //std::cout << "char : "<< (int)c << std::endl;
        double val = ((double)c + 128) / 255.;

        for (size_t i = 0; i < hist_size - 1; i++)
            hist[i] = hist[i + 1];

        cur_h = (cur_h + 1) % hist_size;
        std::cout << time_v << " ";
        for (size_t s = 0; s < hist_size - 1; s++)
            std::cout << hist[s] << " ";
        std::cout << hist[hist_size - 1] << std::endl << val << std::endl;

        hist[hist_size - 1] = val;
        time_v++;
    }
    return 0;
}
