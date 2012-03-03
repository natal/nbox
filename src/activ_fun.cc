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


#include "headers/activ_fun.hh"

double abs (double x)
{
    return x < 0 ? -x : x;
}

#define MAX_X 500

double sigmoid (double x)
{
  if (x > MAX_X)
      return 1;
  else if (x < -MAX_X)
      return 0;
  double res = 1. / (1. + exp (-x));
  return res;
}

double d_sigmoid (double x)
{
  if (abs (x) > MAX_X)
      return 0;
  double exp_x = exp (x);
  double denom = (1. + exp_x);
  denom *= denom;
  double res = exp_x / denom;
  return res;
}

double d_tanh (double x)
{
  double htan_x = tanh (x);
  return 1 - htan_x * htan_x;
}

double identity (double x)
{
  return x;
}
