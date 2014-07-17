
ESDK = $(EPIPHANY_HOME)
ELIBS = $(ESDK)/tools/host/lib
EINCS = $(ESDK)/tools/host/include
ELDF = $(ESDK)/bsps/current/internal.ldf
EXES = main epiphany.srec
OBJS = epiphany.elf

CC = gcc
ECC = e-gcc
EOC = e-objcopy
CFLAGS = -Wall -Wextra -pedantic -Werror
EFLAGS = -funroll-loops -ffast-math
EOFLAGS = --srec-forceS3 --output-target srec

all: $(EXES)


main: host.c shared.h
	$(CC) $(CFLAGS) -I$(EINCS) -L$(ELIBS) -le-hal -lrt -o $@ $^

epiphany.elf: epiphany.c shared.h
	$(ECC) $(CFLAGS) $(EFLAGS) -O3 -T $(ELDF) -le-lib -o $@ $^

epiphany.srec: epiphany.elf
	$(EOC) $(EOFLAGS) $^ $@


clean:
	rm $(EXES) $(OBJS)

