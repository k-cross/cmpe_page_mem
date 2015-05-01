//Client
//Dont forget to STD::REMOVE("MESSAGE") in server

#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <csignal>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "tcp_cl.cpp"
#include "../server/tcp_sv.cpp"
using namespace std;
using boost::asio::ip::tcp;

/* Global Vars */
void* r_addr = (int*)0xaaaa;
int r_len = 0x4000;
char value[25];
std::string server;
std::string path; 
std::string cs_msg = "message"; 
std::string more = "n";
int tcpp = 6666;
    
void* __attribute__((weak)) mmap(void *addrs, 
    size_t lengths, int prots, int flag, int fds)
{
	void* nu = 0;
	if(addrs == r_addr){
		cout << "Potential Page or Segmentation Fault detected!\n";
		cout << "Redirecting to Server Remote Memory...\n\n";
		raise(SIGINT);
	}
	else{
		cout << "Non conflicting memory detected, check mprotect()\n";
	}

	return nu;
}

size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

void r_mapHandler(int signum){
    more = "n";
	cout << "Sending Value: " << value << " to be mapped into Remote Memory\n";

	// ********************VALUE SENT TO SERVER*******************
    for(;;){
	    cout << "\n**SHOULD NOW SEND TO SERVER (DEBUG)**\n";
        cout << "Path: "; cin >> path;
        try{
            boost::asio::io_service io_service;
            async_tcp_client client(io_service, server, path);
            io_service.run();

            std::cout << "send file " << path << " completed successfully.\n";
        }
        catch (std::exception& e){
            std::cerr << e.what() << std::endl;
        }

        std::cout << "Send more values ('y' or 'n')? ";
        std::cin >> more;

        if(more == "n")
            break;
    }
}

int main(int argc, char** argv) {
    server = argv[1];
    path = argv[2];

    if (argc < 2){
        std::cerr << "Usage: " << argv[0] 
            << " <server-address> <file path>\n";
        return __LINE__;
    }
    
	const char* tmp = "tmp.txt";
	ofstream ofile;
	int opt = 4;

	int* map;
    size_t filesize;
    int fd;

    signal(SIGINT, r_mapHandler);

	// Protect Client's memory
    cout << "Client Initialized!\n";

    if(mprotect(r_addr, r_len, PROT_READ)){
    	cout << "\nAcquiring Clientside memory at: " << r_addr 
            << " with length:" << hex << "0x" << r_len << endl;
    	cout << "Clientside memory is locked, redirecting mmap() to Remote Memory\n";
    }
    else
    	cout << "mprotect has failed, please restart!\n";

	while(opt != 0){
		cout << "\n*************Please Make a Selection**************\n";
		cout << "Enter 1 to map a value to remote memory\n";
		cout << "Enter 2 to request a sync of remote memory\n";
		cout << "Enter 0 to Exit!\n";
		cout << "**************************************************\n\n"; 

		cin >> opt;

		// Manually map
		if(opt == 1){
			cout << "Please enter a value to be mapped into shared memory...\n";
			cin >> value;
			ofile.open(tmp);
			ofile << value;
			ofile.close();

    		filesize = getFilesize(tmp);

    		//Open file
    		fd = open(tmp, O_RDONLY, 0);
    		assert(fd != -1);

    		cout << "Calling mmap to page into memory...\n";

    		//Execute mmap (weak)
    		map = (int*)mmap(r_addr, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd);
    		if (map != 0)
    			cout << "mmap function failed, Please Restart!\n"; 

    		close(fd);

		}
		// Sync Time! ************************HERE*******************
		if(opt == 2){
			cout << "Sync Request Sent to server..." << endl;
			// **********TODO Send Sync request to Server*********************
            // **********WORKING*********************

            ofstream outputFile("message");
            outputFile << "sync";
            outputFile.close();

            try{
                boost::asio::io_service io_service;
                async_tcp_client client(io_service, server, cs_msg);
                io_service.run();

                std::cout << "send file " << path << " completed successfully.\n";
            }
            catch (std::exception& e){
                std::cerr << e.what() << std::endl;
            }

            std::remove("message");
            more = "n";

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

			//wait
			cout << " \n**VALUES AND ADDRESSES FROM SERVER SHOULD BE HERE! (DEBUG)**\n\n"; 
		}
		// ********TODO The Sync output from Server should be outputted here **************
		if(opt == 3){
		}
	}
}
