/* Interface code for dealing with text properties.
   Copyright (C) 1993, 1994, 1995, 1997, 1999, 2000, 2001
   Free Software Foundation, Inc.

   This file is part of FANN.

   FANN is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   FANN is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with FANN; see the file COPYING.
   If not, see <http://www.gnu.org/licenses/>.  */


/*
** file: exceptions.hh
** author: benjamin
** created on 06/01/12 at 21:59
*/

#ifndef EXCEPTIONS_HH_
# define EXCEPTIONS_HH_
# include <cmath>
# include <iostream>
# include <cassert>
# include <exception>
# include <sstream>
# include <string>

class UnknownTokenException: std::exception
{
  public:
    virtual const char* what () const throw ()
    {
      return "Lexical error: Unknown token";
    }
};

class NoPerceptronException: std::exception
{
  public:
    NoPerceptronException (unsigned id)
      : id_ (id)
    {
    }
    virtual const char* what () const throw ()
    {
      std::stringstream st;
      st << "Syntax error: Perceptron number ";
      st << id_;
      st << " does not exist.";
      return st.str ().c_str ();
    }
  private:
    unsigned id_;
};

#endif /* !EXCEPTIONS_HH_ */
