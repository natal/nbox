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

class UnknownTokenException: std::exception
{
  public:
    virtual const char* what () const throw ()
    {
      return "Lexical error: Unknown token";
    }
};

#endif /* !EXCEPTIONS_HH_ */
