obj-m += hello.o
obj-m += netlinkKernel.o
all: brendan_mod kenny ziyi_mod ziyi_user full

brendan_mod:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test: all
	sudo insmod ./hello.ko
	sudo rmmod hello
	dmesg | tail

kenny: 
	g++ -g -c testc.cpp -lpthread -lboost_system -o testc.o

ziyi_mod:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

ziyi_user:
	gcc -g -c netlinkUser.c -o user.o

full:
	g++ -g user.o testc.o -o testc
