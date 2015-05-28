# DipsyFunk #

A functional C++ DSP framework for the rapid prototyping of algorithms.

### Why DipsyFunk? ###

DipsyFunk is a DSP framework written in C++ that allows a programmer to implement DSP functions as a series of lambda expressions that are given to DispyFunk to compute. This frees a programmer from having to write any OO code to manage their DSP logic, and let's someone immediately begin developing DSP algorithms.

In addition to its convienience, DipsyFunk also has:

* A standardized, clean, and consolidated interface. All of DipsyFunk's implementation is in a single header file.
* No subclassing or type hierarchies. Simply use the `CallbackController` and `std::function` classes to develop your algorithms and code. Just code the difference equation and you're done!
* Signal-rate and control-rate parameters are easily passed into the DSP logic from GUI components or other subroutines. Access them through the provided interface out of the box.
* Type agnostic - take advantage of whatever bit-depth you choose as soon as you begin writing your DSP logic. Use any APIs custom numerical type as well.

### How do I get set up? ###

* Requires C++11/14 for use, and currently only compiles under `clang++`. Apple's Xcode Command Line Tools for 10.10.2 and above supports this. 
* The provided example requires PortAudio to be installed.
* Run `make` in the root directory to compile the example. 
* Run `./HelloDipsyFunk` to hear an aural treat.