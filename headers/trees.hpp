/*!
    @file
    @brief Binary and Guess trees
*/
#ifndef TREES_HEADER
#define TREES_HEADER

#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstring>

#include "constants.hpp"
#include "dynamic_buffer.hpp"
#include "utils.hpp"

template <class T>
struct BinaryTreeNode {
    BinaryTreeNode() {
        value = T();

        depth = -1;
        parent = 0;
        left_child = 0;
        right_child = 0;
    }

    BinaryTreeNode(size_t parent, T value) : parent(parent), value(value) {
        depth = -1;
        left_child = 0;
        right_child = 0;
    }

    T value;

    size_t depth;
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

    size_t AddNode(size_t parent, const T& value);

    void UpdateNode(size_t node_number, const T& new_value);

    BinaryTreeNode<T> GetNode(size_t node_number) const;
    size_t GetLeftChild(size_t node_number) const;
    size_t GetRightChild(size_t node_number) const;

    bool IsLeaf(size_t node_number) const;
    bool IsLeaf(const BinaryTreeNode<T>& node) const;

    bool IsFree(size_t node_number) {
        return buffer[node_number].left_child == 0 ||
               buffer[node_number].right_child == 0;
    }

private:
    DynamicBuffer<BinaryTreeNode<T>> buffer;
};

template <class T>
size_t BinaryTree<T>::AddNode(size_t parent, const T& value) {
    BinaryTreeNode<char*> new_node = BinaryTreeNode<char*>(parent, value);
    new_node.depth = buffer[parent].depth + 1;
    buffer.Add(new_node);

    if (buffer[parent].left_child == 0) {
        buffer[parent].left_child = buffer.GetCurrSize() - 1;

    } else if (buffer[parent].right_child == 0) {
        buffer[parent].right_child = buffer.GetCurrSize() - 1;

    } else {
        assert(false);
    }

    return buffer.GetCurrSize() - 1;
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
    GuessTree(const char* file_path);

    ~GuessTree();
    GuessTree(const GuessTree& guess_tree) = default;
    GuessTree& operator=(const GuessTree& guess_tree) = default;

    void AskQuestions();

    void WhoIs(size_t curr_node, const char* node_value, bool& found);

private:
    size_t AskQuestion(size_t node_number) const;
    bool Guess(size_t node_number) const;
    void Train(size_t node_number);

    void Load(const char* file_path);
    void Save(const char* file_path);

    void SafeWrite(const char* s, FILE* file);
    void WriteNode(size_t node_number, FILE* file);

    void FestivalAskQuestion(size_t node_number) const;
    void FestivalGuess(size_t node_number) const;

    void FestivalSay(char* words) const;
};

GuessTree::GuessTree(const char* file_path = EMPTY_STRING) {
    if (strlen(file_path)) {
        Load(file_path);

    } else {
        AddNode(0, PLACEHOLDER);
    }
}

GuessTree::~GuessTree() {
    Save("kek.gg");
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

void GuessTree::WhoIs(size_t curr_node, const char* node_value, bool& found) {
    if (!found) {
        if (IsLeaf(curr_node)) {
            if (!strcmp(GetNode(curr_node).value, node_value)) {
                found = true;

                while (curr_node != 0) {
                    if (GetNode(GetNode(curr_node).parent).right_child == curr_node) {
                        curr_node = GetNode(curr_node).parent;

                        printf("%s - YES!\n", GetNode(curr_node).value);

                    } else {
                        curr_node = GetNode(curr_node).parent;
                    }
                }
            }

        } else {
            WhoIs(GetNode(curr_node).left_child, node_value, found);
            WhoIs(GetNode(curr_node).right_child, node_value, found);
        }

    }
}

size_t GuessTree::AskQuestion(size_t node_number) const {
    // FestivalAskQuestion(node_number);
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
    // FestivalGuess(node_number);
    printf("Is this %s?\n", GetNode(node_number).value);

    char response = fgetc(stdin);
    while (response == '\n') {
        response = fgetc(stdin);
    }

    if (response == RESPONSE_YES) {
        // FestivalSay(WIN_WORDS);
        printf("I won again!\n");

        return true;

    } else if (response == REPONSE_NO) {
        // FestivalSay(LOSE_WORDS);
        printf("Oh, that's your day...\n");

        return false;

    } else {
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

    // FestivalSay(WHO_WORDS);
    printf("Who it was?\n");
    ReadString(new_answer_buffer);

    char* new_answer = nullptr;
    new_answer_buffer.CopyToArray(&new_answer);

    DynamicBuffer<char> new_question_buffer;

    printf("Please, ask a question, that is NO for %s and YES for %s\n", 
           GetNode(node_number).value,
           new_answer);
    ReadString(new_question_buffer);

    char* new_question = nullptr;
    new_question_buffer.CopyToArray(&new_question);

    AddNode(node_number, GetNode(node_number).value); // new left node
    AddNode(node_number, new_answer); // new right node
    
    UpdateNode(node_number, new_question);
}

void GuessTree::Load(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    assert(file);

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* code = (char*) calloc(size, sizeof(char));
    assert(code);

    int read_count = fread(code, sizeof(char), size, file);
    assert(read_count == size);

    int close_result = fclose(file);
    assert(close_result != EOF);

    // Processing
    size_t last_node = 0;
    size_t curr_parent = 0;
    char* token = strtok(code, DELIMITER);
    while (token) {
        while (isspace(*token)) {
            ++token;
        }

        if (*token == '\0') {
            token = strtok(nullptr, DELIMITER);

            continue;
        }

        if (*token == '{') { // Curr node becomes a parent
            curr_parent = last_node;

        } else if (*token == '}') { // Return control to parent
            curr_parent = GetNode(curr_parent).parent;

        } else { // Just add string to current parent
            while (!IsFree(curr_parent)) {
                curr_parent = GetNode(curr_parent).parent;
            }

            last_node = AddNode(curr_parent, token);
        }

        token = strtok(nullptr, DELIMITER);
    }
}

void GuessTree::SafeWrite(const char* s, FILE* file) {
    int write_count = fwrite(s, sizeof(char), strlen(s), file);
    assert(write_count == strlen(s));
}

void GuessTree::WriteNode(size_t node_number, FILE* file) {
    char* spaces = GenerateSpaces(GetNode(node_number).depth * 2);
    assert(spaces);

    if (IsLeaf(node_number)) {
        SafeWrite(spaces, file);
        SafeWrite(GetNode(node_number).value, file);
        SafeWrite(DELIMITER, file);

        SafeWrite(spaces, file);
        SafeWrite(OPENING_BRACE, file);
        SafeWrite(DELIMITER, file);

    } else {
        SafeWrite(spaces, file);
        SafeWrite(GetNode(node_number).value, file);
        SafeWrite(DELIMITER, file);

        SafeWrite(spaces, file);
        SafeWrite(OPENING_BRACE, file);
        SafeWrite(DELIMITER, file);

        WriteNode(GetNode(node_number).left_child, file);
        
        SafeWrite(spaces, file);
        SafeWrite(CLOSING_BRACE, file);
        SafeWrite(DELIMITER, file);

        WriteNode(GetNode(node_number).right_child, file);
    }

    free(spaces);
}

void GuessTree::Save(const char* file_path) {
    FILE* file = fopen(file_path, "w");
    assert(file);

    SafeWrite(OPENING_BRACE, file);
    SafeWrite(DELIMITER, file);

    WriteNode(0, file);

    SafeWrite(CLOSING_BRACE, file);
    SafeWrite(DELIMITER, file);

    int close_result = fclose(file);
    assert(close_result != EOF);
}

void GuessTree::FestivalAskQuestion(size_t node_number) const {
    char* question_string = (char*)calloc(1 + strlen(GetNode(node_number).value), sizeof(char));
    sprintf(question_string, "%s\n", GetNode(node_number).value);

    char* command_string = (char*)calloc(24 + strlen(question_string), sizeof(char));
    sprintf(command_string, "echo \"%s\" | festival --tts", question_string);

    system(command_string);

    free(command_string);
    free(question_string);
}

void GuessTree::FestivalGuess(size_t node_number) const {
    char* answer_string = (char*)calloc(1 + strlen(GetNode(node_number).value), sizeof(char));
    sprintf(answer_string, "%s\n", GetNode(node_number).value);

    char* command_string = (char*)calloc(32 + strlen(answer_string), sizeof(char));
    sprintf(command_string, "echo \"Is this %s\" | festival --tts", answer_string);

    system(command_string);

    free(command_string);
    free(answer_string);
}

void GuessTree::FestivalSay(char* words) const {
    char* command_string = (char*)calloc(24 + strlen(words), sizeof(char));
    sprintf(command_string, "echo \"%s\" | festival --tts", words);

    system(command_string);

    free(command_string);
}

#endif
