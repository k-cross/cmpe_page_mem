#include <iostream>
#include <string>
#include "../tcp_api.cpp"

int main(int argc, char* argv[]){
  std::string ip_addr = argv[1];
  std::string filepath = argv[2];

  file_send(ip_addr, filepath);

  return 0;
}
