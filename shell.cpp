#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>

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

  if(!s.size()) return -1;
  
  if (s.size() > 100) {
    cout << "Error: input contains more than 100 characters.";
    return -1;
  }

  if (s.find("exit") != -1) return 0;


  bool contains_input_redirection = false;
  bool contains_output_redirection = false;
  bool contains_pipe = false;
  
  int pos = 0;
  for (int i = 0; i < s.size(); i++){
    if (s.at(i) == INPUT_REDIRECTION || s.at(i) == OUTPUT_REDIRECTION || s.at(i) == PIPE){
      if (i == (s.size() - 1) || i == 0){
	cout << "Error: input contains invalid pipe or redirection token position.";
	return -1;
      }

      if (contains_input_redirection && s.at(i) == INPUT_REDIRECTION) {
	cout << "Error: Multiple input redirections not allowed.";
	return -1;
      }
      if (contains_output_redirection && (s.at(i) == OUTPUT_REDIRECTION || s.at(i) == INPUT_REDIRECTION)) {
	cout << "Error: Multiple output redirections not allowed.";
	return -1;
      }
      if (contains_pipe && (s.at(i) == OUTPUT_REDIRECTION || s.at(i) == INPUT_REDIRECTION)) {
	cout << "Error: Pipe and redirection operator not allowed.";
	return -1;
      }

      if (s.at(i) == INPUT_REDIRECTION) contains_input_redirection = true;
      if (s.at(i) == OUTPUT_REDIRECTION) contains_output_redirection = true;
      if (s.at(i) == PIPE) contains_pipe = true;
      
      if (s.at(i-1) == 0x20 && s.at(i+1) == 0x20){

	if(s.substr(pos, i-pos) == " "){
	  cout << "Error: no whitespace only command allowed";
	  return -1;  
	}
	
	token_groups.push_back(trim_token_group(s.substr(pos, i-pos)));
	token_groups.push_back(string(1, s.at(i)));
	pos = i+2;
      }
      
      else {
	cout << "Error: input contains invalid character.";
	return -1;
      }
    }
  }
  if(s.substr(pos).size() < 1){
    cout << "Error: no whitespace only command allowed";
    return -1;
  }
  token_groups.push_back(trim_token_group(s.substr(pos)));
  return 1; 
}

int execute_input(vector<string> &v)
{
  int pos = 1;
  //  while (pos != v.size() - 1)
  cout << "Command: '" + v.at(0) +"'" << endl;
  vector< vector<char> > args_vector;

  string s;
  stringstream ss(v.at(0));

  while(getline(ss, s, ' ')){
    cout << "Command chunk is: " + s << endl;
    args_vector.push_back(vector<char>(s.begin(), s.end()));
    args_vector.push_back(args_vector.back().push_back('\0'));
  }

  char *command_args[args_vector.size() + 1];

  for(int i = 0; i < args_vector.size(); i++){
    char *command_arg = &args_vector[i][0];   
    command_args[i] = command_arg;
  }

  for(int i = 0; i < args_vector.size(); i++){
    cout << "arg is : '";
    for(int j = 0; j < args_vector[i].size(); j++){
      cout << command_args[i][j];
    }
    cout << "'\n";
  }

  command_args[args_vector.size() + 1] = NULL;

  cout<< command_args[args_vector.size() + 1] << endl;
  
  /*stringstream tss(v.at(0));
  int j = 0;
  while(getline(tss, s, ' ')){
    /*cout << "Command chunk from command_args is: ";
    for (int i = 0; i < s.size(); i++){
      cout << command_args[j][i];
      }*/
  /*cout << "Command chunk from args_vector is: ";
    for (int i = 0; i < s.size(); i++){
      cout << args_vector[j][i];
      }
    cout << '\n';
    j++;
    }*/

  
  execvp(command_args[0], command_args);

  //char * args[] = {"", "", NULL};
	
    //int pid = fork();
   
    /* Child process */
  /*   if (pid == 0) {
      if (v.at(pos).at(0) == PIPE) {
	int pipefd[2];
	//	  pipe(pipefd);
      }
      else if (v.at(pos).at(0) == OUTPUT_REDIRECTION) {

      }
      else if (v.at(pos).at(0) == INPUT_REDIRECTION) {

      }
    }
  */  /* Parent process */
  /*   else if (pid > 0) {
      
    }
    else {
      cerr << "Error: Invalid pid." << endl;
      return -2;
    }
    
    }*/
  return 1;
}
  
int main()
{
  string input;
  vector<string> token_groups;
  cout << "shell> ";
  while (getline(cin, input)) {
    int process_status = process_input(input, token_groups);
    
    if(!process_status){
      break;
    }
    
    if(process_status > 0){  

      //Testing
      /*for(int i = 0; i < token_groups.size(); i++)
	cout << token_groups.at(i) << endl;*/

      execute_input(token_groups);

    }
    cout << "\nshell> ";
  }
}
 
