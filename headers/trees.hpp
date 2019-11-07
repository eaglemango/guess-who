/*!
    @file
    @brief Binary and Guess trees
*/
#ifndef TREES_HEADER
#define TREES_HEADER

#include <cassert>
#include <cstdio>

#include "constants.hpp"
#include "dynamic_buffer.hpp"

template <class T>
struct BinaryTreeNode {
    BinaryTreeNode() {
        value = T();

        parent = 0;
        left_child = 0;
        right_child = 0;
    }

    BinaryTreeNode(size_t parent, T value) : parent(parent), value(value) {
        left_child = 0;
        right_child = 0;
    }

    T value;

    size_t parent;
    size_t left_child;
    size_t right_child;
};

template <class T>
class BinaryTree {
public:
    BinaryTree() = default;

    ~BinaryTree() = default;
    BinaryTree(const BinaryTree& binary_tree) = default;
    BinaryTree& operator=(const BinaryTree& binary_tree) = default;

    void AddNode(size_t parent, const T& value);

    void UpdateNode(size_t node_number, const T& new_value);

    BinaryTreeNode<T> GetNode(size_t node_number) const;
    size_t GetLeftChild(size_t node_number) const;
    size_t GetRightChild(size_t node_number) const;

    bool IsLeaf(size_t node_number) const;
    bool IsLeaf(const BinaryTreeNode<T>& node) const;

private:
    DynamicBuffer<BinaryTreeNode<T>> buffer;
};

template <class T>
void BinaryTree<T>::AddNode(size_t parent, const T& value) {
    BinaryTreeNode<char*> new_node = BinaryTreeNode<char*>(parent, value);
    buffer.Add(new_node);

    if (buffer[parent].left_child == 0) {
        buffer[parent].left_child = buffer.GetCurrSize() - 1;

    } else if (buffer[parent].right_child == 0) {
        buffer[parent].right_child = buffer.GetCurrSize() - 1;

    } else {
        assert(false);
    }
}

template <class T>
void BinaryTree<T>::UpdateNode(size_t node_number, const T& new_value) {
    buffer[node_number].value = new_value;
}

template <class T>
BinaryTreeNode<T> BinaryTree<T>::GetNode(size_t node_number) const {
    return buffer[node_number];
}

template <class T>
size_t BinaryTree<T>::GetLeftChild(size_t node_number) const {
    assert(!IsLeaf(node_number));

    return buffer[node_number].left_child;
}

template <class T>
size_t BinaryTree<T>::GetRightChild(size_t node_number) const {
    assert(!IsLeaf(node_number));

    return buffer[node_number].right_child;
}

template <class T>
bool BinaryTree<T>::IsLeaf(size_t node_number) const {
    return buffer[node_number].left_child == 0 &&
           buffer[node_number].right_child == 0;
}

template <class T>
bool BinaryTree<T>::IsLeaf(const BinaryTreeNode<T>& node) const {
    return node.left_child == 0 &&
           node.right_child == 0;
}

// ===================================================

class GuessTree : public BinaryTree<char*> {
public:
    GuessTree();

    ~GuessTree() = default;
    GuessTree(const GuessTree& guess_tree) = default;
    GuessTree& operator=(const GuessTree& guess_tree) = default;

    void AskQuestions();

private:
    size_t AskQuestion(size_t node_number) const;
    bool Guess(size_t node_number) const;
    void Train(size_t node_number);
};

GuessTree::GuessTree() {
    AddNode(0, "Nobody");
}

void GuessTree::AskQuestions() {
    size_t curr_node_number = 0;

    while (!IsLeaf(curr_node_number)) {
        curr_node_number = AskQuestion(curr_node_number);
    }

    bool guesser_won = Guess(curr_node_number);

    if (!guesser_won) {
        Train(curr_node_number);
    }
}

size_t GuessTree::AskQuestion(size_t node_number) const {
    printf("%s\n", GetNode(node_number).value);

    char response = fgetc(stdin);
    while (response == '\n') {
        response = fgetc(stdin);
    }

    if (response == RESPONSE_YES) {
        return GetNode(node_number).right_child;

    } else if (response == REPONSE_NO) {
        return GetNode(node_number).left_child;

    } else {
        assert(false);
    }
}

bool GuessTree::Guess(size_t node_number) const {
    printf("Is this %s?\n", GetNode(node_number).value);

    char response = fgetc(stdin);
    while (response == '\n') {
        response = fgetc(stdin);
    }

    if (response == RESPONSE_YES) {
        printf("I won again!\n");

        return true;

    } else if (response == REPONSE_NO) {
        printf("Oh, that's your day...\n");

        return false;

    } else {
        printf("Symbol: %d", (int)response);
        assert(false);
    }
}

void ReadString(DynamicBuffer<char>& s) {
    char c = fgetc(stdin);
    while (c == '\n') {
        c = fgetc(stdin);
    }

    while (c != '\n') {
        s.Add(c);

        c = fgetc(stdin);
    }
}

void GuessTree::Train(size_t node_number) {
    DynamicBuffer<char> new_answer_buffer;

    printf("Who it was?\n");
    ReadString(new_answer_buffer);

    char* new_answer = (char*)calloc(new_answer_buffer.GetCurrSize(), sizeof(char));
    assert(new_answer);

    for (size_t i = 0; i < new_answer_buffer.GetCurrSize(); ++i) {
        new_answer[i] = new_answer_buffer[i];
    }

    DynamicBuffer<char> new_question_buffer;

    printf("Please, ask a question, that is NO for %s and YES for %s\n", 
           GetNode(node_number).value,
           new_answer);
    ReadString(new_question_buffer);

    char* new_question = (char*)calloc(new_question_buffer.GetCurrSize(), sizeof(char));
    assert(new_question);
    
    for (size_t i = 0; i < new_question_buffer.GetCurrSize(); ++i) {
        new_question[i] = new_question_buffer[i];
    }

    AddNode(node_number, GetNode(node_number).value); // new left node
    AddNode(node_number, new_answer); // new right node
    
    UpdateNode(node_number, new_question);
}
#endif
