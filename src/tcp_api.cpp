/*
 * <Author> Kenneth Cross
 * <Date> 05/02/2015
 * <Matter> API functions used to send and recieve strings and files
 *
 * <How-To Use> All functions listen on port 4444
 * File Send: Include reciever's IP as a string
 * File Recieve: Listens until connection is made
 * 
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "./client/tcp_cl.cpp"
#include "./server/tcp_sv.cpp"
using boost::asio::ip::tcp;

void file_send(const std::string& ip_addr, const std::string& filename){
    try{
        boost::asio::io_service io_service;
        async_tcp_client client(io_service, ip_addr, filename);
        io_service.run();

        std::cout << "send file " << filename << " completed successfully.\n";
    }

    catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}

void file_recieve(){
    int tcpp = 4444;

    try{
        std::cout << "Using port:  " << tcpp << std::endl;
        async_tcp_server *recv_file_tcp_server = new async_tcp_server(tcpp);
        delete recv_file_tcp_server;
    }

    catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}
