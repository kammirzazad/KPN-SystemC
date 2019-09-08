# kpn_model_systemc
This repository provides an implementation of Kahn processes [1] in SystemC [2] and two examples that utilize this implementation to model applications as KPN. 

# Design
The implementation can be found in *kahn_process.h* which defines the abstract SystemC module with same name. Following the SystemC semantics, this class derives from *sc_module* and defines a thread named *main*. The main thread simply calls pure virtual function of *process()* in a while loop whilst keeping track of the current iteration. 

In SystemC, modules need to communicate with each other through *sc_ports* whose interface is defined by classes derived from *sc_interface* and are connected using classes derived from *sc_channel*. However, to provide a generic implementation, base class does not define any interface or port and it is up to the application developer to define them. *kpn_fifo* folder provides an example of how to define ports with *sc_fifo* interface for producer and consumer processes. Once ports are defined, data can be sent on *out_ports* using *write()* method and received from *in_ports* using *read()* method.

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

# Modeling neural networks
*kpn_neuralnet* folder provides an example of how to model a layer-wise pipelined neural network using our implementation. This example defines placeholder *image_reader*, *image_writer*, *conv_layer*, *max_layer* and *detection_layer* classes that are derived from *kahn_process*. This classes are interfaced in top-level module of *kpn_neuralnet* using *sc_fifos* to build a neural network with two convolutional and one maxpool layer.

# References
[1] Gilles, K. A. H. N. "The semantics of a simple language for parallel programming." Information processing 74 (1974): 471-475.

[2] IEEE Standard SystemC Reference Manual (IEEE 16666-2011), https://standards.ieee.org/standard/1666-2011.html

# Contact
If you have a question or suggestion about this repository or a problem running examples, you can contact Kamyar at kammirzazad@utexas.edu.
