//
// Created by halbg on 3/10/17.
//

#ifndef ARBREBINOMIAL_TREE_H
#define ARBREBINOMIAL_TREE_H

#include <iostream>

class Tree {
private:
    Tree* upSon;
    Tree* downSon;
    double value;
    int date;

public:
    Tree(Tree* l, Tree* r, double v, int d);
    Tree(double v, int d);
    ~Tree();
    const int depth();
    friend std::ostream& operator<<(std::ostream& os, const Tree& t);
    const double getValue();
    const Tree* getUpSon();
    const Tree* getDownSon();
    std::ostream& toStream(std::ostream& os, int lastSize);
};



#endif //ARBREBINOMIAL_TREE_H
