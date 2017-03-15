//
// Created by lebitb on 3/14/17.
//

#ifndef LAB1_SOURCES_TREE_LIB_H
#define LAB1_SOURCES_TREE_LIB_H


void initOption(std::vector<float> &Option);

void initModel(std::vector<float> &Model);

void fillLeaves(std::vector<float> &Data);

void printTree(std::vector<float> &Data);

void results( std::vector<float>& Data, double run_time);

#endif //LAB1_SOURCES_TREE_LIB_H
