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
#define XO    100.0
#define STRIKE     100.0
#define RATE     0.05
#define SIGMA      0.2
#define MATURITY      3.0
#define SUCCESS  1
#define FAILURE  0
#define DEPTH 40.0
#define COUNT 1

#endif //LAB1_SOURCES_BINOMIAL_H
