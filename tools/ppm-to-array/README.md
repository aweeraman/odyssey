# Converts a PPM file to a C-header

This is a simple tool that converts an image file encoded in in the PPM
format to a C-header with an array containing the image data.

At the time of writing, Odyssey doesn't not have a file system and so
one of the mechanisms to "compile" an image file into the the OS was
to use this tool to generate a C-header that is included with Odyssey,
and used by the command shell to read the image data array and write to
a framebuffer for displaying.

This tool requires the 'netbpm' package to convert an image to PPM.

```
$ sudo apt-get install netbpm
$ make
$ ./jpeg-to-c.sh image.jpeg image.h
```
