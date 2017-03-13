//
// Created by halbg on 3/10/17.
//
#include <iostream>
#include "../include/Option.hpp"

Option::Option(double x, double k, double R, double s, double t) : X0(x), K(k), r(R),
                                                                        sigma(s), T(t)
{
}

Option::~Option()
{
}

const double Option::getSpot()
{
    return X0;
}

const double Option::getStrike()
{
    return K;
}

const double Option::getRiskFreeRate()
{
    return r;
}

const double Option::getVolatility()
{
    return sigma;

}

const double Option::getMaturity()
{
    return T;
}

const void Option::print()
{
    std::cout << "Display option: " << std::endl;
    std::cout << "\tSpot: " << X0 << std::endl;
    std::cout << "\tStrike: " << K << std::endl;
    std::cout << "\tRisk free rate: " << r << std::endl;
    std::cout << "\tVolatility: " << sigma << std::endl;
    std::cout << "\tMaturity: " << T << std::endl;
}

const double Option::payoff(double spot) {
    return K-spot > 0 ? K-spot : 0;
}