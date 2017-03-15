//
// Created by lebitb on 3/14/17.
//
#include "binomial.hpp"

void initOption(std::vector<float> &Option) {
    Option[0] = XO;
    Option[1] = STRIKE;
    Option[2] = RATE;
    Option[3] = SIGMA;
    Option[4] = MATURITY;
}

void initModel(std::vector<float> &Model) {
    Model[0] = (float) DEPTH;
    Model[1] = MATURITY/(float) DEPTH;
    Model[2] = exp(SIGMA*sqrt(Model[1]));
    Model[3] = 1/Model[2];
    Model[4] = (Model[3]-exp(RATE*Model[1]))/(Model[3]-Model[2]);
    Model[5] = 1-Model[4];
    Model[6] = exp(-RATE * Model[1]);
}

void fillLeaves(std::vector<float> &Data) {
    float spot = XO * pow(exp(-SIGMA * sqrt(MATURITY/DEPTH)), (int)DEPTH);
    for (int column = 0; column <= (int)DEPTH; column++) {
        Data[column + (int)(DEPTH*(DEPTH+1)/2)] = STRIKE-spot > 0.0 ? STRIKE-spot : 0.0;
        spot *= exp(2*SIGMA*sqrt(MATURITY/DEPTH));
    }
}

void printTree(std::vector<float> &Data) {
    for (int line = 0; line <= DEPTH; line++)
    {
        for (int column = 0; column <= line; column++)
        {
            std::cout << Data[column + line*(line+1)/2] << " ";
        }
        printf("\n");
    }
}

void results( std::vector<float>& Data, double run_time)
{

    float mflops;
    //float errsq;

    mflops = 2.0 * DEPTH * DEPTH * DEPTH/(1000000.0f * run_time);
    printf(" %.2f seconds at %.1f MFLOPS \n",  run_time,mflops);
    //errsq = error(N, C);
    //if (std::isnan(errsq) || errsq > TOL)
    //    printf("\n Errors in multiplication: %f\n",errsq);
}