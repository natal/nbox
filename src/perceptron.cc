#include "perceptron.hh"

Perceptron::Perceptron ()
  : inputs_ (), outputs_ ()
{
}

Perceptron::~Perceptron ()
{
}

void Perceptron::connect_to (Perceptron& out)
{
  axon new_axon (this, &out, 0.);
  out.inputs_.push_back (new_axon);
  outputs_.push_back (new_axon);
}

void Perceptron::activate ()
{
  std::vector<axon>::iterator in_it = inputs_.begin ();

  double sum = 0.;
  for (; in_it != inputs_.end (); in_it++)
  {
    double msg = (*in_it).message_get ();
    double weight = (*in_it).weight_get ();
    sum += msg * weight;
  }

  // message transmission
  std::vector<axon>::iterator out_it = outputs_.begin ();
  double transfer_msg = transfer_func_ (sum);
  for (; out_it != outputs_.end (); out_it++)
    (*out_it).message_set (transfer_msg);
}

double Perceptron::transfer_func_ (double x)
{
  return 1. / (1. + exp (-x));
}
