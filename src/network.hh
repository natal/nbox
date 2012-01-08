/*
** file: network.hh
** author: benjamin
** created on 23/12/11 at 15:15
*/

#ifndef NETWORK_HH_
# define NETWORK_HH_
# include "perceptron.hh"
# include "exceptions.hh"
# include <fstream>

typedef std::pair<size_t, size_t> cell;
typedef Channel<double, Perceptron*, double> input;
typedef Channel<Perceptron*, double, double> output;
typedef std::vector<std::vector<unsigned>*> neuralMap;

class Network
{
  public:
    Network ();
    Network (std::vector<unsigned>& first_layer, neuralMap& neural_map);
    ~Network ();
    // the size of both arrays must match the number of outputs and inuts defined
    // by constrcution
    void interpolate (double* outputs, const double* inputs);
 /*   void interpolate (std::vector<double>& outputs,
                      const std::vector<double>& inputs); */
    void dotify (std::ofstream& fs);

  private:
    std::vector<Perceptron*> perceptrons_;
    std::vector<input> inputs_;
    std::vector<output> outputs_;
    void build_perceptron_ (neuralMap& neural_map,
                            unsigned next_idx,
                            Perceptron* cur);
};

#endif /* !NETWORK_HH_ */
