/* Interface code for dealing with text properties.
   Copyright (C) 1993, 1994, 1995, 1997, 1999, 2000, 2001
   Free Software Foundation, Inc.

   This file is part of FANN.

   FANN is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   FANN is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with FANN; see the file COPYING.
   If not, see <http://www.gnu.org/licenses/>.  */


#include "perceptron.hh"

Perceptron::Perceptron (int index, double learning_rate)
  : inputs_ (),
    outputs_ (),
    action_potential_ (0.),
    activation_val_ (0.),
    learning_rate_ (learning_rate),
    local_err_ (0.),
    index_ (index),
    inputs_utd_ (0),
    marked_ (false)
{
}

Perceptron::~Perceptron ()
{

  std::vector<axon*>::iterator out_it = outputs_.begin ();
  for (; out_it != outputs_.end (); out_it++)
    delete *out_it;
  outputs_.clear ();
}

void Perceptron::connect_to (Perceptron* out)
{
  // double allocation for easy destruction
  axon* new_axon = new axon (this, out, 0.);
  out->inputs_.push_back (new_axon);
  outputs_.push_back (new_axon);
}

int Perceptron::get_index ()
{
  return index_;
}

void Perceptron::activate ()
{

  inputs_utd_++;
  // Neuron will only activate if all inputs have been updated
  if (inputs_utd_ >= inputs_.size ())
  {
    std::vector<axon*>::iterator in_it = inputs_.begin ();

    // weighted sum computation
    action_potential_ = 0.;
    for (; in_it != inputs_.end (); in_it++)
    {
      double msg = (*in_it)->message_get ();
      double weight = (*in_it)->weight_get ();
      action_potential_ += msg * weight;
    }

    // message transmission
    std::vector<axon*>::iterator out_it = outputs_.begin ();
    activation_val_ = transfer_func_ (action_potential_);
    for (; out_it != outputs_.end (); out_it++)
    {
      (*out_it)->message_set (activation_val_);
      (*out_it)->receiver_get ()->activate ();
    }
    // Back to outdated state
    inputs_utd_ = 0;
  }
}

void Perceptron::activate (double input_val)
{
  activation_val_ = input_val;
  std::vector<axon*>::iterator out_it = outputs_.begin ();
  for (; out_it != outputs_.end (); out_it++)
  {
    (*out_it)->message_set (input_val);
    (*out_it)->receiver_get ()->activate ();
  }
}

double Perceptron::transfer_func_ (double x)
{
  return 1. / (1. + exp (-x));
}

double Perceptron::deriv_trans_func_ (double x)
{
  double exp_x = exp (x);
  double denom = (1. + exp_x);
  denom *= denom;
  return exp_x / denom;
}

void Perceptron::dotify (std::ofstream& fs)
{
  std::vector<axon*>::iterator out_it = outputs_.begin ();
  for (; out_it != outputs_.end (); out_it++)
  {
    Perceptron* receiver = (*out_it)->receiver_get ();
    fs << "P" << index_ << " -> " << "P" << receiver->index_;
    fs << " [label=" << (*out_it)->weight_get () << "];";
    fs << std::endl;
  }
}

bool Perceptron::is_marked ()
{
  return marked_;
}

void Perceptron::mark ()
{
  marked_ = true;
}

void Perceptron::unmark ()
{
  marked_ = false;
}

double Perceptron::measure_av ()
{
  return activation_val_;
}

// Backprogation operations
void Perceptron::propagate_err (std::queue<Perceptron*>& queue)
{
  std::vector<axon*>::iterator out_it = outputs_.begin ();
  local_err_ = 0.;
  for (; out_it != outputs_.end (); out_it++)
  {
    Perceptron* next = (*out_it)->receiver_get ();
    local_err_ += (*out_it)->weight_get () * next->local_err_;
  }

  // width-first propagation
  std::vector<axon*>::iterator in_it = inputs_.begin ();
  for (; in_it != inputs_.end (); in_it++)
  {
    Perceptron* sender = (*in_it)->sender_get ();
    if (!sender->marked_)
     {
       queue.push (sender);
       sender->mark ();
     }
  }
}

void Perceptron::propagate_err (std::queue<Perceptron*>& queue, double out_err)
{
  local_err_ = out_err;
  // width-first propagation
  std::vector<axon*>::iterator in_it = inputs_.begin ();
  for (; in_it != inputs_.end (); in_it++)
  {
    Perceptron* sender = (*in_it)->sender_get ();
    if (!sender->marked_)
     {
       queue.push (sender);
       sender->mark ();
     }
  }
}

void Perceptron::adjust_weights (std::queue<Perceptron*>& queue)
{
  // Inbound Axons adjustment
  std::vector<axon*>::iterator in_it = inputs_.begin ();

  double deriv_val = deriv_trans_func_ (action_potential_);

  for (; in_it != inputs_.end (); in_it++)
  {
    double delta_weight = learning_rate_ * local_err_;
    delta_weight *= deriv_val;
    // Not sure weather delta should be multiplied by the previous
    // activation value
    delta_weight *= (*in_it)->receiver_get ()->activation_val_;
    (*in_it)->weight_adjust (delta_weight) ;
  }
  // Outbound propagation
  std::vector<axon*>::iterator out_it = outputs_.begin ();
  for (; out_it != outputs_.end (); out_it++)
  {
    Perceptron* receiver = (*out_it)->receiver_get ();
    if (!receiver->marked_)
    {
      queue.push (receiver);
      receiver->mark ();
    }
  }
}

void Perceptron::set_local_err (double err)
{
  local_err_ = err;
}
