/*
 * <Author> Kenneth Cross
 * <Date> 04/27/2015
 * <Matter> Main memory server connected through tcp
 */

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "tcp_sv.cpp"
using boost::asio::ip::tcp;

int main(int argc, char* argv[]){
    std::string tcp_port = "6666";
    std::string more = "n";
    int tcpp = 6666;

    for(;;){
        try{
            std::cout << argv[0] << " listen on port " << tcp_port << std::endl;
            async_tcp_server *recv_file_tcp_server = new async_tcp_server(tcpp);
            delete recv_file_tcp_server;
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
