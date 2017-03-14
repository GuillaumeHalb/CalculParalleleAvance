#include "binomial.hpp"
#include "util.hpp"
#include <err_code.h>
#include "device_picker.hpp"



int main(int argc, char *argv[])
{



	double start_time;      // Starting time
    double run_time;        // Timing
    util::Timer timer;      // Timing

    int Depth = 7; //default value
    int Root_i, Root_j;
    std::vector<float> Option(5);
    std::vector<float> Model(7);
    std::vector<float> Data((Depth*(Depth+1))/2);

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
        std::cout << "Avant program" << std::endl;
            cl::Program program(context,util::loadProgram("binomial.cl"));
        std::cout << "avant kernel" << std::endl;
                  cl::make_kernel<int,int,int,cl::Buffer,cl::Buffer,cl::Buffer> binomial(program,"binomial");
        std::cout << "apres kernel" << std::endl;
                  d_Option = cl::Buffer(context,Option.begin(),Option.end(),true);
        d_Model = cl::Buffer(context,Model.begin(),Model.end(),true);
        d_Data = cl::Buffer(context,Data.begin(),Data.end(),true);

        std::cout << "Avant NDRange" << std::endl;
        cl::NDRange global(Depth,Depth);
        binomial(cl::EnqueueArgs(queue,global),Depth,Root_i,Root_j,d_Option,d_Model,d_Data);
        queue.finish();
        cl::copy(queue,d_Data,Data.begin(),Data.end());



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
	
