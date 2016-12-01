obj-m += mkmodulo.o
obj-m += mkmodulo2.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) main.c -o lkm	
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm lkm
