#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

const char INPUT_REDIRECTION = '<';
const char OUTPUT_REDIRECTION = '>';
const char PIPE = '|';
const string SYS_EXIT = "exit"; 

bool verify_token_group(string s) {
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

string trim_token_group(string s){
  s = s.substr(s.find_first_not_of(" \f\n\r\t\v"));
  s = s.substr(0, s.find_last_not_of(" \f\n\r\t\v")+1);
  
  int i = 0, j = 0, lastSpace = 0;

  for(i; s[i]; i++){
    if(!lastSpace || s[i] != ' '){
      s[j] = s[i];
      j++;
    }
    lastSpace = (s[i] == ' ');
   }
  s.resize(j);
  return s;
}

int process_input(string s, vector<string> &token_groups){
  token_groups.resize(0);

  if (s.size() == 0) {
    cout << "Error: input contains no characters.";
    return -1;
  }

  if (s.size() > 100) {
    cout << "Error: input contains more than 100 characters.";
    return -1;
  }

  int pos = 0;
  for (int i = 0; i < s.size(); i++){
    if (s.at(i) == INPUT_REDIRECTION || s.at(i) == OUTPUT_REDIRECTION || s.at(i) == PIPE){
      if (i == (s.size() - 1) || i == 0){
	cout << "Error: input contains invalid pipe or redirection token position.";
	return -1;
      }
      if (s.at(i-1) == 0x20 && s.at(i+1) == 0x20) {
	token_groups.push_back(trim_token_group(s.substr(pos, i-1)));
	token_groups.push_back(string(1, s.at(i)));
	pos = i+2;
       }
      else {
	cout<< "Error: input contains invalid character.";
	return -1;
      }
    }
  }
  token_groups.push_back(trim_token_group(s.substr(pos)));
  return 1; 
}

int main()
{
  string input;
  vector<string> token_groups;
  cout << "shell> ";
  while (getline(cin, input)) {
    process_input(input, token_groups);
    for(int i = 0; i < token_groups.size(); i++)
      cout << token_groups.at(i) << endl;
    cout << "\nshell> ";
  }
}
 
