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
