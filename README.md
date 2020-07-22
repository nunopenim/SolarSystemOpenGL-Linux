# SolarSystemOpenGL

Nuno Penim, 2020

Graphical Computing Final project.

Planets and sun are not to real scale or distances. Rendering distance would need to be huge for that. Currently They are on Multiples of the actual distance.

OpenGL and C++

## How to render

The following guide is for Ubuntu 18.04 (possibly 20.04) and it's variants (Xubuntu, Lubuntu, etc...)

First make sure that you have OpenGL, GLEW, GLFW3, GLM installed. To install them, run the following command:

`sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev libglfw3 libglfw3-dev libglew-dev libglm-dev libsdl2-dev binutils-gold g++ mesa-common-dev build-essential libglew1.5-dev libglm-dev `

Secondly, compile! Soon I will add a Makefile and do a general restructure. But for now, to do such, on the top of the directory, type:

`g++ main.cpp -o solarSystem -lglfw -lGLEW -lGLU -lGL`

This will create an executable named solarSystem in the main directory. The final step, if no errors were generated, is to type:

`./solarSystem`

Now, wait, as depending on your machine it can take a bit to open. Use WASD to move, the mouse to look around and shift key to move faster. The camera is above the sun, so if you only see stars, look down first to see the sun, then you can locate the planets! Have fun :)

## Notes for self

Missing Makefile!
