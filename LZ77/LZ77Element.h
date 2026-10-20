#ifndef ELEMENT_H
#define ELEMENT_H

struct LZ77Element {
    int position {};
    int length {};
    char nextChar {};

    LZ77Element(int position, int length, char nextChar) {
        this->position = position;
        this->length = length;
        this->nextChar = nextChar;
    }

    LZ77Element() {}
};

#endif
