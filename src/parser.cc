#include "parser.hh"


MapParser::MapParser ()
{
  tk_info[0] = TkInfo (&MapParser::parse_input, "input");
  tk_info[1] = TkInfo (&MapParser::parse_neuron, "neuron");
}

MapParser::~MapParser ()
{
}

void MapParser::parse_file (const char* file)
{
  std::ifstream filestream;
  filestream.open (file);
  std::string line;


  while (getline (filestream, line))
    next_command_ (line.c_str (), line.length ());
}

Network* MapParser::retrieve_network ()
{
  Network* network = new Network (first_layer_, neural_map_);
  return network;
}

char MapParser::to_lower_ (char c)
{
  if (c >= 'A' && c <= 'Z')
    return c + ('a' - 'A');
  return c;
}

void MapParser::parse_neuron (const char* line)
{
  std::stringstream sstream (std::stringstream::in | std::stringstream::out);

  sstream << line;

  unsigned next = 0;

  neural_map_.push_back (new std::vector<unsigned> ());
  std::vector<unsigned>* cur_neuron = neural_map_.back ();
  while (sstream >> next)
    cur_neuron->push_back (next);
}

void MapParser::parse_input (const char* line)
{
  first_layer_.push_back (neural_map_.size ());
  parse_neuron (line);
}

void MapParser::next_command_ (const char* line, size_t size)
{
  size_t i = 0;

  std::vector<TkInfo>* old_candidats = new std::vector<TkInfo> ();
  std::vector<TkInfo>* new_candidats = new std::vector<TkInfo> ();

  for (size_t i = 0; i < NB_TOKENS; i++)
    old_candidats->push_back (tk_info[i]);

  // start parsing
  while (line[i] == ' ')
    i++;

  // keep track of the starting postion of the word encoutered
  // size_t start = i;

  do
  {
    while (!old_candidats->empty ())
    {
      TkInfo cur_tk = old_candidats->back ();
      old_candidats->pop_back ();
      if (*cur_tk.token == to_lower_ (line[i]))
        new_candidats->push_back (TkInfo (cur_tk.func, cur_tk.token + 1));
    }
    i++;
    old_candidats->clear ();

    //swapping new and old candidats
    std::vector<TkInfo>* tmp = old_candidats;
    old_candidats = new_candidats;
    new_candidats = tmp;

  } while (!old_candidats->empty () && i < size && line[i] != ' ');

  delete new_candidats;
  if (old_candidats->empty ())
  {
    delete old_candidats;
    throw UnknownTokenException ();
  }
  TkInfo top = old_candidats->back ();
  delete old_candidats;

  // call the appropriate parsing method
  (this->*(top.func)) (&line[i]);
}
