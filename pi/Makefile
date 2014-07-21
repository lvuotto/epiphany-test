
ESDK = $(EPIPHANY_HOME)
ELIBS = $(ESDK)/tools/host/lib
EINCS = $(ESDK)/tools/host/include
ELDF = $(ESDK)/bsps/current/internal.ldf
EXES = main e_pi.srec
OBJS = e_pi.elf

CC = gcc
ECC = e-gcc
EOC = e-objcopy
CFLAGS = -Wall -Wextra -Werror -Wl,-rpath,$(ELIBS)
EFLAGS = -funroll-loops -ffast-math
EOFLAGS = --srec-forceS3 --output-target srec

all: $(EXES)

main: pi.c shared.h
	$(CC) $(CFLAGS) -I$(EINCS) -L$(ELIBS) -o $@ $^ -le-hal -lrt

e_pi.elf: e_pi.c shared.h
	$(ECC) $(CFLAGS) $(EFLAGS) -O3 -T $(ELDF) -o $@ $^ -le-lib

e_pi.srec: e_pi.elf
	$(EOC) $(EOFLAGS) $^ $@


clean:
	rm -f $(EXES) $(OBJS)

