
ESDK = $(EPIPHANY_HOME)
ELIBS = $(ESDK)/tools/host/lib
EINCS = $(ESDK)/tools/host/include
ELDF = $(ESDK)/bsps/current/internal.ldf
EXES = main epiphany.srec
OBJS = epiphany.elf

CC = gcc
ECC = e-gcc
EOC = e-objcopy
CFLAGS = -Wall -Wextra -Werror -Wl,-rpath,$(ELIBS)
EFLAGS = -funroll-loops -ffast-math
EOFLAGS = --srec-forceS3 --output-target srec

all: $(EXES)

main: host.c shared.h
	$(CC) $(CFLAGS) -I$(EINCS) -L$(ELIBS) -DSEED=176 -o $@ $^ -le-hal -lrt

epiphany.elf: epiphany.c shared.h
	$(ECC) $(CFLAGS) $(EFLAGS) -O3 -T $(ELDF) -o $@ $^ -le-lib

epiphany.srec: epiphany.elf
	$(EOC) $(EOFLAGS) $^ $@


clean:
	rm -f $(EXES) $(OBJS)

