#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>

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
  
  int i, j = 0, lastSpace = 0;

  for(i = 0; s[i]; i++){
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
    cout << "Error: input contains more than 100 characters.\n";
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
	cout << "Error: input contains invalid pipe or redirection token position.\n";
	return -1;
      }

      if (contains_input_redirection && s.at(i) == INPUT_REDIRECTION) {
	cout << "Error: Multiple input redirections not allowed.\n";
	return -1;
      }
      if (contains_output_redirection && (s.at(i) == OUTPUT_REDIRECTION || s.at(i) == INPUT_REDIRECTION)) {
	cout << "Error: Multiple output redirections not allowed.\n";
	return -1;
      }
      if (contains_pipe && (s.at(i) == OUTPUT_REDIRECTION || s.at(i) == INPUT_REDIRECTION)) {
	cout << "Error: Pipe and redirection operator not allowed.\n";
	return -1;
      }

      if (s.at(i) == INPUT_REDIRECTION) contains_input_redirection = true;
      if (s.at(i) == OUTPUT_REDIRECTION) contains_output_redirection = true;
      if (s.at(i) == PIPE) contains_pipe = true;
      
      if (s.at(i-1) == 0x20 && s.at(i+1) == 0x20){

	if(s.substr(pos, i-pos) == " "){
	  cout << "Error: no whitespace only command allowed\n";
	  return -1;  
	}

	if(!verify_token_group(s.substr(pos, i - pos))){
	  cout << "Error: A token group contains an invalid character\n";
	}
	token_groups.push_back(trim_token_group(s.substr(pos, i-pos)));
	token_groups.push_back(string(1, s.at(i)));
	pos = i+2;
      }
      
      else {
	cout << "Error: input contains invalid character.\n";
	return -1;
      }
    }
  }
  if(s.substr(pos).size() < 1){
    cout << "Error: no whitespace only command allowed.\n";
    return -1;
  }
  token_groups.push_back(trim_token_group(s.substr(pos)));
  return 1; 
}

void execute(vector<string> &v){
  if(true){
    int status;
    int num_pipes = 0;
    for(int i = 0; i < v.size(); i++){
      if(v[i] == "|"){
	num_pipes++;
	v.erase(v.begin() + i);
      }
    }
    
    int pipes[2 * num_pipes];
    
    for(int i = 0; i < num_pipes; i++) pipe(pipes + i * 2);

    vector <int> pids;
    
    for(int i = 0; i <= num_pipes; i++){
      pids.push_back(fork());
      if (pids.back()  == 0){
	if(i == 0) dup2(pipes[1], 1);
    
    
	else if(i == num_pipes) dup2(pipes[2 *(i-1)], 0);
   
	
        else{
	  dup2(pipes[2*(i-1)], 0);
	  dup2(pipes[2*i+1], 1);
        }
      
        for (int k = 0; k < (num_pipes * 2); k++) close(pipes[k]);
	
	stringstream ss(v[i]);
        vector< vector<char> > args_vector;
	char *args[50] = {NULL};
	string s;
      
	while(ss >> s){
	  vector<char> cv(s.begin(), s.end());
	  cv.push_back('\0');
	  args_vector.push_back(cv);
	}
	
	for(int i = 0; i < args_vector.size(); i++){
	  char *arg = &args_vector[i][0];   
	  args[i] = arg;
	}
      
	args[args_vector.size() + 1] = NULL;	
      
	int exit_code = execvp(args[0], args);
	cout << "Error: execution of command failed with exit code: " << exit_code << endl;
      }//closes if (fork() == 0)
    }//closes for loop for forking
    
    for (int i = 0; i < (num_pipes *2); i++) close(pipes[i]);
    
    for(int i = 0; i <= num_pipes; i++) wait(&status);
  }//closes if true
}

int main()
{
  string input;
  vector<string> token_groups;

  cout << "shell> ";

  while (getline(cin, input)) {
    int process_status = process_input(input, token_groups);
    cout << "Process status is: " << process_status << endl;
    if(!process_status) break;

    if(process_status > 0) execute(token_groups);
      
    cout << "shell> ";
  }
  return 0;
}
