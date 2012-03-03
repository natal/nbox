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


#include "headers/perceptron.hh"

Perceptron::Perceptron (int index, double learning_rate)
  : inputs_ (),
    outputs_ (),
    transfer_func_ (sigmoid),
    d_transfer_func_ (d_sigmoid),
    action_potential_ (1.),
    activation_val_ (1.),
    learning_rate_ (learning_rate),
    local_err_ (0.),
    index_ (index),
    marked_ (false)
{
}

Perceptron::Perceptron (int index,
                        double learning_rate,
                        function fun,
                        derivative d_fun)
  : inputs_ (),
    outputs_ (),
    transfer_func_ (fun),
    d_transfer_func_ (d_fun),
    action_potential_ (1.),
    activation_val_ (1.),
    learning_rate_ (learning_rate),
    local_err_ (0.),
    index_ (index),
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

  double rand_weight = (double)(rand () % RAND_MOD - RAND_MOD_2) / RAND_MOD;
  rand_weight *= RAND_RANGE;
  axon* new_axon = new axon (this, out, 1.0, rand_weight);
  out->inputs_.push_back (new_axon);
  outputs_.push_back (new_axon);
}

int Perceptron::get_index ()
{
  return index_;
}

void Perceptron::activate (std::queue<Perceptron*>& w_queue)
{
  std::vector<axon*>::iterator in_it = inputs_.begin ();
  // std::cout << "Activation of neuron " << index_ << std::endl;

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
    Perceptron* receiver = (*out_it)->receiver_get ();
    if (!receiver->marked_)
    {
      receiver->mark ();
      w_queue.push (receiver);
    }
  }
}

void Perceptron::activate (std::queue<Perceptron*>& w_queue, double input_val)
{
  activation_val_ = input_val;
  std::vector<axon*>::iterator out_it = outputs_.begin ();
  // std::cout << "Activation of neuron " << index_ << std::endl;
  for (; out_it != outputs_.end (); out_it++)
  {
    (*out_it)->message_set (input_val);
    Perceptron* receiver = (*out_it)->receiver_get ();
    if (!receiver->marked_)
    {
      receiver->mark ();
      w_queue.push (receiver);
    }
  }
}

void Perceptron::dotify (std::ofstream& fs)
{
  std::vector<axon*>::iterator out_it = outputs_.begin ();
  for (; out_it != outputs_.end (); out_it++)
  {
    Perceptron* receiver = (*out_it)->receiver_get ();
    fs << "P" << index_
       << " -> " << "P" << receiver->index_ << " ";
    fs << " [label=" << (*out_it)->weight_get () << "];";
    fs << std::endl;
  }
}

void Perceptron::dotify_back (std::ofstream& fs)
{
  std::vector<axon*>::iterator in_it = inputs_.begin ();
  for (; in_it != inputs_.end (); in_it++)
  {
    Perceptron* sender = (*in_it)->sender_get();
    fs << "P" << index_ << " -> " << "P" << sender->index_;
    fs << " [label=" << (*in_it)->weight_get () << "];";
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
  // std::cout << "Local error in neuron " << index_ << std::endl;
  for (; out_it != outputs_.end (); out_it++)
  {
    Perceptron* next = (*out_it)->receiver_get ();
    local_err_ += (*out_it)->weight_get () * next->local_err_;
  }
  double res_d = d_transfer_func_ (action_potential_);
  local_err_ *= res_d;

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
  local_err_ = out_err * d_transfer_func_ (action_potential_);
  // width-first propagation
  std::vector<axon*>::iterator in_it = inputs_.begin ();
  // std::cout << "Local error in neuron " << index_ << std::endl;
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
    // Warning: connexions exclusively dependant on bias neurons
    // will not be adjusted
    // example : bias  _ neuron1
    //                |_ neuron2
    //                |_ neuron3

  // Inbound Axons adjustment

  std::vector<axon*>::iterator in_it = inputs_.begin ();

  double delta_weight = learning_rate_ * local_err_ * activation_val_;

  // std::cout << "Weight adjusting from neuron " << index_ << std::endl;

  for (; in_it != inputs_.end (); in_it++)
    (*in_it)->weight_adjust (delta_weight) ;

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

void Perceptron::backpropagate (std::queue<Perceptron*>& queue, double out_err)
{
    local_err_ = out_err * d_transfer_func_ (action_potential_);
    // weight adjustment
    std::vector<axon*>::iterator in_it = inputs_.begin ();

    double scaled_err = learning_rate_ * local_err_;

    for (; in_it != inputs_.end (); in_it++)
    {
        Perceptron* sender = (*in_it)->sender_get ();
        double delta_weight = scaled_err * sender->activation_val_;
        (*in_it)->weight_adjust (delta_weight) ;
    }

    // width-first propagation
    in_it = inputs_.begin ();
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

void Perceptron::backpropagate (std::queue<Perceptron*>& queue)
{
    std::vector<axon*>::iterator out_it = outputs_.begin ();
    local_err_ = 0.;
    // std::cout << "Local error in neuron " << index_ << std::endl;
    for (; out_it != outputs_.end (); out_it++)
    {
        Perceptron* next = (*out_it)->receiver_get ();
        local_err_ += (*out_it)->weight_get () * next->local_err_;
    }
    double res_d = d_transfer_func_ (action_potential_);
    local_err_ *= res_d;

    // weight adjustment
    std::vector<axon*>::iterator in_it = inputs_.begin ();

    double scaled_err = learning_rate_ * local_err_;

    for (; in_it != inputs_.end (); in_it++)
    {
        Perceptron* sender = (*in_it)->sender_get ();
        double delta_weight = scaled_err * sender->activation_val_;
        (*in_it)->weight_adjust (delta_weight) ;
    }

    // width-first propagation
    in_it = inputs_.begin ();
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

void Perceptron::adjust_rate (double delta)
{
    learning_rate_ -= delta;
}

void Perceptron::local_err_set (double err)
{
  local_err_ = err;
}

void Perceptron::make_linear ()
{
  transfer_func_ = identity;
  d_transfer_func_ = identity;
}
