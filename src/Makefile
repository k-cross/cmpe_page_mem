obj-m +=kern_tests.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) module_init
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) module
