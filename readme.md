LIBPS2
======

LIBPS2 is a library for interfacing AVR microcontrollers with a PS/2 keyboard.

###Supported Features

* Device-to-host communications:
	* Full support for basic and extended scan-codes
	* Configurable scan-code buffering

###Features NOT Supported

* No support for host-to-device communications

Table of Contents
===============

1. [Changelog](https://github.com/majestic53/libps2#changelog)
2. [Usage](https://github.com/majestic53/libps2#usage)
	* [Build](https://github.com/majestic53/libps2#build)
	* [Concept](https://github.com/majestic53/libps2#concepts)
	* [Examples](https://github.com/majestic53/libps2#examples)
3. [License](https://github.com/majestic53/libps2#license)

Changelog
=========

###Version 0.1.1510
*Updated: 3/7/2015*

* Repo creation

Usage
=====

###Build

**NOTE:** You will need to install avr-gcc if you don't already have it.

To use LIBPS2 in your own project, simply include the header file ```libps2.h``` and c file ```libps2.c```, found under ```./src/lib/include``` 
and ```./src/lib/src```, in your project.

To build the sample project, compile using the accompanying makefile. The makefile will produce a series of 
object files (found in the ```./build``` directory), and a series of binary files (found in the ```./bin``` directory).

###Concept

TBD

###Examples

TBD

License
======

Copyright(C) 2015 David Jolly <majestic53@gmail.com>

libps2 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libps2 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
