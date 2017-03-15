//
// Created by halbg on 3/13/17.
//

#ifndef ARBREBINOMIAL_MODEL_HPP
#define ARBREBINOMIAL_MODEL_HPP

#include "Option.hpp"

class Model {
private:
    int stepNumber;
    double timeStep;
    double u; // up factor
    double d; // down factor
    double Pu; // probability of going up
    double Pd; // probability of going down
    double discountFactor;

public:
    Model(Option option, int N);
    ~Model();
    const double getU();
    const double getD();
    const double getPu();
    const double getPd();
    const double getTimeStep();
    const int getStepNumber();
    const void print();
    const double getDiscountFactor();
};

#endif //ARBREBINOMIAL_MODEL_HPP
