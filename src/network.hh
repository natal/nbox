/* Interface code for dealing with text properties.
   Copyright (C) 2011
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
typedef std::vector<std::vector<unsigned>*> neuralMap;

class Network
{
  public:
    Network ();
    Network (std::vector<unsigned>& first_layer, neuralMap& neural_map);
    Network (std::vector<unsigned>& first_layer,
             neuralMap& neural_map,
             double learning_rate);
    ~Network ();
    // the size of both arrays must match the number of outputs and inuts defined
    // by constrcution
    void interpolate (double* outputs, const double* inputs);
 /*   void interpolate (std::vector<double>& outputs,
                      const std::vector<double>& inputs); */
    void train (double* desired_outputs, const double* inputs);
    void dotify (std::ofstream& fs);
    void learning_rate_set (double lr);
    double learning_rate_get ();
    size_t inputs_count ();
    size_t outputs_count ();

  private:
    std::vector<Perceptron*> perceptrons_;
    std::vector<Perceptron*> inputs_;
    std::vector<Perceptron*> outputs_;
    void initialize_network_ (std::vector<unsigned>& first_layer, neuralMap& neural_map);
    void build_perceptron_ (neuralMap& neural_map,
                            unsigned next_idx,
                            Perceptron* cur);
    double learning_rate_;
    void unmark_network_ ();
};

#endif /* !NETWORK_HH_ */
