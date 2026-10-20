#ifndef LZ77_COMPRESSION_H
#define LZ77_COMPRESSION_H

#include <bits/stdc++.h>
#include "LZ77Element.h"
using namespace std;

class LZ77Compression {
private:
    const int prevWindowLength = 20;
    const int lookHeadWindowLength = 20;
    string fileName {};
    void readFile(string& fileContent) {

        fileContent = "";

        ifstream fileStream(fileName);

        if (!fileStream.is_open()) {
            cout << "Filed to open the file!" << endl;
            return;
        }

        fileContent.assign(
        istreambuf_iterator<char>(fileStream),
        istreambuf_iterator<char>()
        );
    }
    pair<int , int> findLongestMatch(const string& fileContent , int i) {
        int size = (int) fileContent.size();

        int bestDistance = 0;
        int bestLength = 0;

        int startWindow = max(0, i - prevWindowLength);
        int lookAheadEnd = min(size, i + lookHeadWindowLength);

        for (int j = startWindow; j < i; j++) {
            int length = 0;

            while (i + length < lookAheadEnd && fileContent[i + length] == fileContent[j+length]) {
                length++;
            }

            if (length > bestLength) {
                bestLength = length;
                bestDistance = i - j;
            }
        }
        return {bestDistance , bestLength};
    }
    void storeTheNewCompressedData(const string& compressedFileName , vector<LZ77Element>& dictionaryVector) {
        ofstream outputFile(compressedFileName);

        if (!outputFile.is_open()) {
            cout << "Failed to create the compressed file!" << endl;
            return;
        }

        for (const LZ77Element& token : dictionaryVector) {
            outputFile
                << token.position << ' '
                << token.length << ' '
                << static_cast<int>(
                       static_cast<unsigned char>(token.nextChar)
                   )
                << '\n';
        }
    }
public:
    LZ77Compression() {
    }

    LZ77Compression(const string& fileName) : fileName(fileName) {}

    string getFileName() const {
        return fileName;
    }

    void setFileName(const string& fileName) {
        this->fileName = fileName;
    }

    void compress(const string& outputFileName) {
        assert(!fileName.empty());

        string fileContent;

        readFile(fileContent);

        vector<LZ77Element> dictionaryVector;

        int i = 0;

        while (i < fileContent.size()) {
            auto [bestDistance, bestLength] = findLongestMatch(fileContent, i);
            int nextIndex = i + bestLength;
            if (nextIndex < fileContent.size()) {
                dictionaryVector.push_back(LZ77Element(bestDistance , bestLength , fileContent[nextIndex]));
            }else {
                dictionaryVector.push_back(LZ77Element(bestDistance , bestLength , '\0'));
            }

            i = nextIndex + 1;
        }
        storeTheNewCompressedData(outputFileName, dictionaryVector);
    }


};

#endif
