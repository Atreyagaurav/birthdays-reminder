CC = gcc
CFLAGS = -Wall
DEPS = convertADBS.h
OBJ = bdays.o convertADBS.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -DBIRTHDAYS_FILE=\"$(realpath birthdays.txt)\" -c -o $@ $<

birthdays: $(OBJ)
	gcc $(CFLAGS) -o $@ $^

clean:
	rm *.o birthdays
