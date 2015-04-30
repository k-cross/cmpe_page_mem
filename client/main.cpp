/*
 * <Author> Kenneth Cross
 * <Date> 04/27/2015
 * <Matter> Main memory client connected through tcp
 */

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "tcp_cl.cpp"

using boost::asio::ip::tcp;

int main(int argc, char* argv[]){
    if (argc < 2){
        std::cerr << "Usage: " << argv[0] 
            << " <server-address> and/or <file path>" << std::endl;
        return __LINE__;
    }

    std::string server_ip = argv[1];
    std::string path = argv[2]; // Probably don't care about this
    std::string more = "n"; // string to break loop

    for(;;){
        try{
            boost::asio::io_service io_service;
            async_tcp_client client(io_service, argv[1], argv[2]);
            io_service.run();

            std::cout << "send file " << argv[2] << " completed successfully.\n";
        }

        catch (std::exception& e){
            std::cerr << e.what() << std::endl;
        }

        std::cout << "Map more memory? 'y' or 'n': ";
        std::cin >> more;

        if(more == "n"){
            break;
        }
    }

    return 0;
}
