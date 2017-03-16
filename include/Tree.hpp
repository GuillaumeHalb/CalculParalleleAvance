//
// Created by halbg on 3/10/17.
//

#ifndef ARBREBINOMIAL_TREE_H
#define ARBREBINOMIAL_TREE_H

#include <iostream>
#include "../include/Option.hpp"
#include "../include/Model.hpp"

class Tree {
private:
    int depth;
    double *data; // array of double representing a matrix (matrix representing the tree)

public:
    Tree(int depth);
    ~Tree();
    const int getDepth();
    const double* getData();
    const double get(const int i, const int j);
    void set(const int i, const int j, const double price);
    const void printTree(const int root_i, const int root_j);
    void fillLeaves(Option option, Model model);
    void solveCRR(const int root_i, const int root_j, const int size, Option option, Model model);
    void solveCRR(Option option, Model model);
    void solveCRRUpsidedown(const int root_i, const int root_j, const int size, Option option, Model model);
};



#endif //ARBREBINOMIAL_TREE_H
