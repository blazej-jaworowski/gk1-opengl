#include "ObjLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::tuple<int, int, int> read_index_triple(std::stringstream &stream) {
    int i = 0;
    int ti = 0;
    int ni = 0;
    stream >> i;
    if (stream.peek() == '/') {
        stream.get();
        if (stream.peek() == '/') {
            stream.get();
            stream >> ni;
        } else {
            stream >> ti;
            if (stream.peek() == '/') {
                stream.get();
                stream >> ni;
            }
        }
    }

    return {i, ti, ni};
}

ObjLoader::ObjLoader(std::string filename) {
    std::ifstream file(filename);
    if (file.fail()) {
        std::cerr << "ERROR: File " << filename << " not found.\n";
    }

    std::stringstream ss;
    ss << file.rdbuf();

    while (ss.rdbuf()->in_avail() != 0) {
        std::string word;
        ss >> word;
        if (word == "v") {
            float v1, v2, v3, v4 = 1.0f;
            ss >> v1;
            ss >> v2;
            ss >> v3;
            if (isdigit(ss.peek()))
                ss >> v4;
            vertices.emplace_back(v1, v2, v3);
        } else if (word == "f") {
            auto [i1, ti1, ni1] = read_index_triple(ss);
            auto [i2, ti2, ni2] = read_index_triple(ss);
            auto [i3, ti3, ni3] = read_index_triple(ss);

            i1 = i1 < 0 ? vertices.size() + i1 + 1 : i1;
            i2 = i2 < 0 ? vertices.size() + i2 + 1 : i2;
            i3 = i3 < 0 ? vertices.size() + i3 + 1 : i3;

            faces.emplace_back(i1 - 1, i2 - 1, i3 - 1);
        }
    }
}