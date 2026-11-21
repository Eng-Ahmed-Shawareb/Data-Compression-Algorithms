#ifndef MODIFIED_HUFFMAN_COMPRESSION_H
#define MODIFIED_HUFFMAN_COMPRESSION_H

#include <bits/stdc++.h>
using namespace std;

struct CharNode {
    char c {};
    double probability {};
    CharNode* left {};
    CharNode* right {};
};

struct CompareNodes {
    bool operator()(const CharNode* a, const CharNode* b) const {
        return a->probability > b->probability;
    }
};

class HuffmanCompression {
private:
    string fileName {};

    bool readFile(string& fileContent) {

        fileContent = "";

        ifstream fileStream(fileName);

        if (!fileStream.is_open()) {
            return false;
        }

        fileContent.assign(
        istreambuf_iterator<char>(fileStream),
        istreambuf_iterator<char>()
        );

        return true;
    }

    bool storeContentCode(const string& code, const string& outputFileName) {
        ofstream outputFile(outputFileName, ios::binary | ios::out | ios::trunc);

        if (!outputFile.is_open()) {
            return false;
        }

        outputFile.write(code.data(), static_cast<streamsize>(code.size()));

        return outputFile.good();
    }

    bool storeCharCodeTable(const unordered_map<char, string>& table, const string& tableFileName) {
        ofstream tableFile(tableFileName, ios::binary | ios::out | ios::trunc);

        if (!tableFile.is_open()) {
            return false;
        }

        // Store the number of characters in the table.
        tableFile << table.size() << '\n';

        for (const auto& [character, code] : table) {
            // Store the character as an unsigned numeric byte value.
            tableFile << static_cast<unsigned int>(
                static_cast<unsigned char>(character)
            ) << ' ' << code << '\n';
        }

        return tableFile.good();
    }

    CharNode* buildTree(priority_queue<CharNode*, vector<CharNode*>, CompareNodes>&pq) {
        if (pq.empty()) {
            return nullptr;
        }
        while (pq.size() > 1) {
            CharNode* node1 = pq.top();
            pq.pop();
            CharNode* node2 = pq.top();
            pq.pop();
            CharNode* new_node = new CharNode();
            new_node->probability = node1->probability + node2->probability;
            new_node->left = node1;
            new_node->right = node2;
            pq.push(new_node);
        }
        return pq.top();
    }

    void assignCode(CharNode* root, string currentCode , unordered_map<char , string>&table) {
        if (root == nullptr) {
            return;
        }
        if (root->left == nullptr && root->right == nullptr) {
            table[root->c] = currentCode.empty() ? "0" : currentCode;
            delete root;
            return;
        }
        assignCode(root->left, currentCode + "0" , table);
        assignCode(root->right, currentCode + "1" , table);
        delete root;
    }

    string finalContentCode(const string& fileContent , unordered_map<char , string>&table) {
        string code = "";
        for (auto& i : fileContent) {
            code += table[i];
        }
        return code;
    }

public:
    HuffmanCompression(){}
    HuffmanCompression(const string& fileName) : fileName(fileName) {}

    string getFileName() const {
        return fileName;
    }

    void setFileName(const string& fileName) {
        this->fileName = fileName;
    }

    void compress(const string& outputFileName, const string& tableFileName) {
        if (fileName.empty()) {
            cerr << "Input file name is empty!" << endl;
            return;
        }

        string fileContent;

        if (!readFile(fileContent)) {
            cerr << "Failed to read the input file!" << endl;
            return;
        }
        unordered_map<char , string>table;
        unordered_map<char , int> char_frequency;

        for (auto& i : fileContent) {
            char_frequency[i]++;
        }

        priority_queue<CharNode*, vector<CharNode*>, CompareNodes> pq;

        for (auto& i : char_frequency) {
            CharNode* node = new CharNode();
            node->c = i.first;
            node->probability = (double)((double)i.second / (double)fileContent.size());
            pq.push(node);
        }

        CharNode* root = buildTree(pq);
        assignCode(root, "", table);

        string code = finalContentCode(fileContent, table);

        if (!storeContentCode(code, outputFileName)) {
            cerr << "Failed to store compressed content!" << endl;
            return;
        }

        if (!storeCharCodeTable(table, tableFileName)) {
            cerr << "Failed to store the Huffman table!" << endl;
            return;
        }
    }
};

#endif
