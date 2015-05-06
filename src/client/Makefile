obj-m += hello.o
obj-m += netlinkKernel.o
all: brendan_mod kenny ziyi_mod ziyi_user

brendan_mod:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test: all
	sudo insmod ./hello.ko
	sudo rmmod hello
	dmesg | tail

kenny: final_cl.cpp
	g++ -g final_cl.cpp -lpthread -lboost_system -o client

ziyi_mod:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

ziyi_user:
	gcc -g netlinkUser.c -o user
