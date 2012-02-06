#include "network.hh"

Network::Network ()
  : perceptrons_ (),
    inputs_ (),
    outputs_ (),
    learning_rate_ (1.)
{
}

Network::Network (std::vector<unsigned>& first_layer, neuralMap& neural_map)
  : perceptrons_ (),
    inputs_ (),
    outputs_ (),
    learning_rate_ (1.)
{
  this->initialize_network_ (first_layer, neural_map);
}

Network::Network (std::vector<unsigned>& first_layer,
                  neuralMap& neural_map,
                  double learning_rate)
  : perceptrons_ (),
    inputs_ (),
    outputs_ (),
    learning_rate_ (learning_rate)
{
  this->initialize_network_ (first_layer, neural_map);
}

Network::~Network ()
{
  std::vector<Perceptron*>::iterator it = perceptrons_.begin ();

  for (; it != perceptrons_.end (); it++)
    delete *it;
  perceptrons_.clear ();
}

void Network::initialize_network_ (std::vector<unsigned>& first_layer,
                                   neuralMap& neural_map)
{
  // Inputs creation
  std::vector<unsigned>::iterator it_fl = first_layer.begin ();
  size_t nb_perceptrons = neural_map.size ();

  for (size_t i = 0; i < nb_perceptrons; i++)
    perceptrons_.push_back (new Perceptron (i, learning_rate_));

  for (; it_fl != first_layer.end (); it_fl++)
  {
    inputs_.push_back (perceptrons_[*it_fl]);
    build_perceptron_ (neural_map, *it_fl, perceptrons_[*it_fl]);
  }
  unmark_network_ ();
}

void Network::unmark_network_ ()
{
  std::vector<Perceptron*>::iterator it_pn = perceptrons_.begin ();
  for (; it_pn != perceptrons_.end (); it_pn++)
    (*it_pn)->unmark ();
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
    outputs_.push_back (cur);
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
  std::vector<Perceptron*>::iterator in_it = inputs_.begin ();

  /************** SMART STUFF HAPPENS HERE *****************************************/
  /**/                                                                           /**/
  /*    Neuron activation : triggers a chain message transmition reaction          */
  /**/ for (in_it = inputs_.begin (); in_it != inputs_.end (); in_it++, in_ptr++)/**/
  /**/   (*in_it)->activate (*in_ptr);                                           /**/
  /**/                                                                           /**/
  /*********************************************************************************/

  // collecting outputs
  double* out_ptr = outputs;
  std::vector<Perceptron*>::iterator out_it = outputs_.begin ();
  for (; out_it != outputs_.end(); out_it++, out_ptr++)
    *out_ptr = (*out_it)->measure_av ();
}

void Network::train (double* desired_outputs, const double* inputs)
{
  double* outputs = new double(outputs_.size ());
  double* out_ptr = outputs;
  double* des_ptr = desired_outputs;
  interpolate (outputs, inputs);

  unmark_network_ ();

  std::vector<Perceptron*>::iterator out_it = outputs_.begin ();
  for (; out_it != outputs_.end(); out_it++, des_ptr++, out_ptr++)
  {
    double local_err = *des_ptr - *out_ptr;
    (*out_it)->set_local_err (local_err);
  }

  unmark_network_ ();

  std::queue<Perceptron*> width_queue;
  std::vector<Perceptron*>::iterator in_it = inputs_.begin ();

  for (in_it = inputs_.begin (); in_it != inputs_.end (); in_it++)
    width_queue.push (*in_it);

  // weight adjustments requires a width-first traversal of the map
  // A queue is used for that

  while (!width_queue.empty ())
  {
    Perceptron* front = width_queue.front ();
    width_queue.pop ();
    front->adjust_weights (width_queue);
  }
  delete[] outputs;
}
void Network::dotify (std::ofstream& fs)
{
  std::vector<Perceptron*>::iterator it = perceptrons_.begin ();
  for (; it != perceptrons_.end (); it++)
    (*it)->dotify (fs);
}

void Network::learning_rate_set (double lr)
{
  learning_rate_ = lr;
}

double Network::learning_rate_get ()
{
  return learning_rate_;
}

size_t Network::inputs_count ()
{
  return inputs_.size ();
}

size_t Network::outputs_count ()
{
  return outputs_.size ();
}
