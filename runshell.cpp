#include <iostream>
#include "shell.h"


int main() {
  
  shell s;

  std::string input;
  std::cout << "shell>" << std::endl;
  while (std::cin >> input) {
      s.process_input(std::cout, input);
      std::cout << "shell>" << std::endl;
  }
}
 










