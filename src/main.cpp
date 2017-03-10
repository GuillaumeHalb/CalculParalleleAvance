#include <iostream>
#include "../include/Option.hpp"
#include "../include/Tree.hpp"
#include <math.h>
#include <sstream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    double X0 = 100;
    double K = 10;
    double r = 0.05;
    double sigma = 0.2;
    double T = 3;

    Option option(X0, K, r, sigma, T);
    option.print();

    // Number of step
    int N;
    double step = option.getMaturity()/N;

    double u = exp(sigma*sqrt(step));
    double d = exp(-sigma*sqrt(step));

    double Pu = (d-exp(r*step))/(d-u);
    double Pd = 1 - Pu;

    Tree* leaf1 = new Tree(5.0, 2);
    Tree* leaf2 = new Tree(4.0, 2);
    Tree* leaf3 = new Tree(3.0, 2);
    Tree* tree1 = new Tree(leaf1, leaf2, 4.5, 1);
    Tree* tree2 = new Tree(leaf2, leaf3, 3.5, 1);
    Tree* root = new Tree(tree1, tree2, 4.0, 0);


    // Probleme d'affichage
    std::cout << "Affichage: " << std::endl;
    std::cout << "tree1: " << std::endl;
    std::cout << tree1->toStream(std::cout, 0) << std::endl;


    // Creation de l'arbre du sujet


/*    root->~Tree();
    tree1->~Tree();
    tree2->~Tree();
    leaf1->~Tree();
    leaf2->~Tree();
    leaf3->~Tree();*/

    return 0;
}