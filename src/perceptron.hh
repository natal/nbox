/*
** file: perceptron.hh
** author: benjamin
** created on 23/12/11 at 18:43
*/

#ifndef PERCEPTRON_HH_
# define PERCEPTRON_HH_
# include <cmath>
# include <iostream>
# include <cassert>
# include <vector>
# include <fstream>
# include <algorithm>
# include "channel.hh"

class Perceptron
{
  public:
    typedef Channel<Perceptron*, Perceptron*, double> axon;
    Perceptron (int idx);
    ~Perceptron ();
    void connect_to (Perceptron& out);
    // activate the neuron and transmit the neural message to the adjacent neurons:
    // retrieve the neural message from the inputs, multiply by the weights and compute the
    // activation function. Pass the result down to the output neurons.
    void activate ();
    void dotify (std::ofstream& fs);
    int get_index ();
  private:
    std::vector<axon> inputs_;
    std::vector<axon> outputs_;
    double transfer_func_ (double x);
    int index_;
};


#endif /* !PERCEPTRON_HH_ */
