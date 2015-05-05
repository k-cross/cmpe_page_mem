obj-m += RM_kMod.o
all: program1 program2
program1:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
program2: manager.c
	gcc -g manager.c -o manager
