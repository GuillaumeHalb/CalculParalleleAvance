#include "binomial.hpp"
#include "util.hpp"
#include <err_code.h>
#include "device_picker.hpp"

std::string kernelsource = "__kernel void binomial(                                                     \n" \
"   __global int depth,                                                                                 \n" \
"   __global int root_i,                                                                                \n" \
"   __global int root_j,                                                                                \n" \
"   __global float* option,                                                                             \n" \
"   __global float* model,                                                                              \n" \
"   __global float* data)                                                                               \n" \
"{                                                                                                      \n" \
"       __local float stock_root = option[0] * pow(model[3], root_i - root_j) * pow(model[2], root_j);  \n" \
"       __local int line = get_global_id(0);                                                            \n" \
"       __local int column = get_globale_id(1);                                                         \n" \
"                                                                                                       \n" \
"       for (line = depth + root_i ; line >= root_i ; line--)                                           \n" \
"       {                                                                                               \n" \
"           __local float stock = stock_root * pow(model[3], (line - root_i));                          \n" \
"           for (column = root_j ; column <= (line - root_i) + root_j ; column++)                       \n" \
"           {                                                                                           \n" \
"               if (column <= line) {                                                                   \n" \
"                   __local float payoff = option[K] - stock;                                           \n" \
"                   if (payoff < 0) {                                                                   \n" \
"                       payoff = 0;                                                                     \n" \
"                   }                                                                                   \n" \
"                                                                                                       \n" \
"                   __local float value = fmax(payoff, model[6]*(model[4]*data[column + 1 + (line + 1)*(line + 2)/2]\n" \
"                   + model[5] * data[column + (line + 1)*(line + 2)/2]));                              \n" \
"                   data[column + line*(line+1)/2] = value;                                             \n" \
"                   stock *= model[2] / model[3];                                                       \n" \
"               }                                                                                       \n" \
"           }                                                                                           \n" \
"       }                                                                                               \n" \
"}                                                                                                      \n" \
                                                                                                        "\n";

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
            cl::Program program(context, kernelsource, true);
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
	
