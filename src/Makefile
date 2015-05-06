obj-m += RM_kMod.o
all: client kern_mod server

kern_mod:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

client: 
	g++ -g RM_Client.cpp -o client

server:
	g++ -g RM_Server.cpp -o server
