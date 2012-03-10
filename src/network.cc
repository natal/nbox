/* Interface code for dealing with text properties.
   Copyright (C) 2011-2012
   Free Software Foundation, Inc.

   This file is part of nbox.

   nbox is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   nbox is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with nbox; see the file COPYING.
   If not, see <http://www.gnu.org/licenses/>.  */


#include "headers/network.hh"

nbx::Network::Network ()
  : perceptrons_ (),
    synapses_ (),
    inputs_ (),
    outputs_ (),
    learning_rate_ (1.),
    kernel_ (0)
{
    kernel_ = new Kernel ("sigmoid");
}

nbx::Network::Network (std::vector<unsigned>& first_layer, neuralMap& neural_map)
  : perceptrons_ (),
    synapses_ (),
    inputs_ (),
    outputs_ (),
    learning_rate_ (1.),
    kernel_ (0)
{
    kernel_ = new Kernel ("sigmoid");
  this->initialize_network_ (first_layer, neural_map);
}

nbx::Network::Network (std::vector<unsigned>& first_layer,
                       neuralMap& neural_map,
                       double learning_rate,
                       const std::string& kernel_name)
  : perceptrons_ (),
    inputs_ (),
    outputs_ (),
    learning_rate_ (learning_rate)
{
  kernel_ = new Kernel (kernel_name);
  this->initialize_network_ (first_layer, neural_map);
}

nbx::Network::~Network ()
{
  std::vector<Perceptron*>::iterator it = perceptrons_.begin ();
  for (; it != perceptrons_.end (); it++)
    delete *it;
  delete kernel_;
  perceptrons_.clear ();
}

void nbx::Network::initialize_network_ (std::vector<unsigned>& first_layer,
                                        neuralMap& neural_map)
{
  // Inputs creation
  std::vector<unsigned>::iterator it_fl = first_layer.begin ();
  size_t nb_perceptrons = neural_map.size ();

  srand (time (NULL));

  for (size_t i = 0; i < nb_perceptrons; i++)
    perceptrons_.push_back (new Perceptron (i, learning_rate_, kernel_));

  for (; it_fl != first_layer.end (); it_fl++)
    inputs_.push_back (perceptrons_[*it_fl]);

  for (size_t i = 0; i < nb_perceptrons; i++)
      build_perceptron_ (neural_map, i, perceptrons_[i]);

  unmark_network_ ();
}

void nbx::Network::unmark_network_ ()
{
  std::vector<Perceptron*>::iterator it_pn = perceptrons_.begin ();
  for (; it_pn != perceptrons_.end (); it_pn++)
    (*it_pn)->unmark ();
}

// recursive perceptron building (deep-first traversal of the neural map)
void nbx::Network::build_perceptron_ (neuralMap& neural_map,
                                 unsigned cur_idx,
                                 Perceptron* cur)
{
  std::vector<unsigned>* cur_cell = neural_map[cur_idx];
  std::vector<unsigned>::iterator it = cur_cell->begin ();

  if (!cur->is_marked ())
  {
      cur->mark ();
      if (!cur_cell->size ())
      {
         // cur->make_linear ();
          outputs_.push_back (cur);
          return;
      }

      for (; it != cur_cell->end (); it++)
      {
          if (*it >= perceptrons_.size ())
              throw NoPerceptronException (*it);

          Perceptron* next = perceptrons_[*it];

          Perceptron::axon* a = cur->connect_to (next);
          synapse s (cur->index_get (), *it);
          std::pair<synapse, Perceptron::axon*> p (s, a);
          synapses_.insert (p);

          if (!next->is_marked ())
          {
              build_perceptron_ (neural_map, *it, next);
              next->mark ();
          }
      }
  }
}

void nbx::Network::interpolate (double* outputs, const double* inputs)
{
  const double* in_ptr = inputs;
  std::vector<Perceptron*>::iterator in_it = inputs_.begin ();
  std::queue<Perceptron*> width_queue;

  unmark_network_ ();

  /************** EVALUATION HAPPENS HERE ***********************************/
  /**/                                                                      /**/
  /*    Neuron activation : width-first activation of the network             */
  /**/ for (; in_it != inputs_.end (); in_it++, in_ptr++)                   /**/
  /**/   (*in_it)->activate (width_queue, *in_ptr);                         /**/
  /**/                                                                      /**/
  /**/ while (!width_queue.empty ())                                        /**/
  /**/ {                                                                    /**/
  /**/   Perceptron* front = width_queue.front ();                          /**/
  /**/   width_queue.pop ();                                                /**/
  /**/   front->activate (width_queue);                                     /**/
  /**/ }                                                                    /**/
  /****************************************************************************/

  // collecting outputs
  double* out_ptr = outputs;
  std::vector<Perceptron*>::iterator out_it = outputs_.begin ();
  for (; out_it != outputs_.end(); out_it++, out_ptr++)
    *out_ptr = (*out_it)->measure_av ();
}

void nbx::Network::train_bp (double* desired_outputs, const double* inputs)
{
    double* outputs = new double[outputs_.size ()];
    double* out_ptr = outputs;
    double* des_ptr = desired_outputs;

    interpolate (outputs, inputs);

    std::queue<Perceptron*> queue; // queue used for back propagation
    unmark_network_ ();

 /****************  ERROR BACKPROPAGATION  **********************************/
 /**/std::vector<Perceptron*>::iterator out_it = outputs_.begin ();
 /**/for (; out_it != outputs_.end(); out_it++, des_ptr++, out_ptr++)
 /**/{
 /**/   double local_err = *des_ptr - *out_ptr;
 /**/   (*out_it)->backpropagate (queue, local_err);
 /**/}
 /**/ // Backpropagate error using a width-first traversal of the
 /**/ // neural map
 /**/while (!queue.empty ())
 /**/{
 /**/   Perceptron* front = queue.front ();
 /**/   queue.pop ();
 /**/   front->backpropagate (queue);
 /**/}
 /**/
 /***************************************************************************/
}

void nbx::Network::dotify (std::ofstream& fs)
{
  std::vector<Perceptron*>::iterator it = perceptrons_.begin ();
  for (; it != perceptrons_.end (); it++)
    (*it)->dotify (fs);
}

void nbx::Network::dotify_back (std::ofstream& fs)
{
  std::vector<Perceptron*>::iterator it = perceptrons_.begin ();
  for (; it != perceptrons_.end (); it++)
    (*it)->dotify_back (fs);
}

void nbx::Network::dotify (const char* file)
{
    std::ofstream fs;
    fs.precision(7);
    fs << std::fixed;
    fs.open (file);
    fs << "digraph \"neural map\"" << std::endl;
    fs << "{" << std::endl;
    std::vector<Perceptron*>::iterator it = perceptrons_.begin ();
    for (; it != perceptrons_.end (); it++)
        (*it)->dotify (fs);
    fs << "}" << std::endl;
    fs.close ();
}

void nbx::Network::dotify_back (const char* file)
{
    std::ofstream fs;
    fs.precision(7);
    fs << std::fixed;
    fs.open (file);
    fs << "digraph \"neural map\"" << std::endl;
    fs << "{" << std::endl;
    std::vector<Perceptron*>::iterator it = perceptrons_.begin ();
    for (; it != perceptrons_.end (); it++)
        (*it)->dotify_back (fs);
    fs << "}" << std::endl;
    fs.close ();
}

void nbx::Network::save_weights (const char* file)
{
    std::ofstream fs;
    fs.open (file);
    std::map<synapse, Perceptron::axon*>::iterator it = synapses_.begin ();
    for (; it != synapses_.end (); it++)
    {
        Perceptron* sender = (*it).second->sender_get ();
        Perceptron* receiver = (*it).second->receiver_get ();
        fs << "( " << sender->index_get () << " , "
           << receiver->index_get () << " ) "
           << (*it).second->weight_get () << std::endl;
    }
    fs.close ();
}

void nbx::Network::learning_rate_set (double lr)
{
  learning_rate_ = lr;
}

double nbx::Network::learning_rate_get ()
{
  return learning_rate_;
}

size_t nbx::Network::inputs_count ()
{
  return inputs_.size ();
}

size_t nbx::Network::outputs_count ()
{
  return outputs_.size ();
}

void nbx::Network::adjust_rate (double delta)
{
    learning_rate_ -= delta;
    std::vector<Perceptron*>::iterator it = perceptrons_.begin ();
    for (; it != perceptrons_.end (); it++)
        (*it)->adjust_rate (delta);
}

void nbx::Network::weight_set (unsigned p1, unsigned p2, double val)
{
    synapse s (p1, p2);
    Perceptron::axon* a = synapses_[s];
    if (!a)
        throw NoLinkException (p1, p2, val);
    a->weight_set (val);
}
