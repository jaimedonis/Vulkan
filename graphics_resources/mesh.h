#pragma once

#include "graphics_resource.h"

#include <glm/glm.hpp>

#include <vector>


class MeshItem
{
public:
    MeshItem() = default;

public:
    using Vertices = std::vector<glm::vec4>;
    using Indices = std::vector<unsigned int>;
    using Normals = std::vector<glm::vec3>;
    using UVs = std::vector<glm::vec2>;
    using Colors = std::vector<glm::vec4>;
    using Tangents = std::vector<glm::vec3>;
    using Bitangents = std::vector<glm::vec3>;

    Vertices _vertices;
    Indices _indices;
    Normals _normals;
    UVs _uvs;
    Colors _colors;
    Tangents _tangents;
    Bitangents _bitangents;
};

class Mesh : public GraphicsResource
{
public:
    Mesh();

public:
    std::vector<MeshItem> _mesh_items;
};


