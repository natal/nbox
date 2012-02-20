/* Interface code for dealing with text properties.
   Copyright (C) 1993, 1994, 1995, 1997, 1999, 2000, 2001
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
** file: activ_fun.hh
** author: benjamin
** created on 18/02/12 at 21:04
*/

#ifndef BASIS_FUN_HH_
# define BASIS_FUN_HH_
# include <cmath>
# include <iostream>
# include <cassert>

typedef double (*function) (double);
typedef double (*derivative) (double);

double sigmoid (double x);
double d_sigmoid (double x);
double d_tanh (double x);
double identity (double x);

#endif /* !BASIS_FUN_HH_ */
