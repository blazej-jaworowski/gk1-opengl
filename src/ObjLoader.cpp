#include "ObjLoader.h"

#include <fstream>
#include <iostream>
#include <map>
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

    std::vector<std::tuple<float, float, float>> vertex_vector;
    std::vector<std::tuple<float, float, float>> normal_vector;
    std::vector<std::tuple<int, int, int>> face_vector;
    std::map<int, int> normal_indices;

    std::string line;
    while (std::getline(ss, line)) {
        if (line[0] == '#')
            continue;
        std::stringstream line_stream(line);
        std::string word;
        line_stream >> word;
        if (word == "v") {
            float v1, v2, v3, v4 = 1.0f;
            line_stream >> v1;
            line_stream >> v2;
            line_stream >> v3;
            if (isdigit(line_stream.peek()))
                line_stream >> v4;
            vertex_vector.emplace_back(v1, v2, v3);
        } else if (word == "vn") {
            float n1, n2, n3;
            line_stream >> n1;
            line_stream >> n2;
            line_stream >> n3;
            normal_vector.emplace_back(n1, n2, n3);
        } else if (word == "f") {
            auto [i1, ti1, ni1] = read_index_triple(line_stream);
            auto [i2, ti2, ni2] = read_index_triple(line_stream);
            auto [i3, ti3, ni3] = read_index_triple(line_stream);
            uint32_t i4 = 0;
            uint32_t ti4 = 0;
            uint32_t ni4 = 0;
            if (line_stream.peek() == ' ') {
                std::tie(i4, ti4, ni4) = read_index_triple(line_stream);
            }

            i1 = i1 < 0 ? vertex_vector.size() + i1 + 1 : i1;
            i2 = i2 < 0 ? vertex_vector.size() + i2 + 1 : i2;
            i3 = i3 < 0 ? vertex_vector.size() + i3 + 1 : i3;
            i4 = i4 < 0 ? vertex_vector.size() + i4 + 1 : i4;

            face_vector.emplace_back(i1 - 1, i2 - 1, i3 - 1);

            if (i4 != 0) {
                face_vector.emplace_back(i1 - 1, i4 - 1, i3 - 1);
            }

            if (ni1 == 0 && ni2 == 0 && ni3 == 0)
                continue;

            ni1 = ni1 < 0 ? vertex_vector.size() + ni1 + 1 : ni1;
            ni2 = ni2 < 0 ? vertex_vector.size() + ni2 + 1 : ni2;
            ni3 = ni3 < 0 ? vertex_vector.size() + ni3 + 1 : ni3;

            normal_indices[i1 - 1] = ni1 - 1;
            normal_indices[i2 - 1] = ni2 - 1;
            normal_indices[i3 - 1] = ni3 - 1;

            if (ni4 == 0)
                continue;
            ni4 = ni4 < 0 ? vertex_vector.size() + ni4 + 1 : ni4;
            normal_indices[i4 - 1] = ni4 - 1;
        }
    }

    vertex_count = vertex_vector.size();
    face_count = face_vector.size();

    vertices = new float[vertex_vector.size() * 3];
    normals = new float[vertex_vector.size() * 3];
    faces = new uint32_t[face_vector.size() * 3];

    int i = 0;
    for (const auto &vertex : vertex_vector) {
        std::tie(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2]) =
            vertex;

        if (normal_indices.find(i) == normal_indices.end()) {
            normals[3 * i] = 0;
            normals[3 * i + 1] = 0;
            normals[3 * i + 2] = 1;
        } else {
            std::tie(normals[3 * i], normals[3 * i + 1], normals[3 * i + 2]) =
                normal_vector.at(normal_indices.at(i));
        }
        i++;
    }
    i = 0;
    for (const auto &face : face_vector) {
        std::tie(faces[i], faces[i + 1], faces[i + 2]) = face;
        i += 3;
    }
}

ObjLoader::~ObjLoader() {
    delete[] vertices;
    delete[] normals;
    delete[] faces;
}