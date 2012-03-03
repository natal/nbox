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

  srand (time (NULL));

  for (size_t i = 0; i < nb_perceptrons; i++)
    perceptrons_.push_back (new Perceptron (i, learning_rate_));

  for (; it_fl != first_layer.end (); it_fl++)
    inputs_.push_back (perceptrons_[*it_fl]);

  for (size_t i = 0; i < nb_perceptrons; i++)
      build_perceptron_ (neural_map, i, perceptrons_[i]);

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
          cur->connect_to (next);
          if (!next->is_marked ())
          {
              build_perceptron_ (neural_map, *it, next);
              next->mark ();
          }
      }
  }
}

void Network::interpolate (double* outputs, const double* inputs)
{
  const double* in_ptr = inputs;
  std::vector<Perceptron*>::iterator in_it = inputs_.begin ();
  std::queue<Perceptron*> width_queue;

  unmark_network_ ();

  /************** SMART STUFF HAPPENS HERE ************************************/
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

void Network::train (const double* error)
{
    const double* err_ptr = error;

    std::queue<Perceptron*> e_queue; // queue used for error propagation
    std::queue<Perceptron*> w_queue; // queue used for weight adjustment

    unmark_network_ ();

    /****************  ERROR BACKPROPAGATION  **********************************/
    std::vector<Perceptron*>::iterator out_it = outputs_.begin ();
    for (; out_it != outputs_.end(); out_it++, err_ptr++)
        (*out_it)->propagate_err (e_queue, *err_ptr);

    // Backpropagate error using a width-first traversal of the
    // neural map
    while (!e_queue.empty ())
    {
        Perceptron* front = e_queue.front ();
        e_queue.pop ();
        front->propagate_err (e_queue);
    }

    unmark_network_ ();

    /****************  WEIGHT ADJUSTMENTS  *************************************/
    std::vector<Perceptron*>::iterator in_it = inputs_.begin ();

    for (in_it = inputs_.begin (); in_it != inputs_.end (); in_it++)
        w_queue.push (*in_it);

    // weight adjustments requires a width-first traversal of the map
    // A queue is used for that

    while (!w_queue.empty ())
    {
        Perceptron* front = w_queue.front ();
        w_queue.pop ();
        front->adjust_weights (w_queue);
    }
}

void Network::train (double* desired_outputs, const double* inputs)
{
  double* outputs = new double[outputs_.size ()];
  double* out_ptr = outputs;
  double* des_ptr = desired_outputs;

  interpolate (outputs, inputs);

  std::queue<Perceptron*> e_queue; // queue used for error propagation
  std::queue<Perceptron*> w_queue; // queue used for weight adjustment

  unmark_network_ ();

  /****************  ERROR BACKPROPAGATION  **********************************/
  /**/std::vector<Perceptron*>::iterator out_it = outputs_.begin ();
  /**/for (; out_it != outputs_.end(); out_it++, des_ptr++, out_ptr++)
  /**/{
  /**/   double local_err = *des_ptr - *out_ptr;
  /**/   (*out_it)->propagate_err (e_queue, local_err);
  /**/}
  /**/ // Backpropagate error using a width-first traversal of the
  /**/ // neural map
  /**/while (!e_queue.empty ())
  /**/{
  /**/   Perceptron* front = e_queue.front ();
  /**/   e_queue.pop ();
  /**/   front->propagate_err (e_queue);
  /**/}
  /**/
  /***************************************************************************/

  unmark_network_ ();

  /****************  WEIGHT ADJUSTMENTS  *************************************/
  /**/std::vector<Perceptron*>::iterator in_it = inputs_.begin ();
  /**/
  /**/for (in_it = inputs_.begin (); in_it != inputs_.end (); in_it++)
  /**/   w_queue.push (*in_it);
  /**/
  /**/ // weight adjustments requires a width-first traversal of the map
  /**/ // A queue is used for that
  /**/
  /**/while (!w_queue.empty ())
  /**/{
  /**/   Perceptron* front = w_queue.front ();
  /**/   w_queue.pop ();
  /**/   front->adjust_weights (w_queue);
  /**/}
  /***************************************************************************/

  delete[] outputs;
}

void Network::train_bp (double* desired_outputs, const double* inputs)
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

void Network::dotify (std::ofstream& fs)
{
  std::vector<Perceptron*>::iterator it = perceptrons_.begin ();
  for (; it != perceptrons_.end (); it++)
    (*it)->dotify (fs);
}

void Network::dotify_back (std::ofstream& fs)
{
  std::vector<Perceptron*>::iterator it = perceptrons_.begin ();
  for (; it != perceptrons_.end (); it++)
    (*it)->dotify_back (fs);
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

void Network::adjust_rate (double delta)
{
    learning_rate_ -= delta;
    std::vector<Perceptron*>::iterator it = perceptrons_.begin ();
    for (; it != perceptrons_.end (); it++)
        (*it)->adjust_rate (delta);
}
