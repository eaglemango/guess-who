/*!
    @file
    @brief .dot files processor
*/
#ifndef DOTTER_HEADER
#define DOTTER_HEADER

#include <cstdio>

#include "trees.hpp"

class Dotter {
public:
    Dotter();

    ~Dotter();
    Dotter(const Dotter& dotter) = default;
    Dotter& operator=(const Dotter& dotter) = default;

    void ProcessGuessTree(const GuessTree& guess_tree);

private:
    FILE* output;

    void ProcessGuessTreeNode(const GuessTree& guess_tree, size_t node_number);
};

Dotter::Dotter() {
    output = fopen("game_result.dot", "w");

    fprintf(output, "digraph G {\n");
}

Dotter::~Dotter() {
    fprintf(output, "}\n");
    fclose(output);
}

void Dotter::ProcessGuessTree(const GuessTree& guess_tree) {
    ProcessGuessTreeNode(guess_tree, 0);
}

void Dotter::ProcessGuessTreeNode(const GuessTree& guess_tree, size_t node_number) {
    if (guess_tree.IsLeaf(node_number)) {
        fprintf(output, "    N%d [label=\"%s\"]\n",
                        node_number,
                        guess_tree.GetNode(node_number).value);
    
    } else {
        fprintf(output, "    N%d [shape=diamond, label=\"%s\"]\n",
                        node_number,
                        guess_tree.GetNode(node_number).value);

        ProcessGuessTreeNode(guess_tree, guess_tree.GetNode(node_number).left_child);
        ProcessGuessTreeNode(guess_tree, guess_tree.GetNode(node_number).right_child);

        fprintf(output, "    N%d -> N%d [color=\"#ff7e40\", fontcolor=\"#ff7e40\", label=\"No\"]\n",
                        node_number,
                        guess_tree.GetNode(node_number).left_child);

        fprintf(output, "    N%d -> N%d [color=\"#228b22\", fontcolor=\"#228b22\", label=\"Yes\"]\n",
                        node_number,
                        guess_tree.GetNode(node_number).right_child);
    }
}

#endif
