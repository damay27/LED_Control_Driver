KERNELDIR = /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
NAME = led_ctrl

obj-m := $(NAME).o
$(NAME)-objs := pcie_ctrl.o $(NAME)_main.o file_ops.o

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm $(NAME).ko .$(NAME).ko.cmd $(NAME).mod $(NAME).mod.c \
		 .$(NAME).mod.cmd $(NAME).mod.o .$(NAME).mod.o.cmd \
		 $(NAME).o .$(NAME).o.cmd Module.symvers modules.order \
		 file_ops.o pcie_ctrl.o $(NAME)_main.o $(NAME).mod \
		 .file_ops.o.cmd .pcie_ctrl.o.cmd .$(NAME)_main.o.cmd \