#include "network.hh"
#include "exceptions.hh"
#include "parser.hh"
#include <fstream>
#include <string>
#include <stack>


int main (int argc, char** argv)
{
  if (argc < 1)
    return 1;

  MapParser parser;
  std::cout << "Test program" << std::endl;

  try
  {
    parser.parse_file (argv[1]);
    Network* network = parser.retrieve_network ();

    std::ofstream fs;
    fs.open ("nm.dot");
    fs << "digraph \"neural map\"" << std::endl;
    fs << "{" << std::endl;
    network->dotify (fs);
    fs << "}" << std::endl;
  }
  catch (NoPerceptronException ex)
  {
    std::cout << ex.what () << std::endl;
  }
  return 0;
}
