#include "Loader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

Mesh Loader::LoadOBJ(std::string path, Renderer& r)
{
    Mesh m = Mesh();
    std::vector<vec3> vertices;
    std::vector<vec2> uvs;
    std::vector<vec3> normals;
    std::vector<uint32_t> vertexIndices;
    std::vector<uint32_t> uvIndices;
    std::vector<uint32_t> normalIndices;

    FILE* file = fopen(path.c_str(), "r");
    if (file == nullptr) {
        //error
        return m;
    }

    while (1) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if (strcmp(lineHeader, "v") == 0) {
            vec3 v;
            fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z);
            vertices.push_back(v);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uvs.push_back(uv);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            vec3 n;
            fscanf(file, "%f %f %f\n", &n.x, &n.y, &n.z);
            normals.push_back(n);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            std::string v1, v2, v3;
            uint32_t vIndex[3], uvIndex[3], nIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                &vIndex[0], &uvIndex[0], &nIndex[0],
                &vIndex[1], &uvIndex[1], &nIndex[1],
                &vIndex[2], &uvIndex[2], &nIndex[2]);
            if (matches != 0) {
                // error
                return m;
            }
            vertexIndices.push_back(vIndex[0] - 1);
            vertexIndices.push_back(vIndex[1] - 1);
            vertexIndices.push_back(vIndex[2] - 1);
            uvIndices.push_back(uvIndex[0] - 1);
            uvIndices.push_back(uvIndex[1] - 1);
            uvIndices.push_back(uvIndex[2] - 1);
            normalIndices.push_back(nIndex[0] - 1);
            normalIndices.push_back(nIndex[1] - 1);
            normalIndices.push_back(nIndex[2] - 1);
        }



        //r.CreateVertexBuffer()
    }

    return Mesh();
}
