# This makefile is used to compile the program Ball_dynamics.cpp
# compiler
CC = g++

# compiler flags
CFLAGS = -std=c++17 -framework OpenGL -framework GLUT

# output
OUTPUT = ball_dynamics

# source files
SOURCES = Ball_Dynamics.cpp

all: ball_program

# Compile the program
ball_program: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(OUTPUT) 

# Clean target to remove object files and compiled program
clean:
	rm -f $(OUTPUT)