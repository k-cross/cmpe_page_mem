obj-m += hello.o
all: program1 program2
program1:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test: all
	sudo insmod ./hello.ko
	sudo rmmod hello
	dmesg | tail
program2: final_cl.cpp
	g++ -g final_cl.cpp -lpthread -lboost_system -o client
