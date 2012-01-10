#include "perceptron.hh"

Perceptron::Perceptron (int idx)
  : inputs_ (), outputs_ (), index_ (idx), marked_ (false)
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
  std::vector<axon*>::iterator in_it = inputs_.begin ();

  double sum = 0.;
  for (; in_it != inputs_.end (); in_it++)
  {
    double msg = (*in_it)->message_get ();
    double weight = (*in_it)->weight_get ();
    sum += msg * weight;
  }

  // message transmission
  std::vector<axon*>::iterator out_it = outputs_.begin ();
  double transfer_msg = transfer_func_ (sum);
  for (; out_it != outputs_.end (); out_it++)
    (*out_it)->message_set (transfer_msg);
}

double Perceptron::transfer_func_ (double x)
{
  return 1. / (1. + exp (-x));
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
