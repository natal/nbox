/* Interface code for dealing with text properties.
   Copyright (C) 2011
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
** file: channel.hh
** author: benjamin
** created on 23/12/11 at 15:41
*/

#ifndef CHANNEL_HH_
# define CHANNEL_HH_
# include <iostream>
# include <fstream>

template <typename S, typename R, typename M>
class Channel
{
  public:
    Channel (S sender, R receiver, M message);
    Channel (S sender, R receiver, M message, double weight);
    R receiver_get ();
    S sender_get ();
    M message_get ();
    void message_set (M msg);
    double weight_get ();
    void weight_adjust (double delta);

  private:
    S sender_;
    R receiver_;
    double weight_;
    M message_;
};


# include "channel.hxx"


#endif /* !CHANNEL_HH_ */
