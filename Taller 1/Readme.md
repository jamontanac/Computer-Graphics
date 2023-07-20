# Instructions
----------
## Objective

Implement a system to appreciate the generation of random colors in the area defined by an image.

## Description

Implement an algorithm (in your preferred programming language) that receives the following parameters via the command line:

output_image_name.ppm w h percentage2 percentage3 percentage4 ... percentageN

Where:
- output_image_name.ppm is the name of an image file in PPM format [more info](http://netpbm.sourceforge.net/doc/ppm.html) with the result of the algorithm.
- w is the width, in pixels, of the result.
- h is the height, in pixels, of the result.
- percentageX are the relative sizes of a window with respect to the previous size.

The idea is to generate an image with N windows of different colors. The first window is an image of size "width" X "height", the second is a window of "width * percentage2" X "height * percentage2", the third is a window of "width * percentage2 * percentage3" X "height * percentage2 * percentage3", and so on. All windows are centered at the pixel [0,0].

The colors to use should be random and follow a normal distribution, centered in the middle of the color spectrum with standard deviations of 100 for the first window, 100 * percentage2 for the second, 100 * percentage2 * percentage3 for the third, and so on.

You can only use the base I/O library of the chosen language that allows reading and writing text files.

## Deliverables
A ZIP file that contains:

- Source code,
- User manual.

## Delivery Deadline
Before the second session of the course.

## Expected Outcome
- Gauge the computational complexity of having simple 2D graphic primitives.
- Evaluate students' problem-solving skills.