#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <csignal>

using namespace std;

void* r_addr = (int*)0xaaaa;
int r_len = 0x4000;
char value[25];

void* __attribute__((weak)) mmap(void *addrs, size_t lengths, int prots, int flag, int fds){

	void* nu = 0;
	if(addrs == r_addr){
		cout << "Potential Page or Segmentation Fault detected!" << endl;
		cout << "Redirecting to Server Remote Memory...\n" << endl;
		raise(SIGINT);
	}

	else{
		cout << "Non conflicting memory detected, check mprotect()" << endl;
	}

	return nu;
}

size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

void r_mapHandler(int signum){

	cout << "Sending Value: " << value << " to be mapped into Remote Memory" << endl;

	// TODO Send to Value to server
	cout << "\n**SHIOULD NOW SEND TO SERVER (DEBUG)**" << endl;

}


int main(int argc, char** argv) {

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
    	cout << "\nAcquiring Clientside memory at: " << r_addr << " with length:" << hex << "0x" << r_len << endl;
    	cout << "Clientside memory is locked, redirecting mmap() to Remote Memory" << endl;
    }
    else
    	cout << "mprotect has failed, please restart!" << endl;


	while(opt != 0){

		cout << "\n*************Please Make a Selection**************" << endl;
		cout << "Enter 1 to map a value to remote memory\n";
		cout << "Enter 2 to request a sync of remote memory\n";
		cout << "Enter 0 to Exit!\n";
		cout << "**************************************************\n" << endl;

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

    		cout << "Calling mmap to page into memory..." << endl;

    		//Execute mmap (weak)
    		map = (int*)mmap(r_addr, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd);
    		if (map != 0)
    			cout << "mmap function failed, Please Restart!" << endl;

    		close(fd);

		}
		// Sync Time!
		if(opt == 2){
			cout << "Sync Request Sent to server..." << endl;
			// TODO Send Sync request to Server

			//wait
			cout << " \n**VALUES AND ADDRESSES FROM SERVER SHOULD BE HERE! (DEBUG)**\n" << endl;

		}
		// TODO The Sync output from Server should be outputted here
		if(opt == 3){

		}
	}
}
