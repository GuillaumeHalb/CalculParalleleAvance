//
// Created by lebitb on 3/14/17.
//

#ifndef LAB1_SOURCES_TREE_LIB_H
#define LAB1_SOURCES_TREE_LIB_H


class Tree_lib {
    void initOption(std::vector<float> &Option);

    void initModel(std::vector<float> &Model);

    void feelLeaves(std::vector<float> &Data, float X0, int Depth, float d, float u, float K);
};


#endif //LAB1_SOURCES_TREE_LIB_H
