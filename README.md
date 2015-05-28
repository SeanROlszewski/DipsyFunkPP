# DipsyFunk #

A functional C++ DSP framework for the rapid prototyping of algorithms.

### Why DipsyFunk? ###

* Standardized, clean, and consolidated interface across all your DSP code. All of DipsyFunk's implementation is in a single header file.
* No subclassing or type hierarchies are necessary to represent DSP logic. Just code the difference equation and you're done!
* Signal-rate and control-rate parameters are easily passed into the DSP logic from GUI components or other subroutines. Access them through the provided interface out of the box.
* Type agnostic - take advantage of whatever bit-depth you choose as soon as you begin writing your DSP logic. Use any APIs custom numerical type as well.

### How do I get set up? ###

* Requires C++14 for use, and currently only compiles under `clang++`. Apple's Xcode Command Line Tools for 10.10.2 and above supports this. 
* The provided example requires PortAudio to be installed.
* Run `make` in the root directory to compile the example. 
* Run `./HelloDipsyFunk` to hear an aural treat.