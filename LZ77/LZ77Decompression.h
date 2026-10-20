#ifndef LZ77_DECOMPRESSION_H
#define LZ77_DECOMPRESSION_H

#include <bits/stdc++.h>
#include "LZ77Element.h"
using namespace std;



class LZ77Decompression {
private:
    string fileName {};

    void readElements(const string& compressedFileName, vector<LZ77Element>& elements) {

        ifstream compressedFile(compressedFileName);

        if (!compressedFile.is_open()) {
            cout << "Could not open compressed file!" << endl;
        }

        int position;
        int length;
        int nextCharCode;

        while (compressedFile >> position >> length >> nextCharCode) {

            char nextChar = static_cast<char>(nextCharCode);

            elements.emplace_back(position, length, nextChar);
        }

        compressedFile.close();
    }

    void storeDecompressedData(const string& outputFileName, const string& contentOutput) {
        ofstream outputFile(outputFileName);

        if (!outputFile.is_open()) {
            cout << "Failed to create the decompressed file!" << endl;
            return;
        }

        outputFile << contentOutput;

        if (!outputFile) {
            cout << "Failed to write the decompressed data!" << endl;
        }
    }
public:
    LZ77Decompression(string fileName) : fileName(fileName) {}

    ~LZ77Decompression() {}

    string getFileName() const {
        return fileName;
    }

    void setFileName(const string& fileName) {
        this->fileName = fileName;
    }

    void decompress(const string& outputFileName) {
        assert(!fileName.empty());

        vector<LZ77Element> elements;

        readElements(fileName, elements);

        string contentOutput = "";

        for (auto& element : elements) {
            int startPosition = (int)contentOutput.size() - element.position;

            for (int i = 0; i < element.length; ++i) {
                contentOutput += contentOutput[startPosition + i];
            }

            if (element.nextChar != '\0') contentOutput += element.nextChar;
        }

        storeDecompressedData(outputFileName, contentOutput);
    }




};

#endif
