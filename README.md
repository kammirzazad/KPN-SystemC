# kpn_model_systemc
This repository provides an implementation of Kahn processes [1] in SystemC [2] and two examples that utilize this implementation to model applications as KPN. 

# Design
The implementation can be found in *kahn_process.h* which defines the abstract SystemC module with the same name. Following the SystemC semantics, the kahn_process is a module that inherits from the *sc_module* base class and registers itself with the SystemC kernel using *SC_HAS_PROCESS* macro. In SystemC, each module can have one or more threads that are executed concurrently. kahn_process has a single thread named *main* that is registered with SystemC using *SC_THREAD* macro. The main thread simply calls pure virtual function of *process()* in a while loop whilst keeping track of the current iteration. 

In SystemC, modules need to communicate with each other through *sc_ports* whose interface is defined by classes derived from *sc_interface* and are connected using classes derived from *sc_channel*. However, to provide a generic implementation, base class does not define any interface or port and it is up to the application developer to define them. 

*kpn_fifo* folder provides an example of how to define ports with *sc_fifo* interface for producer and consumer processes. It defines output port for producer process as *sc_fifo_out<char>* which is a shortcut for *sc_port< sc_fifo_out_if<char> >*. In general case, one could replace *sc_fifo_out_if<char>* with any interface that inherits from *sc_interface*. Input port for consumer is defined similarly. Furthermore, multiple ports can simply be defined as arrays of *sc_port*. Once ports are defined, data can be sent on output ports using *write()* method and received from input ports using *read()* method.
  
To build a process network that consists of multiple processes, one needs to instantiate processes and channels that implement the input/output interface of its consumer/producer in a top-level module. Consequently, ports of processes need to be initialized with channels. For example, in *kpn_fifo*, producer and consumer are connected using channel of type *sc_fifo<char>* named *producer_to_consumer* in top-level module of *kpn_info* where their ports are initialized with *producer_to_consumer*.
  
Finally, the top-level module in SystemC can be simulated by instantiating it in *sc_main()* and calling global method of *sc_start()*.
  

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
