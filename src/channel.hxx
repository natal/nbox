#ifndef CHANNEL_HXX_
# define CHANNEL_HXX_

# include "channel.hh"

template <typename S, typename R, typename M>
Channel<S, R, M>::Channel (S sender, R receiver, M msg)
  : sender_ (sender), receiver_ (receiver), weight_ (1.), message_ (msg)
{
}

template <typename S, typename R, typename M>
Channel<S, R, M>::Channel (S sender, R receiver, M msg, double weight)
  : sender_ (sender), receiver_ (receiver), weight_ (weight), message_ (msg)
{
}

/*
** Deletion starts from the first sender and propagates through receivers.
** The idea is to sweep the whole network in a single shot
*/

template <typename S, typename R, typename M>
R Channel<S, R, M>::receiver_get ()
{
  return receiver_;
}

template <typename S, typename R, typename M>
S Channel<S, R, M>::sender_get ()
{
  return sender_;
}


template <typename S, typename R, typename M>
M Channel<S, R, M>::message_get ()
{
  return message_;
}

template <typename S, typename R, typename M>
void Channel<S, R, M>::message_set (M msg)
{
  message_ = msg;
}

template <typename S, typename R, typename M>
double Channel<S, R, M>::weight_get ()
{
  return weight_;
}

template <typename S, typename R, typename M>
void Channel<S, R, M>::weight_adjust (double delta)
{
  weight_ += delta;
}

#endif /* !CHANNEL_HH_ */
