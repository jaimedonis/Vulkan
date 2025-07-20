#include "mesh_resource_loader.h"
#include "mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <glm/glm.hpp>
//#include <glm/gtx/rotate_vector.hpp>
//#include <glm/gtx/transform.hpp>

#include <iostream>


MeshResourceLoader::MeshResourceLoader()
    : ResourceLoader("MeshResourceLoader")
{
}

std::unique_ptr<GraphicsResource> MeshResourceLoader::Create(const std::string& file_path)
{
    // 1. load asimp mesh, create new engine mesh instance +
    // 2. create mesh class instance on the heap, then populate data elements.
    // 3. return std::move(mesh instance ptr), caller owns the new pointer reference

    Assimp::Importer importer;
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);

    const aiScene* scene = importer.ReadFile(file_path.c_str(),
                                             aiProcess_CalcTangentSpace       |
                                                 aiProcess_Triangulate            |
                                                 aiProcess_JoinIdenticalVertices  |
                                                 aiProcess_FixInfacingNormals |
                                                 aiProcess_FindInstances |
                                                 aiProcess_SortByPType);
    if (!scene) {
        std::cerr << "ERROR: Assimp failed -- " << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    std::vector<std::shared_ptr<aiMaterial>> sceneMaterials;
    sceneMaterials.resize(scene->mNumMaterials);
    for (unsigned int m = 0; m < scene->mNumMaterials; ++m) {
        aiMaterial* material = scene->mMaterials[m];
        std::shared_ptr<aiMaterial> dstMaterial = std::make_shared<aiMaterial>();
        aiMaterial::CopyPropertyList(dstMaterial.get(), material);
        sceneMaterials[m] = dstMaterial;
    }

    auto new_mesh_asset = std::make_unique<Mesh>();
    new_mesh_asset->_resource_id = file_path;

    for (decltype(scene->mNumMeshes) i = 0; i < scene->mNumMeshes; ++i) {
        const aiMesh* mesh = scene->mMeshes[i];
        if (!mesh->HasPositions()) {
            std::cerr << "WARNING: A mesh in " << file_path << " does not have positions. Skipping." << std::endl;
            continue;
        }

        MeshItem mesh_item;
        auto num_vertices = mesh->mNumVertices;
        mesh_item._vertices.resize(num_vertices);
        if (mesh->mNormals) {
            mesh_item._normals.resize(num_vertices);
        }
        if (mesh->mTextureCoords[0]) {
            mesh_item._uvs.resize(num_vertices);
        }
        if (mesh->mColors[0]) {
            mesh_item._colors.resize(num_vertices);
        }
        mesh_item._indices.resize(mesh->mNumFaces * 3);

        for (decltype(num_vertices) v = 0; v < num_vertices; ++v) {

            mesh_item._vertices[v] = glm::vec4(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z, 1.f);

            if (mesh->mNormals) {
                mesh_item._normals[v] = glm::vec3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
            }

            if (mesh->mTextureCoords[0]) {
                mesh_item._uvs[v] = glm::vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
            }

            if (mesh->mColors[0]) {
                mesh_item._colors[v] = glm::vec4(mesh->mColors[0][v].r, mesh->mColors[0][v].g, mesh->mColors[0][v].b, mesh->mColors[0][v].a);
            }
        }

        for (decltype(mesh->mNumFaces) f = 0; f < mesh->mNumFaces; ++f) {
            const aiFace& face =  mesh->mFaces[f];
            if (face.mNumIndices != 3) {
                std::cerr << "WARNING: Input mesh may not be triangulated. Skipping face with: " << face.mNumIndices << " vertices." << std::endl;
                continue;
            }

            for (int j = 0; j < 3; ++j) {
                mesh_item._indices[f * 3 + j] = face.mIndices[j];
            }
        }

        new_mesh_asset->_mesh_items.push_back(mesh_item);
        /* TODO
        if (outputMaterials) {
            outputMaterials->push_back(sceneMaterials[mesh->mMaterialIndex]);
        }
        */
    }

    return new_mesh_asset;
}

