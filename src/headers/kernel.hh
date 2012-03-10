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
** file: kernel.hh
** author: benjamin
** created on 18/02/12 at 21:04
*/

#ifndef KERNEL_HH_
# define KERNEL_HH_
# include <cmath>
# include <iostream>
# include <cassert>
# include <map>
# include <string>

namespace nbx
{
    class Kernel
    {
        public:
            typedef double (*function) (double);
            typedef double (*derivative) (double);
            Kernel (const std::string& func_name);
            Kernel (function fun, derivative der);
            double eval (double x);
            double eval_d (double x);

        private:
            typedef std::pair<function, derivative> fun_der;
            typedef std::pair<const std::string, fun_der> fun_label;
            static double sigmoid_ (double x);
            static double d_sigmoid_ (double x);
            static double d_tanh_ (double x);
            void init_map_fun_ ();
            std::map<const std::string, fun_der> map_fun_;
            function fun_;
            derivative der_;
    };
}

#endif /* !KERNEL_HH_*/
