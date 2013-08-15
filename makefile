FLAGS = -Wall -std=c99

OFILES = md5.o aes.o interface.o integers.o

all: interface tidy

interface: $(OFILES)
	gcc $(FLAGS) $(OFILES) -o cryptato

interface.o: interface.c
	gcc $(FLAGS) -c interface.c

md5.o: md5.c
	gcc $(FLAGS) -c md5.c

aes.o: aes.c
	gcc $(FLAGS) -c aes.c

integers.o: integers.c
	gcc $(FLAGS) -c integers.c

tidy:
	rm -f $(OFILES)
