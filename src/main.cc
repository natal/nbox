/* This file is for testing purposes only */

#include "network.hh"
#include "exceptions.hh"
#include "parser.hh"
#include <fstream>
#include <string>
#include <stack>
#define NB_OUTPUTS 2
#define NB_INPUTS 3

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

    double outputs[NB_OUTPUTS];
    double inputs[NB_INPUTS];

    for (unsigned ipt = 0; ipt < NB_INPUTS; ipt++)
    {
      std::cout << "Enter input no " << ipt << std::endl;
      std::cin >> inputs[ipt];
    }
    network->interpolate(outputs, inputs);

    std::cout << "inputs : " << std::endl;
    for (int i = 0; i < NB_INPUTS - 1; i++)
      std::cout << inputs[i] << ", ";
    std::cout << inputs[NB_INPUTS - 1] << std::endl;

    std::cout << "outputs : " << std::endl;
    for (int i = 0; i < NB_OUTPUTS - 1; i++)
      std::cout << outputs[i] << ", ";
    std::cout << outputs[NB_OUTPUTS - 1] << std::endl;

  }
  catch (NoPerceptronException ex)
  {
    std::cout << ex.what () << std::endl;
  }
  return 0;
}
