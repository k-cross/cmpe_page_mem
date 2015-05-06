#include <iostream>
#include <string>
#include "../tcp_api.cpp"
#include "./netlinkUser.c"

int main(int argc, char* argv[]){
  std::string ip_addr = argv[1];
  std::string filepath = argv[2];

  file_send(ip_addr, filepath);

  captains_log("this is a test");

  return 0;
}
