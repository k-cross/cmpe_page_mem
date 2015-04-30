//Server

#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>
#include <vector>
#include "tcp_sv.cpp"
#include "../client/tcp_cl.cpp"

using namespace std;
using boost::asio::ip::tcp;

bool fexists(const char *filename)
{
      ifstream ifile(filename);
        return ifile;
}

size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

/* The fun starts here */

int main(int argc, char** argv) {
    // TCP Vars
    std::string tcp_port = "6666";
    std::string client_ip = argv[1];
    std::string more = "n";
    int tcpp = 6666;

    //Memory Server Vars
	char value[25];
	const char* tmp = "tmp.txt";
    std::ofstream ofile;
    std::ifstream ifile;
	int opt = 3;

    size_t filesize;
    int fd;
    int rip;
    int* map;
    vector<int*> pgs;

    cout << "Server process Initialized!\n";

    while (opt!=0){
        try{
            std::cout << argv[0] << " listen on port " << tcp_port << std::endl;
            async_tcp_server *recv_file_tcp_server = new async_tcp_server(tcpp);
            delete recv_file_tcp_server;
        }

        catch (std::exception& e){
            std::cerr << e.what() << std::endl;
        }

// **************Place cases here******************
        if(fexists("message")){  // File exists
            //do x
            ifile.open("message");
            ifile >> value;
            if(value == "map")
                opt = 1;
            else if(value == "sync")
                opt = 2;
        }
        else{
            cout << "Nothing to do...\n";
            opt = 0;
        }

        // Case 1. Map a Page
    	if (opt == 1){
    		cout << "Remote Page In of Value:" << value << " requested.\n";
    		cout << "Mapping page into memory...\n";

    // TODO: Get Value from Client and put it in value
    // NOTE to Kenny, you may have to change opt and create
    // a loop here to start case 1 and case 2 based off of messages from Client

            /* Passing messages through a file named message */
            //***********************************************

            for(;;){
                try{
                    std::cout << argv[0] << " listen on port " << tcp_port << std::endl;
                    async_tcp_server *recv_file_tcp_server = new async_tcp_server(tcpp);
                    delete recv_file_tcp_server;
                }

                catch (std::exception& e){
                    std::cerr << e.what() << std::endl;
                }

                std::cout << "Get more stuff? 'y' or 'n': ";
                std::cin >> more;

                if(more == "n"){
                    break;
                }
            }

    		cin >> value;

    		ofile.open(tmp);
    		ofile << value;
    		ofile.close();

    		filesize = getFilesize(tmp);

    		//Open file
    		fd = open(tmp, O_RDONLY, 0);
    		assert(fd != -1);

    		//Execute mmap
    		map = (int*)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);

    		//Error Check mmap
    		if (map == MAP_FAILED) {
    			close(fd);
    			cout << "Error mmapping the file, please restart!\n";
    			return 0;
    		}

    		assert(map != NULL);

    		// Add to vector of Page addresses
    		pgs.push_back(map);

    		//Write the mmapped data to stdout (= FD #1)
    		cout << "Page Successfully mapped to: " << map << " !\n\n";
    		cout << "Value in Remote memory: \n"; 
    		write(1, map, filesize);
    		cout << endl;

    		close(fd);
    	}

    	// Case 2. Sync All pages
    	if(opt==2){
    		cout << "Starting Sync to Client..." << endl;
    		for(unsigned int i=0; i<pgs.size(); i++){
    			cout << "Syncing page " << i << " Located at: " << pgs[i] << endl;

    			//TODO Send all the Mother flipping pages to Client (just Addresses and Values)

                for(;;){
                    std::string path = "./tmp";

                    try{
                        boost::asio::io_service io_service;
                        async_tcp_client client(io_service, client_ip, path);
                        io_service.run();

                        std::cout << "send file " << argv[2] << " completed successfully.\n";
                    }
                    catch (std::exception& e){
                        std::cerr << e.what() << std::endl;
                    }

                    std::cout << "more? ";
                    std::cin >> more;

                    if(more == "n")
                        break;
                }


    		    // Unmap pages after send (rest in pepperonis)
    		    rip = munmap(map, filesize);
    		    if (rip != 0){
    		    	cout << "page # " << i << " at " << pgs[i] << " Failed to UNMAP!";
    		    }

    		    assert(rip == 0);
    		    close(fd);
    		}
    		cout << "\nSync Complete!" << endl;
    	}

	}
    cout << "Server process will now exit" << endl;
    return 0;
}
