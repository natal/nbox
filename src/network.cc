#include "network.hh"

Network::Network ()
  : inputs_ ()
{
}

Network::Network (neuralMap& first_layer, neuralMap& neural_map)
  : inputs_ ()
{
  // Inputs creation
  neuralMap::iterator it_nm = first_layer.begin ();
  unsigned pos = 0;
  for (; it_nm != first_layer.end (); it_nm++)
  {
    Perceptron* input_perceptron = new Perceptron ();
    inputs_.push_back (input (0., input_perceptron, 0.));
    build_perceptron_ (neural_map, pos, input_perceptron);
    pos++;
  }
}

// trigger sweeping-destruction of the neurons (from inputs to outputs)
Network::~Network ()
{
  /*
  std::vector<cell>::iterator it = inputs_->begin ();

  for (; it != inputs_.end (); it++)
  {
    delete *it->get_receiver;
    delete *it;
  }
  inputs_.clear ();
  */
}

// recursive perceptron building (deep-first traversal of the neural map)
void Network::build_perceptron_ (neuralMap& neural_map,
                                 unsigned cur_idx,
                                 Perceptron* cur)
{
  std::vector<unsigned>* cur_cell = neural_map[cur_idx];
  if (cur_cell)
  {
    std::vector<unsigned>::iterator it = cur_cell->begin ();
    for (; it != cur_cell->end (); it++)
    {
      Perceptron* new_perceptron = new Perceptron ();
      cur->connect_to (*new_perceptron);
      // Marking the cell
      neural_map[cur_idx] = 0;
      build_perceptron_ (neural_map, *it, new_perceptron);
    }
  }
  outputs_.push_back (output (cur, 0., 0.));
}

void Network::interpolate (double* outputs, const double* inputs)
{
  const double* in_ptr = inputs;
  std::vector<input>::iterator in_it = inputs_.begin ();

  // setting inputs
  for (; in_it != inputs_.end (); in_it++, in_ptr++)
    (*in_it).message_set (*in_ptr);

  /************** SMART STUFF HAPPENS HERE *****************************************/
  /**/                                                                           /**/
  /*    Neuron activation : triggers a chain message transmition reaction          */
  /**/ for (in_it = inputs_.begin (); in_it != inputs_.end (); in_it++, in_ptr++)/**/
  /**/   (*in_it).receiver_get ()->activate ();                                 /**/
  /**/                                                                           /**/
  /*********************************************************************************/

  // collecting outputs
  double* out_ptr = outputs;
  std::vector<output>::iterator out_it = outputs_.begin ();
  for (; out_it != outputs_.end(); out_it++, out_ptr++)
    (*out_ptr) = (*out_it).receiver_get ();
}
