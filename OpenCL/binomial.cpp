#include "binomial.hpp"
#include "util.hpp"
#include <err_code.h>
#include "device_picker.hpp"



int main(int argc, char *argv[])
{



	double start_time;      // Starting time
        double run_time;        // Timing
    	util::Timer timer;      // Timing

	float* Option;
	float* Model;
	float* Data;	

	int Depth = 7; //default value
	int Root_i, Root_j;
	
	Option = (float*)malloc(5*sizeof(float));
	Model = (float*)malloc(7*sizeof(float));
	Data = (float*)malloc(Depth*sizeof(float));

	cl::Buffer d_Option ;
	cl::Buffer d_Model ;
	cl::Buffer d_Data ;
	
	try {
		cl_uint deviceIndex = 0;
        	parseArguments(argc, argv, &deviceIndex);

		 // Get list of devices
        	std::vector<cl::Device> devices;
        	unsigned numDevices = getDeviceList(devices);

        	// Check device index in range
        	if (deviceIndex >= numDevices)
        	{
          		std::cout << "Invalid device index (try '--list')\n";
          		return EXIT_FAILURE;
        	}

		cl::Device device = devices[deviceIndex];

        	std::string name;
        	getDeviceName(device, name);
        	std::cout << "\nUsing OpenCL device: " << name << "\n";

        	std::vector<cl::Device> chosen_device;
        	chosen_device.push_back(device);
        	cl::Context context(chosen_device);
        	cl::CommandQueue queue(context, device);

	}  catch ( cl::Error err)
    {
        std::cout << "Exception\n";
        std::cerr << "ERROR: "
                  << err.what()
                  << "("
                  << err_code(err.err())
                  << ")"
                  << std::endl;
    }

    return EXIT_SUCCESS;
}
	
