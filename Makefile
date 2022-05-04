CC = gcc
CFLAGS = -Wall
DEPS = convertADBS.h
OBJ = bdays.o convertADBS.o convert.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -DBIRTHDAYS_FILE=\"$(realpath birthdays.txt)\" -c -o $@ $<

all: birthdays convert

birthdays: bdays.o convertADBS.o
	gcc $(CFLAGS) -o $@ $^

convert: convert.o convertADBS.o
	gcc $(CFLAGS) -o $@ $^

clean:
	rm *.o birthdays convert
