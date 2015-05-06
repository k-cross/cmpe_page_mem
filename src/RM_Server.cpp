//============================================================================
// Name        : CMPE_142_RM_Server.cpp
// Author      : Mason
// Version     :
// Copyright   : 
// Description :
//============================================================================

#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

int main(int argc, char** argv) {

	char value[25];
	const char* tmp = "tmp.txt"; // temp file for FD
	ofstream ofile;
	int opt = 3;

    size_t filesize;
    int fd;
    int rip;
    int* map;
    //void* range = (void*)0xaaaaa000;

    vector<int*> pgs;

    cout << "Server process Initialized!\n";

    while (opt!=0){

        cout << "***SAMPLE DIALOG (Working ver will not have) 1 to map a page\n 2 to SYNC all pgs. 0 to exit" << endl;
        cin >> opt;

        // Case 1. Map a Page
    	if (opt == 1){
    		cout << "Remote Page In of Value:" << value << " requested.\n";
    		cout << "Mapping page into memory...\n";

    		// TODO: Get Value from Client and put it in value
    		// NOTE to Kenny, you may have to change opt and create
    		// a loop here to start case 1 and case 2 based off of messages from Client

    		cout << "***SAMPLE DIALOGEnter a value" << endl;
    		cin >> value;

    		ofile.open(tmp);
    		ofile << value;
    		ofile.close();

    		filesize = getFilesize(tmp);

    		//Open file
    		fd = open(tmp, O_RDONLY, 0);
    		assert(fd != -1);


    		//cout << "Range is: " << range << endl;
    		//Execute mmap
    		map = (int*)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    		//range = (char*)range + filesize;

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
    		cout << "Page Successfully mapped to: " << map << " !\n" << endl;
    		cout << "Value in Remote memory: " << endl;
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
