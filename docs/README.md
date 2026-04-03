# Project Documentation

## About Project

The project simulates N-body gravity using different methods. It focuses on performance and experiments, meaning that without changing the code you can change implementation used.


## Philosophy of the project

Project is designed to be as fast as possible, leaving plenty of room for improvement. It is also designed to be modular, meaning that whole project divided on 3 parts and each part can have some different implementations, which can be used in executing the file. This gives perfect environment to look for different configurations since one can make very realistic simulation and change scenario without changing the code.


## How to run

To build you need to run following command
```
cmake -S . -B build -DDIMENSION=3
```

in which you can choose the value of dimension, then build project using this command
```
cmake --build build
```

and you can start the project using one of this two paths (depending on wether you use Linux (first path) or Windows)
```
./build/PhysicsSim                   
.\build\Debug\PhysicsSim.exe         
```

Optionally, you can enable sanitizers for debugging (cannot be used together):
```
cmake -S . -B build -DDIMENSION=3 -DENABLE_TSAN=ON
cmake -S . -B build -DDIMENSION=3 -DENABLE_ASAN=ON
```
TSAN detects data races between threads, ASAN detects memory errors (out-of-bounds, use-after-free).

## Modules

There are 3 main modules and 3 supporting. Supporting are math, physics and core. Main are graphics, space, universe.

### Math

This module implements all math used by the project. It has 2 classes Vector and Matrix which describe them in mathematical sense (Vector is fixed array here with same number of elements as dimension in the project and matrix is Vector of Vectors i.e. square matrix with length dimension). It also implements other stuff like sin, cos and some constants. It is designed to easy switch implementation of math used in the project.

### Physics

This module describe how the body looks like. It has struct body which memorizes all its properties such as name, state (position and velocity), mass, radius, active flag, trail (optimized using shared pointer), information about trail and color.

### Core

This module implements main usage functionality. There is configurator which gives class which you asked in config, there is timer which finds HUD, all constants and also aliases for all definition from other libraries, so it is easy to change implementation. It also implements pattern factory so classes can register them self and in result be seen in config.

### Graphics

This module implements the transition from array of objects to the actual picture on the screen.

### Space

This module describes laws of gravity. It has 2 main functions. One gives acceleration of specific body and other one gives topology (nowhere implemented yet).

### Universe

This module observes what is going on with objects and actually change them. Current implementation has steps ("step" is kind of modules used in universe module) for moving object (rk4, euler), dealing with collision (merging) and saving trail (trail).

## Usage

Whole file is configured using config.json in the main directory. If you leave it empty, code will take default values for everything. Here is explanation of each entry:

```"batchSize"```: It is optimization for large runs. It means, how many ticks should be gone to update the frame value. 
Default value is 100


### Window

```"width"```: width of the window. If you choose 0 it takes width of the monitor.
Default value is width of the window.

```"height"```: height of the window. If you choose 0 it takes height of the monitor.
Default value is height of the window.


### Universe

```"type"```: which implementation of the universe you choose. There is standard universe (performs rk4 move and trail step) and pipeline (where you can choose pipeline by yourself) and by default standard is chose.

```"dt"```: how much time is simulated in 1 tick.
Default value is 0.001.

```"trailFreq"```: how often to add a trail.
Default value is 1.

```"pipeline"```: array of steps performed by universe (only for pipeline universe). There are following steps: "rk4", "euler", "merging", "trail".
By default it is empty.

### Space

```"type"```: which implementation of physics you choose. There is newtonian and relativistic (Einstein-Infeld-Hoffmann). The set physics laws.
By default newtonian is choosed

```"G"```: Gravitation constant.
Default value is 1.0.

```"c"```: Speed of light
Default value is 100.

```"softening"```: Softening parameter to prevent zero division when small distance.
Default value is 0.01.


### Graphics

```"type"```: which implementation of graphics you choose. There is only circle now. 
Default implementation is circle.

```"focalLength"```: focal length for 3D to 2D projection.
Default value is 5.

```"cameraTarget"```: you can choose to set the object so you see how everyone move relative to it (optional to choose).
Default target is non.


### Bodies

```"bodies"```: Array of objects.
By default empty.

```"name"```: Name of an object.
Default name is "objectN{}" where you substitute "{}" with index of the object.

```"pos"```: Position of an object.
Default value is 0 in all coordinates.

```"vel"```: Velocity of an object.
Default value is 0 in all coordinates.

```"mass"```: Mass of an object.
Default value is 1.

```"radius"```: Radius of an object.
Default value is 0.05.

```"active"```: Is object active (or is it alive in other words).
Default value is true.

```"trail_enabled"```: True if we are showing trail. False otherwise.
Default value is false.

```"max_trail_size"```: Maximum number of trails for an object.
Default value is 100.

```"red"```: Specify the color of the object from 0 to 1.
Default value is 0.

```"green"```: Specify the color of the object from 0 to 1.
Default value is 0.

```"blue"```: Specify the color of the object from 0 to 1.
Default value is 0.


## Keys usage

There are 2 keys combination you can use in the program. F3 shows HUD in the name of app and terminal. So it will show FPS (frames per second) and TPS (ticks per second).

You can also use digits from 1 to the value of dimension (up to 9) to rotate in the simulation. It is implemented using axis simulation, meaning each number has own axis where 1 may be X, 2 - Y and if press both at the same time simulation is rotated by XY axis. If you also press shift with axis it is rotated in another dimension. For now you can press only two numbers at a time.