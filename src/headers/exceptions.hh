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
# include <cstring>

# define MSG_MAX_LENGTH 1000

class UnknownTokenException: public std::exception
{
  public:
    virtual const char* what () const throw ()
    {
      return "Lexical error: Unknown token";
    }
};

class NoLinkException: public std::exception
{
  private:
      int p1_;
      int p2_;
      double val_;
  public:
    NoLinkException (int p1, int p2, double val)
        : p1_ (p1), p2_ (p2), val_ (val)
    {
    }

    virtual const char* what () const throw ()
    {
      std::stringstream sstream;
      sstream << "Weight loading error : Could not set weight " << val_
              << " on synapse from P" << p1_
               << " to P"<< p2_ << std::endl;
      return sstream.str ().c_str ();
    }
};

class SyntaxErrorException: public std::exception
{
  public:
    SyntaxErrorException (std::string msg)
    {

        std::string m = "Syntax error: ";
        msg = m + msg;
        size_t len = msg.length ();
        len = len > MSG_MAX_LENGTH ? MSG_MAX_LENGTH : len;
        strncpy (msg_, msg.c_str (), len);
        msg_[len] = '\0';
    }

    virtual const char* what () const throw ()
    {
      return msg_;
    }
  private:
      char msg_[MSG_MAX_LENGTH];
};

class NoPerceptronException: public std::exception
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
