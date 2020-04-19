CC = gcc
CFLAGS = -Wall
DEPS = convertADBS.h
OBJ = bdays.o convertADBS.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

birthdays: $(OBJ)
	gcc $(CFLAGS) -o $@ $^
