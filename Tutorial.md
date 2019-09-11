# SystemC Modules and Threads
The implementation can be found in [kahn_process.h](https://github.com/kammirzazad/KPN-SystemC/blob/master/kahn_process.h) which defines the abstract SystemC module with the same name. Each SystemC program consists of a collection of modules. Following these semantics, the *kahn_process* is a module that inherits from the *sc_module* base class and registers itself with the SystemC kernel using *SC_HAS_PROCESS* macro. 
```
SC_HAS_PROCESS(kahn_process);
```
In SystemC, each module can have one or more threads that are executed concurrently. *kahn_process* has a single thread named *main* that is registered with SystemC using *SC_THREAD* macro. 
```
SC_THREAD(main);
```
The main thread simply calls pure virtual function of *process()* in a while loop whilst keeping track of the current iteration. 
```
void	main() { while(true) {process(); iter++;} }
```

# Ports and Interfaces
In SystemC, modules need to communicate with each other through *sc_ports* whose interface is defined by classes derived from *sc_interface* and are connected using classes derived from *sc_channel*. However, to provide a generic implementation, our Kahn base class does not define any interface or port and it is up to the application developer to define them. Given that Kahn processes communicate with each other through FIFOs, classes derived from *kahn_process* can use FIFO interfaces (*sc_fifo_in_if*, *sc_fifo_out_if*) provided by SystemC to define their ports and connect them.

[*kpn_fifo* folder](https://github.com/kammirzazad/KPN-SystemC/tree/master/kpn_fifo) provides an example of how to define ports for exchanging *char*s with *sc_fifo* interface for producer and consumer processes. It defines output port for producer process as *sc_fifo_out<char>* which is a shortcut for *sc_port< sc_fifo_out_if<char> >*. 
```
sc_fifo_out<char> out;
```
In general case, one could replace *char* with any data type that application uses. Input port for consumer is defined similarly. Furthermore, multiple ports can simply be defined as arrays of *sc_fifo_out* or *sc_fifo_in*. Once ports are defined, data can be sent on output ports using *write()* method and received from input ports using *read()* method.

# Process Networks
To build a process network that consists of multiple processes, one needs to instantiate processes and channels that implement the input/output interface of its consumer/producer in a top-level module. For example, in *kpn_fifo*:
```
p0 = new producer("producer0");
c0 = new consumer("consumer0");
producer_to_consumer = new sc_fifo<char>(1);
```
Consequently, ports of the processes need to be initialized with channels.
```
c0->in(*producer_to_consumer);
p0->out(*producer_to_consumer);  
```
Finally, the top-level module in SystemC can be simulated by instantiating it in *sc_main()* and calling global method of *sc_start()*. Note that, unlike traditional C program, there is **no** *main()* method and execution rather starts from *sc_main()*.
```
int sc_main(int, char *[])
{
    kpn_fifo kf0("kpn_fifo0");
    sc_start();
    return 0;
}
```

# Modeling Neural Networks
[*kpn_neuralnet* folder](https://github.com/kammirzazad/KPN-SystemC/tree/master/kpn_neuralnet) provides an example of how to model a layer-wise pipelined neural network using our implementation. This example defines placeholder *image_reader*, *image_writer*, *conv_layer*, *max_layer* and *detection_layer* classes that are derived from *kahn_process*. This classes are interfaced in top-level module of *kpn_neuralnet* using *sc_fifo*s to build a neural network with two convolutional and one maxpool layer.
