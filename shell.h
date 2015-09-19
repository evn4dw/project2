#include <string>
#include <iostream>

class shell {

 public:
  shell();
  void process_input(std::ostream &, std::string line);

 private:
  static const char INPUT_REDIRECTION;
  static const char OUTPUT_REDIRECTION;
  static const char PIPE;

  bool verify_word(std::ostream &, std::string word);
  void run_command(std::ostream &, std::string command);
};
