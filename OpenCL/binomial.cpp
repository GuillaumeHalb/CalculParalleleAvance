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
"       float stock_root = option[0] * pow(model[3], root_i - root_j) * pow(model[2], root_j);  \n" \
"       int line = get_global_id(0);                                                            \n" \
"       int column = get_global_id(1);                                                          \n" \
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
    int Root_i, Root_j;
    std::vector<float> Option_cl(5);
    std::vector<float> Model_cl(7);
    std::vector<float> Data(((Depth+1)*(Depth+2))/2);


    //************************************************************************************************
    //Sequetial version
    //************************************************************************************************

    timer.reset();
    for (int i =0; i < COUNT; i++)
    {
        start_time = static_cast<double>(timer.getTimeMilliseconds()) / 1000.0;
        Option option(static_cast<double>(XO), static_cast<double>(STRIKE), static_cast<double>(RATE),
                      static_cast<double>(SIGMA), static_cast<double>(MATURITY));
        option.print();

        // Number of step
        int N = (int)DEPTH;

        Model model(option, N);
        model.print();

        // Creation de l'arbre du sujet
        Tree tree(N);
        tree.fillLeaves(option, model);
        std::cout << "Initialisation séquentielle des feuilles" << std::endl;
        tree.printTree(0, 0);
        tree.solveCRR(0, 0, option, model);
        std::cout << "CRR séquentielle" << std::endl;
        tree.printTree(0, 0);

        run_time  = (static_cast<double>(timer.getTimeMilliseconds()) / 1000.0) - start_time;
        results(Data,run_time);
    }

    //************************************************************************************************
    //Sequetial version upsidedown
    //************************************************************************************************
    Option option(static_cast<double>(XO), static_cast<double>(STRIKE), static_cast<double>(RATE),
                  static_cast<double>(SIGMA), static_cast<double>(MATURITY));
    option.print();

    // Number of step
    int N = (int)DEPTH;
    Model model(option, N);
    model.print();
    Tree tree(N);
    tree.fillLeaves(option, model);
    std::cout << "Initialisation séquentielle des feuilles" << std::endl;
    tree.printTree(0, 0);
    tree.solveCRRUpsidedown(0, 0, option, model);
    std::cout << "CRR upside down séquentielle" << std::endl;
    tree.printTree(0, 0);

    //*****************************************************************************************************
    //*****************************************************************************************************
    //*****************************************************************************************************
/*
    std::cout << "Avant Init" << std::endl;
    printTree(Data);*/

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


        //************************************************************************************************
        //Parallel version
        //************************************************************************************************

        initOption(Option_cl);
        initModel(Model_cl);
        fillLeaves(Data);

        std::cout << "Initialisation parallele des feuilles" << std::endl;
        printTree(Data);

        d_Option = cl::Buffer(context,Option_cl.begin(),Option_cl.end(),true);
        d_Model = cl::Buffer(context,Model_cl.begin(),Model_cl.end(),true);
        d_Data = cl::Buffer(context, Data.begin(), Data.end(), false );

        cl::Program program(context, kernelsource, true);

        cl::make_kernel<int,int,int,cl::Buffer,cl::Buffer,cl::Buffer> binomial(program,"binomial");


        for(int i =0; i < COUNT; i++)
        {
            start_time = static_cast<double>(timer.getTimeMilliseconds()) / 1000.0;
            cl::NDRange global(Depth,Depth);
            binomial(cl::EnqueueArgs(queue,global),Depth,Root_i,Root_j,d_Option,d_Model,d_Data);
            queue.finish();
            cl::copy(queue,d_Data,Data.begin(),Data.end());
            std::cout << "CRR parallele" << std::endl;
            printTree(Data);
            run_time  = (static_cast<double>(timer.getTimeMilliseconds()) / 1000.0) - start_time;
            results(Data,run_time);
        }

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
	
