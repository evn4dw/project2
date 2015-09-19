#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>

const char INPUT_REDIRECTION = '<';
const char OUTPUT_REDIRECTION = '>';
const char PIPE = '|';
const std::string SYS_EXIT = "exit"; 

bool verify_word(std::string s)
{
 for (int i = 0; i < s.size(); i++) {
    int dec = (int) s.at(i);
    bool range1 = (dec >= 45) && (dec <= 57);
    bool range2 = (dec >= 65) && (dec <= 90);
    bool range3 = (dec >= 97) && (dec <= 122);
    bool range4 = (dec == 95);
    if (!(range1 || range2 || range3 || range4)) {
      return false;
    }
 }
 return true;
}

std::vector<std::string> process_input(std::ostream &out, std::string s)
{
  std::vector<std::string> input(10);
  if (s.size() > 100) {
    out << "Error: Cannot process lines longer than 100 char." << std::endl;
    return input;
  }
  if (!verify_word(s)) {
    out << "Error: Invalid characters found in line.";
    return input;
  }

  int pos = 0;
  int i = 0;
  for (i; i < s.size(); i++){
    if (s.at(i) == INPUT_REDIRECTION || s.at(i) == OUTPUT_REDIRECTION || s.at(i) == PIPE){
      input.push_back(s.substr(pos, i));
      input.push_back("" + s.at(i));
      pos = i+1;
    }
  }
  input.push_back(s.substr(pos, i));
  return input; 
}

int main()
{
  std::string input;
  std::cout << "shell>" << std::endl;
  while (std::cin >> input) {
    std::vector<std::string> v = process_input(std::cout, input);
    for(int i = 0; i < v.size(); i++)
      std::cout << v[i] << std::endl;
    std::cout << "shell>" << std::endl;
  }
}
 
