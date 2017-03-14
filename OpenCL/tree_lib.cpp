//
// Created by lebitb on 3/14/17.
//

#include "tree_lib.hpp"
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

void feelLeaves(std::vector<float> &Data) {
double spot = XO*pow(exp(-SIGMA*sqrt(MATURITY/(float)DEPTH)), DEPTH);
    for (int column = 0; column <= DEPTH; column++) {
        Data[column + DEPTH*(DEPTH+1)/2] = STRIKE-spot > 0 ? STRIKE-spot : 0;
        spot *= exp(2*SIGMA*sqrt(MATURITY/(float)DEPTH));
    }
}