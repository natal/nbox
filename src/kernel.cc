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


#include "headers/kernel.hh"

#define ABS(x) (x < 0 ? -x : x)
#define MAX_X 500

double nbx::Kernel::sigmoid_ (double x)
{
  if (x > MAX_X)
      return 1;
  else if (x < -MAX_X)
      return 0;
  double res = 1. / (1. + exp (-x));
  return res;
}

double nbx::Kernel::d_sigmoid_ (double x)
{
  if (ABS (x) > MAX_X)
      return 0;
  double exp_x = exp (x);
  double denom = (1. + exp_x);
  denom *= denom;
  double res = exp_x / denom;
  return res;
}

double nbx::Kernel::d_tanh_ (double x)
{
  double htan_x = tanh (x);
  return (1 - htan_x * htan_x);
}

nbx::Kernel::Kernel (const std::string& func_name)
{
    init_map_fun_ ();
    fun_ = map_fun_[func_name].first;
    der_ = map_fun_[func_name].second;
}

nbx::Kernel::Kernel (function fun, derivative der)
    : fun_ (fun),
      der_ (der)
{
}

double nbx::Kernel::eval (double x)
{
    return fun_ (x);
}

double nbx::Kernel::eval_d (double x)
{
    return der_ (x);
}

void nbx::Kernel::init_map_fun_ ()
{
    fun_der sig_set (sigmoid_, d_sigmoid_);
    fun_der tanh_set (tanh, d_tanh_);
    fun_label sig_label ("sigmoid", sig_set);
    fun_label th_label ("tanh", tanh_set);
    fun_label ht_label ("htan", tanh_set);

    map_fun_.insert (sig_label);
    map_fun_.insert (th_label);
    map_fun_.insert (ht_label);
}
