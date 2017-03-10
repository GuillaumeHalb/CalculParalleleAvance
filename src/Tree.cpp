//
// Created by halbg on 3/10/17.
//
#include <clocale>
#include <iostream>
#include <sstream>
#include "../include/Tree.hpp"

Tree::Tree(Tree* l, Tree* r, double v, int d) : upSon(l), downSon(r), value(v), date(d)
{
}

Tree::Tree(double v, int d) : value(v), date(d)
{
    upSon = NULL;
    downSon = NULL;
}

Tree::~Tree() {
    if (this->upSon != NULL) {
        this->upSon->~Tree();

    }
    if (this->downSon != NULL) {
        this->downSon->~Tree();
    }
}

const int Tree::depth()
{
    if (upSon == NULL) {
        return 0;
    } else {
        return 1 + this->downSon->depth();
    }
}

const double Tree::getValue()
{
    return this->value;
}

const Tree* Tree::getUpSon() {
    return this->upSon;
}

const Tree* Tree::getDownSon() {
    return this->downSon;
}

std::ostream& operator<<(std::ostream& os, const Tree& t)
{
    std::stringstream stringStream("");

    stringStream << "(t=" << t.date << ", v=" << t.value << ")";

    os << stringStream.str();

    if (t.upSon != NULL) {
        os << " -> " << *t.upSon << std::endl;
    }
    if (t.downSon != NULL) {
        for (int i = 0; i < stringStream.str().size(); i++) {
            os << " ";
        }
        os << " -> " << *t.downSon << std::endl;
    }
    return os;
}

std::ostream& Tree::toStream(std::ostream& os, int lastSize) {
    std::stringstream stringStream("");

    stringStream << "-> (t=" << this->date << ", v=" << this->value << ") ";
    os << stringStream.str();

    if (this->upSon != NULL) {
        os << this->upSon->toStream(os, lastSize + stringStream.str().size()) << std::endl;
    }
    if (this->downSon != NULL) {
        for (int i = 0; i < stringStream.str().size() + lastSize; i++) {
            os << " ";
        }
        os <<  this->downSon->toStream(os, lastSize + stringStream.str().size()) << std::endl;
    }
    return os;
}