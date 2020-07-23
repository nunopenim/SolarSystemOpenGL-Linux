# Copyright (C) 2020 Nuno Penim
# 
# Makefile to be used in the SolarSystemOpenGL project
# Can be easily modified for other projects.
#
# Like everything in this project, Licensed under DBAD Public License

CC = g++

GLFLAGS = -lGL -lGLU -lGLEW -lglfw

OBJ1 = main.cpp

OBJ1_EXEC = solarSystem

all:
	$(CC) $(OBJ1) -o $(OBJ1_EXEC) $(GLFLAGS)
	./solarSystem

build:
	$(CC) $(OBJ1) -o $(OBJ1_EXEC) $(GLFLAGS)

clean:
	rm -rf solarSystem

