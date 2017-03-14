#include "binomial.hpp"
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

void initOption(std::vector<float> &Option, float X0, float K, float r, float sigma, float T) {
    Option[0] = X0;
    Option[1] = K;
    Option[2] = r;
    Option[3] = sigma;
    Option[4] = T;
}

void initModel(std::vector<float> &Model, int Depth, float timeStep, float u, float d, float Pu, float Pd, float discountFactor) {
    Model[0] = (float) Depth;
    Model[1] = timeStep;
    Model[2] = u;
    Model[3] = d;
    Model[4] = Pu;
    Model[5] = Pd;
    Model[6] = discountFactor;
}

void feelLeaves(std::vector<float> &Data, float X0, int Depth, float d, float u, float K) {
    double spot = X0*pow(d, Depth);
    for (int column = 0; column <= Depth; column++) {
        Data[column + Depth*(Depth+1)/2] = K-spot > 0 ? K-spot : 0;
        spot *= u/d;
    }
}

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

    float X0 = 100;
    float K = 100;
    float r = 0.05;
    float sigma = 0.2;
    float T = 3;


/*    Option[0] = X0;
    Option[1] = K;
    Option[2] = r;
    Option[3] = sigma;
    Option[4] = T;*/

/*    for (int i = 0; i < 5; i++) {
        std::cout << Option[i] << std::endl;
    }*/

    float timeStep = T/(float) Depth;
    float u = exp(sigma*sqrt(timeStep));
    float d = 1/u;
    float Pu = (d-exp(r*timeStep))/(d-u);
    float Pd = 1 - Pu;
    float discountFactor = exp(-r*timeStep);



/*    for (int i = 0; i < 7; i++) {
        std::cout << Model[i] << std::endl;
    }*/


/*    for (int line = 0; line <= Depth; line++)
    {
        for (int column = 0; column <= line; column++)
        {
            std::cout << Data[column + line*(line+1)/2] << " ";
        }
        printf("\n");
    }*/

    cl::Buffer d_Option ;
    cl::Buffer d_Model ;
    cl::Buffer d_Data ;

    std::cout << "before try"<< std::endl;

    try {


        cl_uint deviceIndex = 0;

        std::cout << "1"<< std::endl;

        parseArguments(argc, argv, &deviceIndex);

        std::cout << "2"<< std::endl;


        // Get list of devices
        std::vector<cl::Device> devices;
        std::cout << "3"<< std::endl;

        unsigned numDevices = getDeviceList(devices);
        std::cout << "4"<< std::endl;

        // Check device index in range
        if (deviceIndex >= numDevices)
        {
            std::cout << "Invalid device index (try '--list')\n";
            return EXIT_FAILURE;
        }

        std::cout << "5"<< std::endl;

        cl::Device device = devices[deviceIndex];

        std::string name;
        getDeviceName(device, name);
        std::cout << "\nUsing OpenCL device: " << name << "\n";

        std::vector<cl::Device> chosen_device;
        chosen_device.push_back(device);
        cl::Context context(chosen_device);
        cl::CommandQueue queue(context, device);

std::cout << "initialisation vectors" << std::endl;

        initOption(Option, X0, K, r, sigma, T);
        initModel(Model, Depth, timeStep, u, d, Pu, Pd, discountFactor);
        feelLeaves(Data, X0, Depth, d, u, K);

        std::cout << "init buffer" << std::endl;

        d_Option = cl::Buffer(context,Option.begin(),Option.end(),true);
        d_Model = cl::Buffer(context,Model.begin(),Model.end(),true);
        d_Data = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(float)*Depth*(Depth+1)/2);
        std::cout << "fin init" << std::endl;


        std::cout << "Avant program" << std::endl;
        cl::Program program(context, kernelsource, true);


        std::cout << "avant kernel" << std::endl;
        cl::make_kernel<int,int,int,cl::Buffer,cl::Buffer,cl::Buffer> binomial(program,"binomial");
        std::cout << "apres kernel" << std::endl;


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
	
