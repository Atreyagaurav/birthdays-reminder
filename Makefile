CC = gcc
CFLAGS = -Wall
DEPS = convertADBS.h
OBJ = bdays.o convertADBS.o convert.o test.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -DBIRTHDAYS_FILE=\"$(realpath birthdays.txt)\" -c -o $@ $<

all: birthdays adbs-convert

birthdays: bdays.o convertADBS.o
	gcc $(CFLAGS) -o $@ $^

adbs-convert: convert.o convertADBS.o
	gcc $(CFLAGS) -o $@ $^

test: test.o convertADBS.o
	gcc $(CFLAGS) -o $@ $^

clean:
	rm *.o birthdays adbs-convert
