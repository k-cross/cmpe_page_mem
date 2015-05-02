#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <sstream>
#include "tcp_cl.cpp"
using boost::asio::ip::tcp;

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "tcp_sv.cpp"
using boost::asio::ip::tcp;


void file_send(const std::string& ip_addr, const std::string& filename){
    try{
        boost::asio::io_service io_service;
        async_tcp_client client(io_service, serv, path);
        io_service.run();

        std::cout << "send file " << path << " completed successfully.\n";
    }

    catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}

void file_recieve(const std:sting& ip_addr){
    int tcpp = 4444;

    try{
        std::cout << argv[0] << " listen on port " << tcpp << std::endl;
        async_tcp_server *recv_file_tcp_server = new async_tcp_server(tcpp);
        delete recv_file_tcp_server;
    }

    catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}
