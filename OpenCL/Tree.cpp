//
// Created by halbg on 3/10/17.
//
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Tree.hpp"
#include "Option.hpp"
#include "Model.hpp"
#include <math.h>

Tree::Tree(int depth) : depth(depth)
{
    // Tridiagonal matrix of size N+1
    data = (double*) malloc(((depth+1) * (depth+2)) / 2 * sizeof(double));
}

Tree::~Tree() {
    free(data);
}

const int Tree::getDepth()
{
    return depth;
}

const double* Tree::getData()
{
    return this->data;
}

const double Tree::get(const int line, const int column) {
    return data[column + line*(line+1)/2];
}

void Tree::set(const int line, const int column, const double value) {
    data[column + line*(line+1)/2] = value;
}

const void Tree::printTree(const int root_i, const int root_j)
{
    for (int line = root_i; line <= root_i + depth; line++)
    {
        for (int column = root_j; column <= root_j + (line - root_i); column++)
        {
            std::cout << get(line, column) << " ";
        }
        printf("\n");
    }
}

void Tree::fillLeaves(Option option, Model model) {
    double spot = option.getSpot()*pow(model.getD(), depth);
    for (int column = 0; column <= depth; column++) {
        set(depth, column, option.payoff(spot));
        spot *= model.getU()/model.getD();
    }
}

void Tree::solveCRR(const int root_i, const int root_j, Option option, Model model) {
    double stock_root = option.getSpot() * pow(model.getD(), root_i - root_j) * pow(model.getU(), root_j);
    for (int line = depth + root_i ; line >= root_i ; line--)
    {
        double stock = stock_root * pow(model.getD(), (line - root_i));
        for (int column = root_j ; column <= (line - root_i) + root_j ; column++)
        {
            if (column <= line) {
                double value = fmax(option.payoff(stock),
                                    model.getDiscountFactor() * (model.getPu() * get(line + 1, column + 1) +
                                                                 model.getPd() * get(line + 1, column)));
                set(line, column, value);
                stock *= model.getU() / model.getD();
            }
        }
    }
}
