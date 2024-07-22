CC = g++
CFLAGS = -std=c++11 -Wall -I.
LIBS = -lGL -lGLEW -lglfw

SRCS = main.cpp window.cpp input.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = terrain_renderer

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)