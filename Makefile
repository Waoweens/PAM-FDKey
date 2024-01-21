BUILD_DIR=build
OBJECTS=$(BUILD_DIR)/pam_fdkey.o
CFLAGS=-fPIC -Wall
LDFLAGS=-shared
TARGET=$(BUILD_DIR)/pam_fdkey.so

all: clean $(OBJECTS)
	gcc $(LDFLAGS) -o $(TARGET) $(OBJECTS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(BUILD_DIR)
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

install:
	install -m 0755 $(TARGET) /lib/security/
