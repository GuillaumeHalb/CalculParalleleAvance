#include <iostream>
#include "../include/Option.hpp"
#include "../include/Tree.hpp"
#include <sstream>

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
    int N = 7;

    Model model(option, N);
    model.print();

    // Creation de l'arbre du sujet
    //int depth = 7;
    Tree tree(N);
    tree.fillLeaves(option, model);
    std::cout << std::endl;
    tree.printTree(0, 0);
    tree.solveCRR(0, 0, option, model);
    tree.printTree(0, 0);

    return 0;
}