#include "Loader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

enum class OBJLoadState {
    None,
    Vertices,
    Uvs,
    Normals,
    Faces
};

Mesh SendToGPU(std::vector<Vertex> &vertexBuffer, std::vector<uint32_t> &vertexIndices, Renderer& r) 
{
    Mesh m = Mesh();
    m.vertexBuffer = r.CreateVertexBuffer(&vertexBuffer[0], vertexBuffer.size());
    m.indexBuffer = r.CreateIndexBuffer(&vertexIndices[0], vertexIndices.size());
    m.numIndices = vertexIndices.size();
    return m;
}

Mesh ConvertVertexBuffer(std::vector<vec3>& vertices, std::vector<uint32_t>&vertexIndices, std::vector<vec2>& uvs, std::vector<uint32_t> &uvIndices, Renderer& r) {
    std::vector<Vertex> vertexBuffer;
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        Vertex v;
        v.position[0] = vertices[i].x;
        v.position[1] = vertices[i].y;
        v.position[2] = vertices[i].z;

        vertexBuffer.push_back(v);
    }

    for (size_t i = 0; i < vertexIndices.size(); ++i)
    {
        vertexBuffer[vertexIndices[i]].uv[0] = uvs[uvIndices[i]].x;
        vertexBuffer[vertexIndices[i]].uv[1] = uvs[uvIndices[i]].y;
        
        /*vertexBuffer[vertexIndices[i]].normal[0] = normals[normalIndices[i]].x;
        vertexBuffer[vertexIndices[i]].normal[1] = normals[normalIndices[i]].y;
        vertexBuffer[vertexIndices[i]].normal[2] = normals[normalIndices[i]].z;*/
        
    }
    return SendToGPU(vertexBuffer, vertexIndices, r);
}

Mesh Loader::LoadOBJ(std::string path, Renderer& r)
{
    Mesh m = Mesh();
    std::vector<vec3> vertices;
    std::vector<vec2> uvs;
    std::vector<vec3> normals;
    std::vector<uint32_t> vertexIndices;
    std::vector<uint32_t> uvIndices;
    std::vector<uint32_t> normalIndices;

    std::string line;
    std::ifstream myfile(path);
    
    OBJLoadState state = OBJLoadState::None;
    if (myfile.is_open())
    {
        while (getline(myfile, line)) {
            if (line[0] == 'v' && line[1] == ' ' ) {
                if (state != OBJLoadState::Faces) {
                    vec3 v;
                    int res = sscanf(line.c_str(), " %*c %f %f %f\n", &v.x, &v.y, &v.z);
                    vertices.push_back(v);
                    state = OBJLoadState::Vertices;
                }
                /*else if(state == OBJLoadState::Faces)
                {
                    return ConvertVertexBuffer(vertices, vertexIndices, uvs, uvIndices, r);
                }*/
            }

            if (line[0] == 'v' && line[1] == 't')
            {
                vec2 uv;
                int res = sscanf(line.c_str(), " %*s %f %f\n", &uv.x, &uv.y);
                uvs.push_back(uv);
                state = OBJLoadState::Uvs;
            }

            if (line[0] == 'v' && line[1] == 'n')
            {
                vec3 n;
                int res = sscanf(line.c_str(), " %*s %f %f %f\n", &n.x, &n.y, &n.z);
                normals.push_back(n);
                state = OBJLoadState::Normals;
            }

            if (line[0] == 'f') 
            {
                unsigned int vIndex[3], uvIndex[3], nIndex[3];
                if (state == OBJLoadState::Normals) 
                {
                    int res = sscanf(line.c_str(), " %*c %u/%u/%u %u/%u/%u %u/%u/%u\n",
                        &vIndex[0], &uvIndex[0], &nIndex[0],
                        &vIndex[1], &uvIndex[1], &nIndex[1],
                        &vIndex[2], &uvIndex[2], &nIndex[2]);
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
                else
                {
                    int res = sscanf(line.c_str(), " %*c %u/%u %u/%u %u/%u\n",
                        &vIndex[0], &uvIndex[0], 
                        &vIndex[1], &uvIndex[1],
                        &vIndex[2], &uvIndex[2]);
                    vertexIndices.push_back(vIndex[0] - 1);
                    vertexIndices.push_back(vIndex[1] - 1);
                    vertexIndices.push_back(vIndex[2] - 1);
                    uvIndices.push_back(uvIndex[0] - 1);
                    uvIndices.push_back(uvIndex[1] - 1);
                    uvIndices.push_back(uvIndex[2] - 1);
                }

                //state = OBJLoadState::Faces;
            }
        }
    }

    return ConvertVertexBuffer(vertices, vertexIndices, uvs, uvIndices, r);
}

/*
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
        if (matches != 9) {
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
}
*/
