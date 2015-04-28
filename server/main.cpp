/*
 * <Author> Kenneth Cross
 * <Date> 04/27/2015
 * <Matter> Main memory server connected through tcp
 */

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "tcp_boostsv.cpp"
using boost::asio::ip::tcp;

int main(int argc, char* argv[]){
  try{
    /* Uncomment the lines below after client completion */

    /*if (argc != 2){
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }*/

    boost::asio::io_service io_service;
    tcp_server server(io_service);
    io_service.run();
  }

  catch (std::exception& e){
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
