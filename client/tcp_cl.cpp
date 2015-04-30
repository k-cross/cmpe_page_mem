/*
 * <Author> Kenneth Cross
 * <Date> 04/30/2015
 * <Matter> Official client protocol for shared memory
 */

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <sstream>
using boost::asio::ip::tcp;

std::string tcp_port = "6666";

class async_tcp_client{
public:
    async_tcp_client(boost::asio::io_service& io_service,
        const std::string& server_ip, const std::string& path)
        : resolver_(io_service), socket_(io_service)
    {
        size_t pos; /* Might Not Need This */

        // Open file to be streamed
        in_stream.open(path.c_str(), std::ios_base::binary | std::ios_base::ate);
        if(!in_stream){ 
            std::cout << "Failed to stream " << path << std::endl; 
            return ;
        }

        size_t file_size = in_stream.tellg();
        in_stream.seekg(0); // Start at the beginning

        // Send filename and size to the server first
        std::ostream request_stream(&request_);
        request_stream << path << '\n' << file_size << "\n\n";
        std::cout << "Request Size: " << request_.size() << '\n';

        /* Start resolve to translate server and service names
         * into a list of endpoints
         */

        tcp::resolver::query query(server_ip, tcp_port);
        resolver_.async_resolve(query, 
            boost::bind(&async_tcp_client::handle_resolve, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::iterator));
    }
private:
    void handle_resolve(boost::system::error_code& err,
        tcp::resolver::iterator endpoint_iterator)
    {
        if(!err){
            /* Attempt connection to the first endpoint. Each endpoint
             * will be tried until a successful connection's made.
             */

            tcp::endpoint endpoint = *endpoint_iterator;
            socket_.async_connect(endpoint,
                boost::bind(&async_tcp_client::handle_connect, this,
                boost::asio::placeholders::error, ++endpoint_iterator));
        }
        else{
            std::cout << "Error: " << err.message() << "\n";
        }
    }

    void handle_connect(boost::system::error_code& err,
        tcp::resolver::iterator endpoint_iterator)
    {
        if (!err){
           // Successful connection / send data
           boost::asio::async_write(socket_, request_,
               boost::bind(&async_tcp_client::handle_write_file, this,
               boost::asio::placeholders::error));
        }
        else if (endpoint_iterator != tcp::resolver::iterator()){
            // Failed -> Try next
            socket_.close();
            tcp::endpoint endpoint = *endpoint_iterator;
            socket_.async_connect(endpoint,
                boost::bind(&async_tcp_client::handle_connect, this,
                boost::asio::placeholders::error, ++endpoint_iterator));
        }
        else{
            // Complete Failure
            std::cout << "Error: " << err.message() << "\n";
        }
    }

    void handle_write_file(const boost::system::error_code& err){
        if (!err){

            if(in_stream.eof() == false){
                in_stream.read(buf.c_array(), (std::streamsize)buf.size());

                if (in_stream.gcount()<=0){
                    std::cout << "Read file error " << std::endl;
                    return;
                }

                std::cout << "Send " << in_stream.gcount() 
                    << " bytes, total: " << in_stream.tellg() << " bytes.\n";

                boost::asio::async_write(socket_,
                    boost::asio::buffer(buf.c_array(), in_stream.gcount()),
                    boost::bind(&async_tcp_client::handle_write_file, this,
                    boost::asio::placeholders::error));

                if (err){
                    std::cout << "send error:" << err << std::endl;
                    return;
                }
            }
            else{
                return;
            }
        }
        else{
            std::cout << "Error: " << err.message() << "\n";
        }

    }
    tcp::resolver resolver_;
    tcp::socket socket_;
    boost::array<char, 1024> buf;
    boost::asio::streambuf request_; //Buffer's what stores the requested file to be sent
    std::ifstream in_stream; //Might want to change stream type
};
