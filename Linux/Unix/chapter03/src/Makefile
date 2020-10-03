CC = gcc
CFLAGS += -Wall

%: %.c
	$(CC) $(CFLAGS) $^ -o ../out/$@

clean:
	rm -rf ../out/*