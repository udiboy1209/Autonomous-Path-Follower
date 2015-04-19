Autonoumous Obstacle Detecting Path Follower
===========================================

**Embedded system project for CS101 course.**

Team members
-----------

|Name | Roll No.|
|:-----|---------|
| Meet Udeshi(TL) | 14D070007 |
| Siddhant Garg | 14D070027 |
| Prakirt Raj Jhunjhunwala | 140070027 |
| Nishant Dhama | 14D070064 |

Description
-----------

This project is aimed at creating a robot
which will be able to follow a path specified
by the user on an arena.

The path will be drawn on a GLUT canvas.
Path is basically a series of points interpolated with circular arcs.
Both drag and click features are provided to specify the points of the path.

#### Example of a path which can be drawn

<img alt="Sample path drawn on GLUT canvas" src="https://raw.githubusercontent.com/udiboy1209/Autonomous-Path-Follower/master/sample_path.png" width="300" height="auto" align="centre"/>

As you can see, the path formed is continuos and differentiable.
We use a specific algorithm for this interpolation.

Softwares Used
==============
Download & install the following softwares from the given links:-
 * [Atmel Studio 6.0](http://www.atmel.com/microsite/atmel_studio6/)
 * [OpenGL Utility Toolkit](https://www.opengl.org/resources/libraries/glut/)
 * [AVR Bootloader for Windows](http://www.nex-robotics.com/resources/avr-bootloader.html)
 * [X-CTU interface  for XBee](http://www.digi.com/support/kbase/kbaseresultdetl?id=2125)
 * [Code::Blocks](http://www.codeblocks.org/)
 * [Android Studio](http://developer.android.com/sdk/index.html)

Installation
================

Download this repository by running

    git clone http://www.github.com/udiboy1209/Autonomous-Path-Follower

Installing GLUT
---------------

Run this command

    sudo apt-get install freeglut3 freeglut3-dev

Then cd to the folder named `Autonomous-PathFollower/glut` and run

    make
    make clean
    ./gl

It will open the GLUT canvas

Configure X-CTU:
-------------------

Refer to the youtube Video: (https://www.youtube.com/watch?v=YAwuubSJAqc)

Tutorial videos
===============

[Robot Live Demonstration](https://www.youtube.com/watch?v=dD4qjS22m34&feature=youtu.be)

[Screencast Installation tutorial](https://www.youtube.com/watch?v=86wnkCGIsLE&feature=youtu.be)




