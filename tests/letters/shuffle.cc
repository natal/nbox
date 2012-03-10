#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char** argv)
{
    srand (time (NULL));
    std::ofstream ofs;
    std::ifstream ifs1;
    std::ifstream ifs2;

    ifs1.open (argv[1]);
    ifs2.open (argv[2]);
    ofs.open (argv[3]);

    std::string line;

    unsigned long n, m, l1, l2;

    ifs1 >> n;
    ifs1 >> m;
    ifs1 >> l1;
    ifs2 >> n;
    ifs2 >> m;
    ifs2 >> l2;

    ofs << n << std::endl;
    ofs << m << std::endl;
    ofs << l1 + l2 << std::endl;

    while (!ifs1.eof () || !ifs2.eof ())
    {
        if (!ifs1.eof () && rand () % 2 || ifs2.eof ())
        {
            std::getline (ifs1, line);
            if (line != "")
            {
                ofs << line << std::endl;
                std::getline (ifs1, line);
                ofs << line << std::endl;
            }
        }
        else
        {
            std::getline (ifs2, line);
            if (line != "")
            {
                ofs << line << std::endl;
                std::getline (ifs2, line);
                ofs << line << std::endl;
            }
        }
    }

    return 0;
}
