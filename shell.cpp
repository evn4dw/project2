#include "shell.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

const char shell::INPUT_REDIRECTION = '<';
const char shell::OUTPUT_REDIRECTION = '>';
const char shell::PIPE = '|';
const std::string SYS_EXIT = "exit"; 

shell::shell()
{

}

void shell::process_input(std::ostream & out, std::string s)
{
  if (s.size() > 100) {
    out << "Error: Cannot process lines longer than 100 char." << std::endl;
    return;
  }

  do {
    std::size_t pos_input = s.find(INPUT_REDIRECTION);
    std::size_t pos_output = s.find(OUTPUT_REDIRECTION);
    std::size_t pos_pipe = s.find(PIPE);

    if (pos_input == std::string::npos && 
	pos_output == std::string::npos && pos_pipe == std::string::npos) {
      run_command(out, s);
      return;
    }
      
    std::cout << "POS_INPUT: " << pos_input << std::endl;
    std::cout << "POS_OUTPUT: " << pos_output << std::endl;
    std::cout << "POS_PIPE: " << pos_pipe << std::endl;

    if (pos_input < pos_output && pos_input < pos_pipe)
      out << "INPUT";

    else if (pos_output < pos_input && pos_output < pos_pipe)
      out << "OUTPUT";

    else if (pos_pipe < pos_input && pos_pipe < pos_output)
      out << "PIPE";

    pid_t pid = fork();

    if (pid == 0) {
      out << "Child process" << std::endl;

      execvp();
    }
    else if (pid > 0) {
      out << "Parent process" << std::endl;
    } 

    out << s << std::endl;
  }
  while (!(pos_input == std::string::npos) || 
	 !(pos_output == std::string::npos) ||
	 !(pos_pipe == std::string::npos));
}

bool shell::verify_word(std::ostream & out, std::string s)
{
 for (int i = 0; i < s.size(); i++) {
    int dec = (int) s.at(i);
    bool range1 = (dec >= 45) && (dec <= 57);
    bool range2 = (dec >= 65) && (dec <= 90);
    bool range3 = (dec >= 97) && (dec <= 122);
    bool range4 = (dec == 95);
    if (! (range1 || range2 || range3 || range4)) {
      out << "Error: Word contains invalid character." << std::endl;
      return false;
    }
  }
 return true;
 }

void shell::run_command(std::ostream & out, std::string s)
{
  if (!s.at(0) == '/') {  

  }
  exec
}
