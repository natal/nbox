#include "network.hh"
#include "parser.hh"
#include <fstream>
#include <string>
#include <stack>


int main (int argc, char** argv)
{
  if (argc < 1)
    return 1;

  MapParser parser;
  parser.parse_file (argv[1]);
  return 0;
}
