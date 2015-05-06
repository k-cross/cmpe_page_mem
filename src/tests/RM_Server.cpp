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
#include <string>
#include "../tcp_api.cpp"

using namespace std;

size_t getFilesize(const char* filename) {
	struct stat st;
	stat(filename, &st);
	return st.st_size;
}

int main (void){

	string temp_m, temp_v;
	int i = 1;
	ifstream message;
	ifstream value;
	ofstream value_out;
	const char* tmp = "tmp.txt"; // temp file for FD
	ofstream ofile;
	size_t filesize;
	int fd;
	int rip;
	int* map;
	//void* range = (void*)0xaaaaa000;
	vector<int*> pgs;
	const string ip_addr = "10.0.0.2"; // Client's IP

	//
	// Master Server loop
	// IFSTREAM message -> temp_m
	// IFSTREAM value	-> temp_v
	// IFSTREAM value_out
	while (1 == 1){

		file_recieve();
		message.open("message");
		message >> temp_m;
		message.close();

		if (temp_m == "mem"){

			file_recieve();
			value.open("value");
			value >> temp_v;
			value.close();

			//Start MMapping temp_v
			ofile.open(tmp);
			ofile << temp_v;
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
			// End MMapping temp_v


		}
		else if (temp_m == "sync"){

			value_out.open("value_out");

			cout << "Starting Sync to Client..." << endl;
			for(unsigned int i=0; i<pgs.size(); i++){

				cout << "Syncing page " << i << " Located at: " << pgs[i] << endl;
				value_out << "Address: " << hex << pgs[i] << " Value: " << pgs[i] << endl;

				// Unmap pages after send
				rip = munmap(map, filesize);
				if (rip != 0){
					cout << "page # " << i << " at " << pgs[i] << " Failed to UNMAP!";
				}
				assert(rip == 0);
				close(fd);
			}
			value_out.close();
			//Send all the  pages to Client (just Addresses and Values)
			file_send(ip_addr, "value_out");

			cout << "\nSync Complete!" << endl;
		}


	}
	// Main loop Exited
	cout << "server will now exit..." << endl;
	return 0;
}
