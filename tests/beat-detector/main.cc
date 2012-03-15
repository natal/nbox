#include <iostream>
#include "../../src/headers/network.hh"
#include "../../src/headers/exceptions.hh"
#include "../../src/headers/parser.hh"
#include "inc/fmod.hpp"
#include "inc/fmod_errors.h"
#include <fstream>
#include <string>
#include <stack>
#include <ctime>
#include <cmath>
#include <cstring>
#include <exception>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define THRESHOLD 0.15
#define SPECTRUM_WIDTH 512
#define SLEEP_TIME 25
#define HISTORY_SIZE 10
#define MAX_ITERATIONS 5000
#define SAMPLE_SIZE 8192
#define MAX_MSG_LENGTH 2000
#define MIN(a,b) (a < b ? a : b)

using namespace nbx;

class OptionErrorException: public std::exception
{
    public:
        OptionErrorException (const char* error_msg)
        {
            std::string s(error_msg);
            strncpy (msg_, s.c_str (), MIN (s.length (), MAX_MSG_LENGTH));
        }
        virtual const char* what () const throw ()
        {
            return msg_;
        }
    private:
        char msg_[MAX_MSG_LENGTH];
};

static void check_opt (int argc, char** argv)
{
    std::stringstream sstream;
    if (argc < 3)
    {
        sstream << "Usage: ./beat [music file] [map file] [weight file]"
                << std::endl;

        throw OptionErrorException (sstream.str ().c_str ());
    }
}


static double sqme (double* a,
                    double* b,
                    size_t l)
{
    double length = 0;
    for (size_t i = 0; i < l; i++)
    {
        double diff = b[i] - a[i];
        length += diff * diff;
    }
    return length;
}

void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
      std::stringstream sstream;
      sstream << "FMOD error (" << result << ") " <<
        FMOD_ErrorString (result) << std::endl;
      throw OptionErrorException (sstream.str ().c_str ());
    }
}

static void check_io_sizes (size_t file_in,
                            size_t file_out,
                            size_t nn_in,
                            size_t nn_out)
{

    if (!file_in)
        throw OptionErrorException ("Syntax Error: Data File, \
                                     Not a valid input size");
    if (!file_out)
        throw OptionErrorException ("Syntax Error: Data File, \
                                     Not a valid output size");
    if (!nn_in)
        throw OptionErrorException ("Syntax Error: Neural Map File, \
                                     Not a valid input size");
    if (!nn_out)
        throw OptionErrorException ("Syntax Error: Neural Map File, \
                                     Not a valid output size");
    if (nn_in != file_in || nn_out != file_out)
    {
        std::stringstream sstream;
        sstream << std::endl <<
            "ERROR : the provided neural map must" <<
            " have " << file_in << " inputs and " <<
            file_out << " outputs." << std::endl;
        throw OptionErrorException (sstream.str ().c_str ());
    }
}

float compute_energy (float* sp_l, float* sp_r)
{
  float sum = 0;

  for (int i = 0; i < SPECTRUM_WIDTH; i++, sp_l++, sp_r++)
    sum += (*sp_l) * (*sp_l);

  return sum;
}

void launch_learning (Network* network, const char* music_path)
{
  FMOD::System     *system;
  FMOD::Sound      *music;
  FMOD::Channel    *channel = 0;
  FMOD_RESULT      result;
  int              key;
  unsigned int     version;
  float            spectrum_l[SPECTRUM_WIDTH];
  float            spectrum_r[SPECTRUM_WIDTH];
  time_t           prev;
  time_t           cur;

  bool run = true;

  result = FMOD::System_Create(&system);
  ERRCHECK(result);

  result = system->getVersion(&version);
  ERRCHECK(result);

  if (version < FMOD_VERSION)
  {
    fprintf (stderr, "Error!  You are using an old version of FMOD \
             %08x.This program requires %08x\n", version, FMOD_VERSION);
    exit(0);
  }


  result = system->setOutput(FMOD_OUTPUTTYPE_ALSA);
  ERRCHECK(result);

  result = system->init(32, FMOD_INIT_NORMAL, 0);
  ERRCHECK(result);


  result = system->createSound(music_path, FMOD_SOFTWARE, 0, &music);


  ERRCHECK(result);

  result = music->setLoopCount(-1);
  int val;
  music->getLoopCount(&val);
  std::cout << "Loop count : " << val<< std::endl;
  ERRCHECK(result);


  result = system->playSound(FMOD_CHANNEL_FREE, music, 0, &channel);


  prev = time (NULL);
  cur = time (NULL);

  double delta_rate = 1. / (2. * MAX_ITERATIONS);

  double inputs[HISTORY_SIZE] = {0.};
  double output = 0;
  size_t eff_nb_data = 0.;
  size_t nb_epochs = 0;
  double acc_err = 0;
  bool   song_started = false;

  while (run)
  {
    double diff = difftime (cur, prev);
    if (diff < 0.025)
    {
      sleep (0.025 - diff);
      prev = cur;
      cur = time (NULL);
    }

    channel->getSpectrum (spectrum_l, SPECTRUM_WIDTH, 0,
                           FMOD_DSP_FFT_WINDOW_RECT);
    channel->getSpectrum (spectrum_r, SPECTRUM_WIDTH, 1,
                           FMOD_DSP_FFT_WINDOW_RECT);

    double energy = compute_energy (spectrum_l, spectrum_r);

    for (size_t i = HISTORY_SIZE; i > 0; i--)
      inputs[i - 1] = inputs[i];

    if (!song_started && eff_nb_data > HISTORY_SIZE)
      song_started = true;

    eff_nb_data++;

    if (song_started && nb_epochs < MAX_ITERATIONS)
    {
      network->train_bp (&energy, inputs);
      network->interpolate (&output, inputs);


      if (output > THRESHOLD)
        std::cout << "beat" << std::endl;

      acc_err += sqme (&energy, &output, 1);

      if (eff_nb_data > SAMPLE_SIZE)
      {
        network->adjust_rate (delta_rate);
        acc_err /= (double)eff_nb_data;
        eff_nb_data = 0;
        std::cout << acc_err << std::endl;
        acc_err = 0;
        nb_epochs++;
      }
    }
  }
}

int main (int argc, char** argv)
{
  MapParser parser;
  std::cout << std::endl;
  std::cout << "NBOX beat learning program" << std::endl;
  std::cout << std::endl;

  try
  {
    check_opt (argc, argv);


    // Network building

    parser.parse_file (argv[2]);
    Network* network = parser.build_network (1., "sigmoid");

    size_t icount = network->inputs_count ();
    size_t ocount = network->outputs_count ();

    check_io_sizes (HISTORY_SIZE, 1, icount, ocount);


    std::cout << "Learning from song : " << argv[1] << std::endl;

    launch_learning (network, argv[1]);

    // Learning phase

    std::cout << "Saving weight ouput file..." << std::endl;

    network->save_weights (argv[4]);

    std::cout << "Weight ouput file successfully saved" << std::endl;
  }
  catch (std::exception& ex)
  {
    std::cerr << ex.what () << std::endl;
    return 1;
  }
  return 0;
}
