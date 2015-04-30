#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <sstream>
#include "tcp_cl.cpp"
using boost::asio::ip::tcp;

int main(int argc, char* argv[]){
    std::string serv = argv[1];
    std::string path = argv[2];
    std::string more = "n";

    if (argc != 3){
        std::cerr << "Usage: " << argv[0] << " <server-address> <file path>" << std::endl;
        return __LINE__;
    }

    for(;;){
        try{
            boost::asio::io_service io_service;
            async_tcp_client client(io_service, serv, path);
            io_service.run();

            std::cout << "send file " << path << " completed successfully.\n";
        }
        catch (std::exception& e){
            std::cerr << e.what() << std::endl;
        }

        std::cout << "more? ";
        std::cin >> more;

        if(more == "n")
            break;
    }
    return 0;
}
