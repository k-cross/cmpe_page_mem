#include <iostream>
#include <string>
#include "../tcp_api.cpp"

int main(int argc, char* argv[]){
  std::string arguments = argv[1];
  std::string base_command = "./nettest ";
  std::string full_command = base_command + arguments;

  FILE* file = popen(full_command.c_str(), "r");
  char buffer[100];
  fscanf(file, "%100s", buffer);
  pclose(file);

  std::string from_buffer = buffer; //c++ string easier to work with

  return 0;
}
