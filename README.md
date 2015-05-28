# DipsyFunk #

A functional C++ DSP framework for the rapid prototyping of algorithms.

### Why DipsyFunk? ###

* Standardized, clean, and consolidated interface across all DSP code.
* No subclassing or type hierarchies are necessary to represent DSP logic.
* Signal-rate and control-rate parameters are easily passed into the DSP logic from GUI components or other subroutines.
* Type agnostic - take advantage of whatever bit-depth you choose as soon as you begin writing your DSP logic.

### How do I get set up? ###

* Requires C++14 for use, and currently only compiles under `clang++`. Apple's Xcode Command Line Tools for 10.10.2 and above supports this. 
* The provided example requires PortAudio to be installed.
* Run `make` in the root directory to compile the example. 
* Run `./HelloDipsyFunk` to hear an aural treat.