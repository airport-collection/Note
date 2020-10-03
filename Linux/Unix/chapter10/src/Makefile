CC = gcc
CFLAGS += -Wall

%: %.c ../../lib/error_functions.o ../../lib/get_num.o
	$(CC) $(CFLAGS) $^ -o ../out/$@

clean:
	rm -rf ../out/*