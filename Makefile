CC = g++
CFLAGS = -std=c++11 -Wall -I.
LIBS = -lGL -lGLEW -lglfw -lm

SRCS = main.cpp window.cpp terrain.cpp input.cpp camera.cpp
HEADERS = window.h terrain.h input.h camera.h light.h
OBJS = $(SRCS:.cpp=.o)
TARGET = terrain_renderer

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)