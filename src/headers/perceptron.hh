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


/*
 ** file: perceptron.hh
 ** author: benjamin
 ** created on 23/12/11 at 18:43
 */

#ifndef PERCEPTRON_HH_
# define PERCEPTRON_HH_
# include <cmath>
# include <iostream>
# include <cassert>
# include <vector>
# include <queue>
# include <fstream>
# include <algorithm>
# include <ctime>

# include "channel.hh"
# include "kernel.hh"

# define RAND_MOD 64
# define RAND_MOD_2 32
# define RAND_RANGE 0.1
# define RAND_MIN 0.0001

namespace nbx
{
    class Perceptron
    {
        public:
            typedef Channel<Perceptron*, Perceptron*, double> axon;

            /**
             ** @brief The perceptron's constructor.
             ** @param index the index of the perceptron in the neural list.
             ** @param learning_rate the learning rate of the perceptron.
             */

            Perceptron (int index,
                        double learning_rate,
                        Kernel* kern);

            ~Perceptron ();

            /**
             ** @brief connect the perceptron to another.
             ** @param out the perceptron to connect to
             */
            axon* connect_to (Perceptron* out);

            /**
             ** @brief activate the neuron and transmit the neural message
             **         to the adjacent neurons:
             **           - retrieve the neural message from the inputs,
             **           - multiply by the weights and compute the activation function.
             **           - Pass the result down to the output neurons.
             ** @param input_val use this to replace to impose your own input value
             **                  otherwise input value is computed from input nerons
             */
            void activate (std::queue<Perceptron*>& w_queue);
            void activate (std::queue<Perceptron*>& w_queue, double input_val);

            void dotify (std::ofstream& fs);
            void dotify_back (std::ofstream& fs);
            int index_get ();
            void adjust_rate (double delta);

            // Marking mecanism

            bool is_marked ();
            void mark ();
            void unmark ();

            /**
             ** @brief measure the perceptron's last activation value
             */
            double measure_av ();


            // Backpropagation operations - requires all neurons
            // to be unmarked

            /**
             ** @brief Computes the local error according to the error of
             **        its receivers' error. Uses width-first traversal to
             **        propagate error
             ** @param queue used to queue up the perceptrons in the order
             **              required for backpropagation
             */
            void backpropagate (std::queue<Perceptron*>& queue);
            void backpropagate (std::queue<Perceptron*>& queue,
                                double out_err);
            void local_err_set (double err);

        private:
            Kernel* kernel_;
            std::vector<axon*> inputs_;
            std::vector<axon*> outputs_;
            double action_potential_;
            double activation_val_;
            double learning_rate_;
            double local_err_;
            int index_;
            bool marked_;
    };
}

#endif /* !PERCEPTRON_HH_ */
