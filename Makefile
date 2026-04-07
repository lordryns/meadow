OUTPUT = meadow 
CFLAGS = -Wextra -Wall -Wpedantic
LIBS = -lX11
TARGET = meadow.c

build: $(TARGET)
	$(CC) -o $(OUTPUT) $(CFLAGS) $(TARGET) $(LIBS)

run:
	xinit ./xinitrc -- /usr/bin/Xephyr :100 -ac -screen 1380x720 -host-cursor

clean:
	rm -rf $(OUTPUT)

