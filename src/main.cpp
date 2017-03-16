#include <iostream>
#include "../include/Option.hpp"
#include "../include/Tree.hpp"
#include <sstream>
#include <X11/Xlib.h>

int main() {
    std::cout << "Hello, World!" << std::endl;
    double X0 = 100;
    double K = 100;
    double r = 0.05;
    double sigma = 0.2;
    double T = 3;

    Option option(X0, K, r, sigma, T);
    option.print();

    // Number of step
    int N = 3;

    Model model(option, N);
    model.print();

    // Creation de l'arbre du sujet
    Tree tree(N);
    tree.fillLeaves(option, model);
    std::cout << std::endl;
    tree.printTree(0, 0);
    std::cout << std::endl;

    tree.solveCRR(option, model);
    tree.printTree(0, 0);
    std::cout << std::endl;

    tree.fillLeaves(option, model);
    tree.solveCRR(2, 0, N, option, model);
    tree.printTree(0, 0);
    std::cout << std::endl;

    tree.fillLeaves(option, model);
    std::cout << std::endl;
    tree.solveCRR(2, 2, N, option, model);
    tree.printTree(0, 0);
    std::cout << std::endl;

    tree.fillLeaves(option, model);
    tree.solveCRR(2, 2, N, option, model);
    tree.solveCRR(2, 0, N, option, model);
    tree.printTree(0, 0);
    std::cout << std::endl;

    tree.solveCRRUpsidedown(2, 1, 1, option, model);
    tree.printTree(0, 0);
    std::cout << std::endl;

    tree.fillLeaves(option, model);
    tree.solveCRRUpsidedown(N, N-1, N, option, model);
    tree.printTree(0, 0);

    return 0;
}