# KPN_SystemC
This repository provides an implementation of Kahn processes [1] in SystemC [2] and two examples that utilize this implementation to model applications as KPN. 

# Building and running
To build *kpn_fifo*, first make sure that *SYSTEMC_HOME* variable is set to the installation folder of SystemC, e.g.
```
export SYSTEMC_HOME=/usr/local/packages/systemc-2.3.1
```
then run:
```
cd kpn_fifo/
make
```
This will generate *kpn_fifo.x* that can be run directly. Likewise, similar procedure can be used to build and run *kpn_neuralnet*.

# Content
* **kpn_fifo/**: An example of modeling producer-consumer pair as KPN in SystemC
* **kpn_neuralnet/**: An example of modeling three layer neural network as KPN in SystemC
* **LICENSE**
* **Makefile.all**: Base helper makefile provided by SystemC, used by examples (see its header for license)
* **Makefile.config**: Same as Makefile.all
* **Makefile.rules**: Same as Makefile.all
* **README.md**: This README
* **kahn_process.h**: Base class for implementing Kahn processes in SystemC
* **tutorial.md**: A short tutorial on SystemC modeling and its semantics

# References
[1] Gilles, K. A. H. N. "The semantics of a simple language for parallel programming." Information processing 74 (1974): 471-475.  
[2] IEEE Standard SystemC Reference Manual (IEEE 16666-2011), https://standards.ieee.org/standard/1666-2011.html

# Contact
If you have a question or suggestion about this repository or a problem running examples, you can contact Kamyar at kammirzazad@utexas.edu.
