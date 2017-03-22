#include "binomial.hpp"
//includes for sequential version
#include <iostream>
#include "Option.hpp"
#include "Tree.hpp"
#include <sstream>
//end sequential
#include "util.hpp"
#include <err_code.h>
#include "device_picker.hpp"


std::string kernelsource = "__kernel void binomial(                                                     \n" \
"   const int depth,                                                                                 \n" \
"   const int root_i,                                                                                \n" \
"   const int root_j,                                                                                \n" \
"   __global float* option,                                                                             \n" \
"   __global float* model,                                                                              \n" \
"   __global float* data)                                                                               \n" \
"{                                                                                                      \n" \
/*"       root_j = get_global_id(1);                                                              \n" \
"       root_i = get_global_id(0);                                                              \n" \*/
"       float stock_root = option[0] * pow(model[3], root_i - root_j) * pow(model[2], root_j);  \n" \
"       int line;                                                            \n" \
"       int column ;                                                          \n" \
"                                                                                                       \n" \
"       for (line = depth + root_i ; line >= root_i ; line--)                                           \n" \
"       {                                                                                               \n" \
        "           float stock = stock_root * pow(model[3], (line - root_i));                          \n" \
"           for (column = root_j ; column <= (line - root_i) + root_j ; column++)                       \n" \
"           {                                                                                           \n" \
"               if (column <= line) {                                                                   \n" \
"                   float payoff = option[1] - stock;                                           \n" \
"                   if (payoff < 0) {                                                                   \n" \
"                       payoff = 0;                                                                     \n" \
"                   }                                                                                   \n" \
"                                                                                                       \n" \
"                   float value = fmax(payoff, model[6]*(model[4]*data[column + 1 + (line + 1)*(line + 2)/2]\n" \
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

    int Depth = DEPTH; //default value
    int Root_i = 0, Root_j = 0;
    std::vector<float> Option_cl(5);
    std::vector<float> Model_cl(7);
    std::vector<float> Data(((Depth+1)*(Depth+2))/2);
    std::string debug = "";

    cl::Buffer d_Option ;
    cl::Buffer d_Model ;
    cl::Buffer d_Data ;
    cl::Buffer d_Debug ;

    try {


        //************************************************************************************************
        //Sequetial version
        //************************************************************************************************


        timer.reset();
        start_time = static_cast<double>(timer.getTimeMilliseconds()) / 1000.0;
        for (int i =0; i < COUNT; i++)
        {

            Option option(static_cast<double>(XO), static_cast<double>(STRIKE), static_cast<double>(RATE),
                          static_cast<double>(SIGMA), static_cast<double>(MATURITY));
            //option.print();

            // Number of step
            int N = (int)DEPTH;

            Model model(option, N);
            //model.print();

            // Creation de l'arbre du sujet
            Tree tree(N);
            tree.fillLeaves(option, model);
            //std::cout << "Initialisation séquentielle des feuilles" << std::endl;
            //tree.printTree(0, 0);


            tree.solveCRR(option, model);
           // std::cout << "CRR séquentielle" << std::endl;
            //tree.printTree(0, 0);

        }
        run_time  = (static_cast<double>(timer.getTimeMilliseconds()) / 1000.0) - start_time;
        results(Data,run_time);

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


        //************************************************************************************************
        //Parallel version
        //************************************************************************************************

        initOption(Option_cl);
        initModel(Model_cl);
        fillLeaves(Data);

        d_Option = cl::Buffer(context,Option_cl.begin(),Option_cl.end(),true);
        d_Model = cl::Buffer(context,Model_cl.begin(),Model_cl.end(),true);
        d_Data = cl::Buffer(context, Data.begin(), Data.end(), false );

        cl::Program program(context, util::loadProgram("binomial.cl"), true);

        cl::make_kernel<cl::Buffer,cl::Buffer,cl::Buffer,int,int,int,int> binomial(program,"binomial");


        start_time = static_cast<double>(timer.getTimeMilliseconds()) / 1000.0;
        for(int i =0; i < COUNT; i++)
        {



            //Setting variables
            int size = 0;
            int root_i = 0;

            if (Depth % 2 ==1) {
                size = Depth - ((int) Depth / 2 + 1);
                root_i = Depth/2 + 1;
            }
            else {
                size = Depth - ((int) Depth / 2);
                root_i = Depth/2;
            }

            int global_size = size*(size+1)/2;

            //Upright tree solve
            cl::NDRange global(2*size, size);
            cl::NDRange local(size, size);

            binomial(cl::EnqueueArgs(queue,global, local),d_Option,d_Model,d_Data, Depth, size,root_i,0);

            //Variables for UpsideDown tree solve
            root_i = (root_i - 1)*2;
            int sizeU = size - (Depth - root_i);

            //UpsideDown tree solve
            binomial(cl::EnqueueArgs(queue,global, local),d_Option,d_Model,d_Data, Depth, sizeU,root_i,1);

            cl::NDRange global2(2*(size+1), size+1);
            cl::NDRange local2(size + 1, size + 1);
            binomial(cl::EnqueueArgs(queue,global2, local2),d_Option,d_Model,d_Data, Depth, size,0,2);

            queue.finish();
            cl::copy(queue,d_Data,Data.begin(),Data.end());
            //std::cout << "CRR parallele" << std::endl;
            //printTree(Data);
        }
        run_time  = (static_cast<double>(timer.getTimeMilliseconds()) / 1000.0) - start_time;
        results(Data,run_time);

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
	
