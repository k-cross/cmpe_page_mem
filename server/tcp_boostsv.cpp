/*
 * <Author> Kenneth Cross
 * <Date> 04/27/2015
 * <Matter> Boost libraries used for tcp communication with server
 *
 * Constants are in CAPS
 * Member Functions contain trailing _
 *
 * Strings: grab an ip address and something to stream
 */

#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class tcp_connection : public boost::enable_shared_from_this<tcp_connection>{
public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_service& io_service){
    return pointer(new tcp_connection(io_service));
  }

  tcp::socket& socket(){
    return socket_;
  }

  void start(){
    message_ = "Initial Server Connection Established\n";

    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&tcp_connection::handle_write, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

private:
  tcp_connection(boost::asio::io_service& io_service) : socket_(io_service)
  {
  }

  void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
  {
  }

  tcp::socket socket_;
  std::string message_;
  std::string mem_stream_;
};

class tcp_server{
public:
  tcp_server(boost::asio::io_service& io_service)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), 6666)){ //listen on port 6666
    start_accept();
  }

private:
  void start_accept(){
    tcp_connection::pointer new_connection =
      tcp_connection::create(acceptor_.get_io_service());

    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
  }

  void handle_accept(tcp_connection::pointer new_connection,
      const boost::system::error_code& error){
    if (!error){
      new_connection->start();
    }

    start_accept();
  }

  tcp::acceptor acceptor_;
  std::string ip_addr;
};

class tcp_query{
public:
  tcp_query(boost::asio::io_service& io_service, std::string& ip, std::string& stream);
  void tcp_setup(){
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(ip_addr_, "6666"); /* Port Number or Service Name */
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);

    for (;;){
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
    }
  }
}

private:
  tcp_query(std::string ip = "127.0.0.1"){ip_addr_ = ip;}  /* Constructor */
  ~tcp_query(); /* Destructor */
  
  std::string ip_addr_;
};
