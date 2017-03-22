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
    int N = 21;

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

    if (N % 2 == 1) {
        int rooti = N/2 + 1;
        int rootj = rooti;
        int size = N - rooti;
        tree.solveCRR(rooti, 0, size, option, model);
        std::cout << "sous arbre1" << std::endl;
        tree.printTree(0,0);
        tree.solveCRR(rooti, rootj, size, option, model);
        std::cout << "sous arbre2" << std::endl;
        tree.printTree(0,0);
        tree.solveCRRUpsidedown(N - 1, rootj - 1, size, option, model);
        std::cout << "sous arbre inversé" << std::endl;
        tree.printTree(0,0);
        std::cout << "total" << std::endl;
        tree.solveCRR(0,0, N - rooti, option, model);
        tree.printTree(0,0);
    } else {
        int rooti = N/2;
        int rootj = rooti;
        int size = N - rooti;
        tree.solveCRR(rooti, 0, size, option, model);
        std::cout << "sous arbre1" << std::endl;
        tree.printTree(0,0);
        tree.solveCRR(rooti, rootj, size, option, model);
        std::cout << "sous arbre2" << std::endl;
        tree.printTree(0,0);
        tree.solveCRRUpsidedown(N, rootj - 1, size + 1, option, model);
        std::cout << "sous arbre inversé" << std::endl;
        tree.printTree(0,0);
        std::cout << "total" << std::endl;
        tree.solveCRR(0,0, N - rooti, option, model);
        tree.printTree(0,0);
    }

    int depth = N;






    return 0;
}