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
# include <queue>
# include <fstream>
# include <algorithm>
# include "channel.hh"

class Perceptron
{
  public:
    typedef Channel<Perceptron*, Perceptron*, double> axon;
    Perceptron (int index, double learning_rate);
    ~Perceptron ();
    void connect_to (Perceptron* out);
    /**
    ** @brief activate the neuron and transmit the neural message
    **         to the adjacent neurons:
    **              retrieve the neural message from the inputs,
    **              multiply by the weights and compute the activation function.
                    Pass the result down to the output neurons.
    */
    void activate ();
    void activate (double input_val);
    void dotify (std::ofstream& fs);
    int get_index ();

    // Marking mecanism

    bool is_marked ();
    void mark ();
    void unmark ();
    /**
    ** @brief measure the perceptron's last activation value
    */
    double measure_av ();


    // Backpropagation components - requires all neurons
    // to be unmarked
    void propagate_err ();
    void adjust_weights (std::queue<Perceptron*>& queue);
    void set_local_err (double err);

  private:
    std::vector<axon*> inputs_;
    std::vector<axon*> outputs_;
    double transfer_func_ (double x);
    double deriv_trans_func_ (double x);
    double action_potential_;
    double activation_val_;
    double learning_rate_;
    double local_err_;
    int index_;
    unsigned inputs_utd_;
    bool marked_;
};


#endif /* !PERCEPTRON_HH_ */
