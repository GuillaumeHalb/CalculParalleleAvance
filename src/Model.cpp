//
// Created by halbg on 3/13/17.
//

#include "../include/Option.hpp"
#include "../include/Model.hpp"
#include <math.h>
#include <iostream>


Model::Model(Option option, int N):stepNumber(N)
{
    timeStep = option.getMaturity()/(double)N;

    u = exp(option.getVolatility()*sqrt(timeStep));
    d = 1/u;

    Pu = (d-exp(option.getRiskFreeRate()*timeStep))/(d-u);
    Pd = 1 - Pu;
    discountFactor = exp(-option.getRiskFreeRate()*timeStep);
}

Model::~Model()
{
}

const double Model::getU()
{
    return u;
}

const double Model::getD()
{
    return d;
}

const double Model::getPu()
{
    return Pu;
}

const double Model::getPd()
{
    return Pd;
}

const double Model::getTimeStep()
{
    return timeStep;
}

const int Model::getStepNumber()
{
    return stepNumber;
}

const double Model::getDiscountFactor()
{
    return discountFactor;
}

const void Model::print() {
    std::cout << "Display model :" << std::endl;
    std::cout << "\tstepNumber : " << stepNumber << std::endl;
    std::cout << "\ttimeStep : " << timeStep << std::endl;
    std::cout << "\tu : " << u << std::endl;
    std::cout << "\td : " << d << std::endl;
    std::cout << "\tPu : " << Pu << std::endl;
    std::cout << "\tPd : " << Pd << std::endl;
}
