/*
 *	kpn_neuralnet.cpp -- Sample code for modeling neural network using KPN model
 *
 *	System-Level Architecture and Modeling Lab
 *	Department of Electrical and Computer Engineering
 *	The University of Texas at Austin 
 *
 * 	Author: Kamyar Mirzazad Barijough (kammirzazad@utexas.edu)
 */

#include <vector>
#include <string>
#include <iostream>
#include <systemc.h>
#include "../kahn_process.h"

using	std::cout;
using	std::endl;
typedef std::vector<std::string> strs;

class	image_reader : public kahn_process
{
	public:

	strs	images;

	sc_fifo_out<float> out;

	image_reader(sc_module_name name, strs _images)
	:	kahn_process(name),
		images(_images)
	{
		cout << "instantiated image_reader" << endl;
	}

	void	process() override
	{
		float	val = 1.234;

		for(size_t i=0; i<images.size(); i++)
		{
			cout << "reading image " << images[i] << " @ iter " << iter++ << endl;

			// read images[i] from file
			// for(val in images[i])
			// 	out->write(val);
			out->write(val);
		}
	}
}; 

class	image_writer : public kahn_process
{
	public:

	int	iter;

	strs	images;

	sc_fifo_in<float> in;

	image_writer(sc_module_name name, strs _images)
	:	kahn_prcoess(name),
		images(_images)
	{
		cout << "instantiated image_writer" << endl;
	}

	void	process() override
	{
		float	    val;
		std::string outFN;

		for(size_t i=0; i<images.size(); i++)
		{
			// read values from "in"
			in->read(val);

			// dump to file
			outFN = "predicted_";
			outFN += images[i];

			cout << "writing predictions to " << outFN << "  @ iter " << iter++ << endl;
		}
	}
};

class	conv_layer : public kahn_process
{
	public:

	const	int stride;
	const	int numFilters;
	const	int layerIndex;
	const	int filterSize;	

	sc_fifo_in<float> in;
	sc_fifo_out<float> out;

	conv_layer(sc_module_name name, int _layerIndex, int _filterSize, int _stride, int _numFilters)
	:	kahn_process(name),
		stride(_stride),
		numFilters(_numFilters),
		layerIndex(_layerIndex),
		filterSize(_filterSize)
	{
		cout << "instantiated convolutional layer " << layerIndex << " with filter size of " << filterSize << ", stride of " << stride << " and " << numFilters << " filters" << endl;
	}

	void	process() override
	{
		float val;

		in->read(val);
		cout << "forwarding convolutional layer " << layerIndex << " @ iter " << iter << endl;
		out->write(10*val);
	}
};

class	max_layer : public kahn_process
{
	public:

	const	int stride;
	const	int layerIndex;
	const	int filterSize;	

	sc_fifo_in<float> in;
	sc_fifo_out<float> out;

	max_layer(sc_module_name name, int _layerIndex, int _filterSize, int _stride) 
	:	kahn_process(name),
		stride(_stride),
		layerIndex(_layerIndex),
		filterSize(_filterSize)
	{
		cout << "instantiated max layer " << layerIndex << " with filter size of " << filterSize << " and stride of " << stride << endl;
	}

	void	process() override
	{
		float val;

		in->read(val);
		cout << "forwarding max layer " << layerIndex << " @ iter " << iter << endl;
		out->write(val+1.5);
	}
};

class	detection_layer : public kahn_process
{
	public:

	sc_fifo_in<float> in;
	sc_fifo_out<float> out;

	detection_layer(sc_module_name name) 
	:	kahn_process(name)
	{
		cout << "instantiated detection layer " << endl;
	}

	void	process() override
	{
		float val;

		in->read(val);
		cout << "forwarding detection layer @ iter " << iter << endl;
		out->write(val+0.1);
	}
};

class	kpn_neuralnet : public sc_module
{
	public:

	sc_fifo<float>	*reader_to_conv0, 
			*conv0_to_max1, 
			*max1_to_conv2,
			*conv2_to_detection,
			*detection_to_writer;

	max_layer	*max1;
	conv_layer	*conv0, *conv2;
	image_reader	*reader0;
	image_writer	*writer0;
	detection_layer	*det0;

	kpn_neuralnet(sc_module_name name) : sc_module(name)
	{
		strs images = {"dog.jpg", "horse.jpg"};

		reader_to_conv0 = new sc_fifo<float>(1);
		conv0_to_max1   = new sc_fifo<float>(1);
		max1_to_conv2   = new sc_fifo<float>(1);
		conv2_to_detection = new sc_fifo<float>(1);
		detection_to_writer = new sc_fifo<float>(1);

		reader0 = new image_reader("image_reader",images);
		reader0->out(*reader_to_conv0);

		conv0 = new conv_layer("conv0",0,3,1,16);
		conv0->in(*reader_to_conv0);
		conv0->out(*conv0_to_max1);

		max1 = new max_layer("max1",1,2,2);
		max1->in(*conv0_to_max1);
		max1->out(*max1_to_conv2);

		conv2 = new conv_layer("conv2",2,3,1,32);
		conv2->in(*max1_to_conv2);
		conv2->out(*conv2_to_detection);

		det0 = new detection_layer("detection");
		det0->in(*conv2_to_detection);
		det0->out(*detection_to_writer);

		writer0 = new image_writer("image_writer",images);
		writer0->in(*detection_to_writer);
	}
};

int	sc_main(int, char *[]) 
{
	kpn_neuralnet knn0("kpn_neuralnet");
	sc_start();
	return 0;
}
