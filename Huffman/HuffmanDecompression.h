#ifndef HUFFMAN_DECOMPRESSION_H
#define HUFFMAN_DECOMPRESSION_H

#include <bits/stdc++.h>
using namespace std;

class HuffmanDecompression {
private:
    string codeFileName {};
    string tableFileName {};

    bool readFile(string& fileContent) {

        fileContent = "";

        ifstream fileStream(codeFileName, ios::binary);

        if (!fileStream.is_open()) {
            return false;
        }

        fileContent.assign(
        istreambuf_iterator<char>(fileStream),
        istreambuf_iterator<char>()
        );

        return true;
    }

    bool readCharCodeTable(unordered_map<string, char>& table) {
        ifstream tableFile(tableFileName, ios::binary);

        if (!tableFile.is_open()) {
            return false;
        }

        size_t numberOfCharacters;

        if (!(tableFile >> numberOfCharacters)) {
            return false;
        }

        unordered_map<string, char> tempTable;

        for (size_t i = 0; i < numberOfCharacters; ++i) {
            unsigned int characterValue;
            string code;

            if (!(tableFile >> characterValue >> code)) {
                return false;
            }

            // Validate that the value represents a valid byte.
            if (characterValue > numeric_limits<unsigned char>::max()) {
                return false;
            }

            // Validate that the Huffman code contains only 0s and 1s.
            if (code.find_first_not_of("01") != string::npos) {
                return false;
            }

            char character = static_cast<char>(
                static_cast<unsigned char>(characterValue)
            );

            // Reject duplicate Huffman codes.
            if (!tempTable.emplace(code, character).second) {
                return false;
            }
        }

        table = move(tempTable);

        return true;
    }

    void runDecompression(string& decompressedContent , const string& codeContent , unordered_map<string, char>& table) {
        string currentCode = "";
        for (int i = 0 ; i < codeContent.size(); ++i) {
            currentCode += codeContent[i];
            if (table.find(currentCode) != table.end()) {
                decompressedContent += table[currentCode];
                currentCode = "";
            }
        }
    }

    bool storeDecompressedData(const string& outputFileName, const string& contentOutput) {
        ofstream outputFile(
            outputFileName,
            ios::binary | ios::out | ios::trunc
        );

        if (!outputFile.is_open()) {
            return false;
        }

        outputFile.write(
            contentOutput.data(),
            static_cast<streamsize>(contentOutput.size())
        );

        return outputFile.good();
    }
public:
    HuffmanDecompression(){}
    HuffmanDecompression(const string& codeFileName , const string& tableFileName) : codeFileName(codeFileName), tableFileName(tableFileName) {}

    void setCodeFileName(const string& codeFileName) { this->codeFileName = codeFileName; }
    void setTableFileName(const string& tableFileName) { this->tableFileName = tableFileName; }
    string getCodeFileName() { return codeFileName; }
    string getTableFileName() { return tableFileName; }

    void decompress(const string& outputFileName) {
        if (codeFileName.empty() || tableFileName.empty()) {
            cerr << "Input file name is empty!" << endl;
            return;
        }

        unordered_map<string, char>table;

        string codeContent;

        if (!readFile(codeContent)) {
            cerr << "Error reading code content!" << endl;
            return;
        }

        if (!readCharCodeTable(table)) {
            cerr << "Error reading code table!" << endl;
            return;
        }

        string decompressedContent = "";

        runDecompression(decompressedContent, codeContent, table);

        storeDecompressedData(outputFileName, decompressedContent);
    }

};

#endif
