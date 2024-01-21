OBJECTS=pam_fdkey.o
CFLAGS=-fPIC -Wall
LDFLAGS=-shared
TARGET=build/pam_fdkey.so

all: clean $(OBJECTS)
	gcc $(LDFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	gcc $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)

install:
	install -m 0755 $(TARGET) /lib/security/