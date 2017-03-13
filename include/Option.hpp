//
// Created by halbg on 3/10/17.
//

#ifndef ARBREBINOMIAL_OPTION_H
#define ARBREBINOMIAL_OPTION_H

class Option {
private:
    double X0;
    double K;
    double r;
    double sigma;
    double T;

public:
    Option(double x, double k, double R, double s, double t);
    ~Option();
    const double getSpot();
    const double getStrike();
    const double getRiskFreeRate();
    const double getVolatility();
    const double getMaturity();
    const void print();
    const double payoff(double spot);
};

#endif //ARBREBINOMIAL_OPTION_H
