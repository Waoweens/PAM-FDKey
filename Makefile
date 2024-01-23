BUILD_DIR=build
OBJECTS=$(BUILD_DIR)/pam_fdkey.o
CFLAGS=-fPIC -Wall `pkg-config --cflags dbus-1`
LDFLAGS=-shared `pkg-config --libs dbus-1`
TARGET=$(BUILD_DIR)/pam_fdkey.so
PAM_TARGET=/lib/security

all: clean $(OBJECTS)
	gcc $(LDFLAGS) -o $(TARGET) $(OBJECTS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(BUILD_DIR)
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

install:
	install -m 0755 $(TARGET) $(PAM_TARGET)/
