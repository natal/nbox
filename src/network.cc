#include "network.hh"

Network::Network ()
  : inputs_ ()
{
}

Network::Network (std::vector<unsigned>& first_layer, neuralMap& neural_map)
  : inputs_ ()
{
  // Inputs creation
  std::vector<unsigned>::iterator it_fl = first_layer.begin ();
  size_t nb_perceptrons = neural_map.size ();

  for (size_t i = 0; i < nb_perceptrons; i++)
    perceptrons_.push_back (new Perceptron (i));

  for (; it_fl != first_layer.end (); it_fl++)
  {
    inputs_.push_back (input (0., perceptrons_[*it_fl], 0.));
    build_perceptron_ (neural_map, *it_fl, perceptrons_[*it_fl]);
  }
}

Network::~Network ()
{
  std::vector<Perceptron*>::iterator it = perceptrons_.begin ();

  for (; it != perceptrons_.end (); it++)
    delete *it;
  perceptrons_.clear ();
}

// recursive perceptron building (deep-first traversal of the neural map)
void Network::build_perceptron_ (neuralMap& neural_map,
                                 unsigned cur_idx,
                                 Perceptron* cur)
{
  std::vector<unsigned>* cur_cell = neural_map[cur_idx];
  std::vector<unsigned>::iterator it = cur_cell->begin ();

  if (!cur_cell->size ())
  {
    outputs_.push_back (output (cur, 0., 0.));
    return;
  }

  for (; it != cur_cell->end (); it++)
  {
    if (*it >= perceptrons_.size ())
      throw NoPerceptronException (*it);

    Perceptron* next = perceptrons_[*it];
    cur->connect_to (next);
    if (!next->is_marked ())
    {
      build_perceptron_ (neural_map, *it, next);
      next->mark ();
    }
  }
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

void Network::dotify (std::ofstream& fs)
{
  std::vector<Perceptron*>::iterator it = perceptrons_.begin ();
  for (; it != perceptrons_.end (); it++)
    (*it)->dotify (fs);
}
