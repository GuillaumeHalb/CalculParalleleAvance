//
// Created by lebitb on 3/14/17.
//

#ifndef LAB1_SOURCES_BINOMIAL_H
#define LAB1_SOURCES_BINOMIAL_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>

#include <vector>

#define __CL_ENABLE_EXCEPTIONS
#include "cl.hpp"

#include "tree_lib.hpp"

extern double wtime();   // returns time since some fixed past point (wtime.c)

// ----------------------------------------------------------------
//  Constants
// ----------------------------------------------------------------
#define XO    100    // Order of the square matrices A, B, and C
#define STRIKE     100     // A elements are constant and equal to AVAL
#define RATE     0.05     // B elements are constant and equal to BVAL
#define SIGMA      0.2 // tolerance used in floating point comparisons
#define MATURITY      3       // Max dim for NDRange
#define SUCCESS  1
#define FAILURE  0
#define DEPTH 7

#endif //LAB1_SOURCES_BINOMIAL_H
