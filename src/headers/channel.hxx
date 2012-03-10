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


#ifndef CHANNEL_HXX_
# define CHANNEL_HXX_

# include "channel.hh"

# define MAX_WEIGHT 100.
# define MIN_WEIGHT 0.0001

template <typename S, typename R, typename M>
nbx::Channel<S, R, M>::Channel (S sender, R receiver, M msg)
  : sender_ (sender), receiver_ (receiver), weight_ (1.), message_ (msg)
{
}

template <typename S, typename R, typename M>
nbx::Channel<S, R, M>::Channel (S sender, R receiver, M msg, double weight)
  : sender_ (sender), receiver_ (receiver), weight_ (weight), message_ (msg)
{
}

/*
** Deletion starts from the first sender and propagates through receivers.
** The idea is to sweep the whole network in a single shot
*/

template <typename S, typename R, typename M>
R nbx::Channel<S, R, M>::receiver_get ()
{
  return receiver_;
}

template <typename S, typename R, typename M>
S nbx::Channel<S, R, M>::sender_get ()
{
  return sender_;
}


template <typename S, typename R, typename M>
M nbx::Channel<S, R, M>::message_get ()
{
  return message_;
}

template <typename S, typename R, typename M>
void nbx::Channel<S, R, M>::message_set (M msg)
{
  message_ = msg;
}

template <typename S, typename R, typename M>
double nbx::Channel<S, R, M>::weight_get ()
{
  return weight_;
}

template <typename S, typename R, typename M>
void nbx::Channel<S, R, M>::weight_adjust (double delta)
{
  weight_ += delta;
  /*
  double sign = 1;
  if (weight_ < 0)
      sign = -1;
//  double sign = weight_ / abs (weight_);
//  if (abs (weight_ - MAX_WEIGHT) > 0)
//     weight_ = sign * MAX_WEIGHT;
  if (abs (weight_) < MIN_WEIGHT)
     weight_ = sign * MIN_WEIGHT;
     */
}

template <typename S, typename R, typename M>
void nbx::Channel<S, R, M>::weight_set (double val)
{
  weight_ = val;
}

#endif /* !CHANNEL_HH_ */
