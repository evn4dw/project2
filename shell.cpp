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

bool verify_token_order(vector<string> &v) {
  bool contains_pipe = false;
  bool contains_input_redirection = false;
  bool contains_output_redirection = false;

  for (int i = 0; i < v.size(); i++) {
    if (contains_output_redirection && v.at(i).at(0) == INPUT_REDIRECTION)
      return false;
    if (contains_output_redirection && v.at(i).at(0) == OUTPUT_REDIRECTION)
      return false;
    if (contains_input_redirection && v.at(i).at(0) == INPUT_REDIRECTION)
      return false;

    if (v.at(i).at(0) == PIPE)
      contains_pipe = true;
    if (v.at(i).at(0) == INPUT_REDIRECTION)
      contains_input_redirection = true;
    if (v.at(i).at(0) == OUTPUT_REDIRECTION)
      contains_output_redirection = true;
    
    if (contains_pipe && (contains_input_redirection || contains_output_redirection))
      return false;
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
    //If empty, process next line of input without error message
    return -1;
  }

  if (s.size() > 100) {
    cout << "Error: input contains more than 100 characters.";
    return -1;
  }

  if (trim_token_group(s) == "exit") {
    return -2;
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
	cout << "Error: input contains invalid character.";
	return -1;
      }
    }
  }
  token_groups.push_back(trim_token_group(s.substr(pos)));
  if (!verify_token_order(token_groups)) {
      cout << "Error: invalid pipe or redirection tokens." << endl;
      return -1;
    }
  return 1; 
}

int execute_input(vector<string> &v)
{
  int pos = 1;
  while (pos != v.size() - 1) {
    if (v.at(pos).at(0) == PIPE) {
      int pipefd[2];
      
      //Set up pipe
    }
	
    int pid = fork();
    
    /* Child process */
    if (pid == 0) {
      if (v.at(pos).at(0) == PIPE) {
	int pipefd[2];
	//	char * args[] = {"", "",  };
	//	char * 

	//	  pipe(pipefd);
      }
      else if (v.at(pos).at(0) == OUTPUT_REDIRECTION) {

      }
      else if (v.at(pos).at(0) == INPUT_REDIRECTION) {

      }
    }
    /* Parent process */
    else if (pid > 0) {
      
      
      
    }
    else {
      cerr << "Error: Invalid pid." << endl;
      return -2;
    }
    
  }
}
  
int main()
{
  string input;
  vector<string> token_groups;

  char * buffer = new char[100];
  string cwd = getcwd(buffer, 100);
  cout << cwd;
  while (getline(cin, input)) {
    int exit_code = process_input(input, token_groups);
    if (exit_code == -2)
      break;
    else
      int completed = execute_input(token_groups);


    // Testing
    for(int i = 0; i < token_groups.size(); i++)
      cout << token_groups.at(i) << endl;
    cout << "\nshell> ";
  }
}
 
