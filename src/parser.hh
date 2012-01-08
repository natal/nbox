/*
** file: parser.hh
** author: benjamin
** created on 29/12/11 at 07:12
*/

#ifndef PARSER_HH_
# define PARSER_HH_
# include <iostream>
# include <cassert>
# include <sstream>
# include <fstream>
# include "network.hh"
# include "exceptions.hh"

# define NB_TOKENS 2

class MapParser
{
  public:
    MapParser ();
    ~MapParser ();
    void parse_file (const char* file);
    Network* retrieve_network ();
    void parse_neuron (const char* line);
    void parse_input (const char* line);

  private:
    neuralMap neural_map_;
    std::vector<unsigned> first_layer_;
    char to_lower_ (char c);

    typedef void (MapParser::*parse_mthd) (const char*);
    class TkInfo
    {
      public:
        TkInfo ()
          : func (0), token (0)
        {
        }
        TkInfo (parse_mthd f, const char* tk)
          : func (f), token (tk)
        {
        }
        parse_mthd func;
        const char* token;
    };
    void next_command_ (const char* line, size_t size);
    TkInfo tk_info[NB_TOKENS];
};

#endif /* !PARSER_HH_ */
