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
