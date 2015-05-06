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
#include <sys/ioctl.h>
#include "../tcp_api.cpp"

/* Define ioctl commands */
#define IOCTL_PATCH_TABLE 0x00000001
#define IOCTL_FIX_TABLE   0x00000004

using namespace std;

// vars
void* r_addr = (void*)0xaaaaa000;
void* r_addr_end = (void*)0xaaaaa000;
int r_len = 0x4000;
char value[25];

size_t getFilesize(const char* filename) {
	struct stat st;
	stat(filename, &st);
	return st.st_size;
}

int main(int argc, char* argv[]) {
    const string ip_addr = argv[1];
	const char* tmp = "tmp.txt";
	ofstream ofile;
	ifstream ifile;
	int opt;
	char ch;
	bool first_run = true;
	int* map;
	size_t filesize;
	int fd;

	do{
		cout << "\n*************Please Make a Selection**************" << endl;
		cout << "Enter 1 to map a value to remote memory\n";
		cout << "Enter 2 to request a sync of remote memory\n";
		cout << "DEBUGEnter 3 to connect to the interceptor device\n";
		cout << "Enter 0 to Exit!\n";
		cout << "**************************************************\n" << endl;

		cin >> opt;

		// OPTION 1: Manually map memory
		if(opt == 1){
			if(first_run){
				cout << "Would you like to specify a start and end address for remote memory? (Y/n)" << endl;
				cin >> ch;
				if(ch == 'Y' || ch == 'y'){
					cout << "\nPlease enter a start address (Omitting the '0x')" << endl;
					cin >> r_addr;

					cout << "\nPlease enter an end address (Omitting the '0x')" << endl;
					cin >> r_addr_end;

					cout << "\nOk! Using values: "<< hex << r_addr << " through " << hex << r_addr_end << " for Remote Memory" << endl;
				}
				else
					cout << "\nUsing default values: " << hex << "0x" << r_addr << " - " << hex << "0x" << r_addr_end << endl;
				first_run = false;
			}
			// Starting Manual memory mapping procedure
			cout << "\nPlease enter a value to be mapped into shared memory...\n";
			cin >> value;
			ofile.open(tmp);
			ofile << value;
			ofile.close();
			filesize = getFilesize(tmp);

			// Kernel Module interface
			//TODO: EDIT TO ALL IN r_addr and r_addr_end
			int device = open("/dev/interceptor", O_RDWR); // Open Character driver created by the kernel module
			if(device > -1)
				printf("Character device successfully opened! Device Number: %i\n",device);
			else{
				printf("Character device failed to open! Device Number: %i\n",device);
				return 0;
			}
			ioctl(device, IOCTL_PATCH_TABLE, r_addr, r_addr_end); // tell the cDriver (kMod) to apply sys_call_table patch

			//Open file
			fd = open(tmp, O_RDONLY, 0);
			assert(fd != -1);

			// call mmap with the r_memory address
			cout << "Calling mmap to page into Remote memory..." << endl;
			map = (int*)mmap(r_addr, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);

			if(map == 0) cout << "SUCCESS! : memory was mapped remotely (Special RM_kMod return code)" << endl;
			//else if (map == MAP_FAILED)cout << "ERROR: map Failed" << endl;
			else if(map > 0) cout << "ERROR: mmap Succeeded! MEMORY WAS WRITTEN LOCALLY" << endl;
			close(fd);

			// Sleep for a bit then revert the table
			sleep(2);

			// Send Value to Server (message first then value)! OFSREAM ofile --> value
			ofile.open("message");
			ofile << "mem" << endl;
			ofile.close();

			file_send(ip_addr, "message");
			sleep(2);

			// Send value
			ofile.open("value");
			ofile << value << endl;
			file_send(ip_addr, "value");


			ioctl(device, IOCTL_FIX_TABLE); // tell the cDriver (kMod) to remove sys_call_table patch
			close(device);

		}
		// Sync Time!
		if(opt == 2){
			cout << "Sending Sync Request to Server..." << endl;
			ofile.open("message");
			ofile << "sync" << endl;
			ofile.close();
			file_send(ip_addr, "message");

			// Wait for response from server
			file_recieve();
			ifile.open("value_out");

			while(ifile.good()){
				ifile.getline(value, 25);
				cout << "Received from Server: " << value << endl;
			}
			ifile.close();
			cout << "Sync Complete" << endl;
		}

		// Connect to interceptor device, attempt mmap and close
		if(opt == 3){


		}
	}while(opt != 0 && opt >= 1 && opt <= 2);

	cout << "RM_Client will now exit..." << endl;
	return 0;
}
