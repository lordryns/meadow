OUTPUT = meadow 
CFLAGS = -Wextra -Wall -Wpedantic
LIBS = -lX11
TARGET = meadow.c utils.c config.c

build: $(TARGET)
	$(CC) -o $(OUTPUT) $(CFLAGS) $(TARGET) $(LIBS)

run:
	xinit ./xinitrc -- /usr/bin/Xephyr :100 -ac -screen 1100x700 -host-cursor

clean:
	rm -rf $(OUTPUT)

