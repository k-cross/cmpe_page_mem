#include <iostream>
#include <string>
#include "../tcp_api.cpp"

extern "C" {
#include "./netlinkUser.c"
}

int main(int argc, char* argv[]){
  std::string ip_addr = argv[1];
  std::string filepath = argv[2];

  file_send(ip_addr, filepath);

  char a[25] = "this is a test";
 <char*>captains_log(a);

  return 0;
}
