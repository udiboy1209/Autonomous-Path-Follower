Autonoumous Obstacle Detecting Path Follower
===========================================

**Embedded system project for CS101 course.**

Team members
-----------

|Name | Roll No.|
|:-----|---------:|
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

![Sample path drawn on GLUT canvas](https://raw.githubusercontent.com/udiboy1209/Autonomous-Path-Follower/master/sample_path.png "Example of a path which can be drawn")

As you can see, the path formed is continuos and differentiable.
We use a specific algorithm for this interpolation.

