CC = g++
CFLAGS = -std=c++11 -Wall
LIBS = -lGL -lGLEW -lglfw

TARGET = terrain_renderer
SRCS = main.cpp

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

clean:
	rm -f $(TARGET)