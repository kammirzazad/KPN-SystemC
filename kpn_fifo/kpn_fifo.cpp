/*
 *	kpn_fifo.cpp -- Sample code for modeling producer-consumer pair using KPN model
 *
 *	EE382N.13 Embedded System Design and Modeling
 *	Department of Electrical and Computer Engineering
 *	The University of Texas at Austin 
 *
 * 	Author: Kamyar Mirzazad Barijough (kammirzazad@utexas.edu)
 */

#include <string>
#include <iostream>
#include <systemc.h>
#include "../kahn_process.h"

class	producer : public kahn_process
{
	public:

	sc_fifo_out<char> out;

	producer(sc_module_name name)
	:	kahn_process(name),
		counter(0),
		message("Embedded System Design and Modelling\n")
	{
		std::cout << "Instantiated producer!" << std::endl;
	}

	protected:

	void	process() override
	{
		out->write(message[counter++]);

		if(counter == (int)message.size())
			counter = 0;
	}

	private:

	int	counter;
	const	std::string message;
};

class	consumer : public kahn_process
{
	public:

	sc_fifo_in<char> in;

	consumer(sc_module_name name)
	:	kahn_process(name)
	{
		std::cout << "Instantiated consumer!" << std::endl;
	}

	protected:

	void	process() override
	{
		char val;

		in->read(val);

		if(val == ' ')
			std::cout << "-";
		else
			std::cout << val;
	}
};

class	kpn_fifo : public sc_module
{
	public:

	producer	*p0;
	consumer	*c0;
	sc_fifo<char>	*producer_to_consumer;

	kpn_fifo(sc_module_name name) : sc_module(name)
	{
		p0 = new producer("producer0");
		c0 = new consumer("consumer0");
		producer_to_consumer = new sc_fifo<char>(1);

		c0->in(*producer_to_consumer);
		p0->out(*producer_to_consumer);
	}
};

int	sc_main(int, char *[])
{
	kpn_fifo kf0("kpn_fifo0");
        sc_start();
        return 0;
}
