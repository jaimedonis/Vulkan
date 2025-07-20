#include "mesh_render_object.h"

#include "../diagnostics/logger.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtx/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assert.h>

static const glm::vec3 DEFAULT_NORMAL(0.f, 0.f, 1.f);
static const glm::vec2 DEFAULT_UV(0.f, 0.f);
static const glm::vec4 DEFAULT_COLOR(1.f, 1.f, 1.f, 1.f);


MeshRenderObject::MeshRenderObject()
{

}

void MeshRenderObject::OnLoad()
{
    assert(_mesh_resource != nullptr);

    GL_INVOKE(glGenVertexArrays(1, &_vertex_array_object))
    GL_INVOKE(glGenBuffers(1, &_vertex_position_object))
    GL_INVOKE(glGenBuffers(1, &_vertex_index_buffer_object))
    GL_INVOKE(glGenBuffers(1, &_vertex_normals_buffer_object))
    GL_INVOKE(glGenBuffers(1, &_vertex_uv_buffer_object))
    GL_INVOKE(glGenBuffers(1, &_vertex_colors_buffer_object))
    GL_INVOKE(glGenBuffers(1, &_vertex_tangents_buffer_object))
    GL_INVOKE(glGenBuffers(1, &_vertex_bitangents_buffer_object))
}

void MeshRenderObject::OnUnload()
{
    GL_INVOKE(glDeleteVertexArrays(1, &_vertex_array_object))
    _vertex_array_object = 0;
    GL_INVOKE(glDeleteBuffers(1, &_vertex_position_object))
    _vertex_position_object = 0;
    GL_INVOKE(glDeleteBuffers(1, &_vertex_index_buffer_object))
    _vertex_index_buffer_object = 0;
    GL_INVOKE(glDeleteBuffers(1, &_vertex_normals_buffer_object))
    _vertex_normals_buffer_object = 0;
    GL_INVOKE(glDeleteBuffers(1, &_vertex_uv_buffer_object))
    _vertex_uv_buffer_object = 0;
    GL_INVOKE(glDeleteBuffers(1, &_vertex_colors_buffer_object))
    _vertex_colors_buffer_object = 0;
    GL_INVOKE(glDeleteBuffers(1, &_vertex_tangents_buffer_object))
    _vertex_tangents_buffer_object = 0;
    GL_INVOKE(glDeleteBuffers(1, &_vertex_bitangents_buffer_object))
    _vertex_bitangents_buffer_object = 0;
}

void MeshRenderObject::OnRender()
{
    for(size_t i = 0; i < _mesh_resource->_mesh_items.size(); ++i ) {
        // TODO: don't make a copy
        auto vertex_positions = _mesh_resource->_mesh_items[i]._vertices;
        auto vertex_indices = _mesh_resource->_mesh_items[i]._indices;
        auto vertex_normals = _mesh_resource->_mesh_items[i]._normals;
        auto vertex_uvs = _mesh_resource->_mesh_items[i]._uvs;
        auto vertex_colors = _mesh_resource->_mesh_items[i]._colors;
        auto vertex_tangents = _mesh_resource->_mesh_items[i]._tangents;
        auto vertex_bitangents = _mesh_resource->_mesh_items[i]._bitangents;

        // bind vao
        glBindVertexArray(_vertex_array_object);

        // == update mesh vertex buffers ==

        // update vertex position buffer
        int vertex_attribute_index = 0; // @ shader offset
        if(!vertex_positions.empty()) {
            GL_INVOKE(glBindBuffer(GL_ARRAY_BUFFER, _vertex_position_object))
            GL_INVOKE(glBufferData(GL_ARRAY_BUFFER,
                        sizeof(glm::vec4) * vertex_positions.size(),
                        glm::value_ptr(vertex_positions.data()[0]),
                        GL_STATIC_DRAW))
            GL_INVOKE(glVertexAttribPointer(vertex_attribute_index, 4, GL_FLOAT, GL_FALSE, 0, 0))
            GL_INVOKE(glEnableVertexAttribArray(vertex_attribute_index))
            GL_INVOKE(glBindBuffer(GL_ARRAY_BUFFER, 0))
        }

        // update vertex index buffer
        if( !vertex_indices.empty() ) {
            GL_INVOKE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertex_index_buffer_object))
            GL_INVOKE(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         sizeof(vertex_indices[0]) * vertex_indices.size(),
                         &(vertex_indices.data()[0]),
                         GL_STATIC_DRAW))
            GL_INVOKE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
        }

        // update vertex normals buffer
        vertex_attribute_index = 1; // @ shader offset
        if( !vertex_normals.empty() ) {
            GL_INVOKE(glBindBuffer(GL_ARRAY_BUFFER, _vertex_normals_buffer_object))
            GL_INVOKE(glBufferData(GL_ARRAY_BUFFER,
                         sizeof(glm::vec3) * vertex_normals.size(),
                         glm::value_ptr(vertex_normals.data()[0]),
                         GL_STATIC_DRAW))
            GL_INVOKE(glVertexAttribPointer(vertex_attribute_index, 3, GL_FLOAT, GL_FALSE, 0, 0))
            GL_INVOKE(glEnableVertexAttribArray(vertex_attribute_index))
            GL_INVOKE(glBindBuffer(GL_ARRAY_BUFFER, 0))
        } else {
            GL_INVOKE(glVertexAttrib3fv(vertex_attribute_index, glm::value_ptr(DEFAULT_NORMAL)))
        }

        // update vertex uv buffer
        vertex_attribute_index = 2; // @ shader offset
        if( !vertex_uvs.empty() ) {
            GL_INVOKE(glBindBuffer(GL_ARRAY_BUFFER, _vertex_uv_buffer_object))
            GL_INVOKE(glBufferData(GL_ARRAY_BUFFER,
                         sizeof(vertex_uvs[0]) * vertex_uvs.size(),
                         &(vertex_uvs.data()[0]),
                         GL_STATIC_DRAW))
            GL_INVOKE(glVertexAttribPointer(vertex_attribute_index, 2, GL_FLOAT, GL_FALSE, 0, 0))
            GL_INVOKE(glEnableVertexAttribArray(vertex_attribute_index))
            GL_INVOKE(glBindBuffer(GL_ARRAY_BUFFER, 0))
        } else {
            GL_INVOKE(glVertexAttrib2fv(vertex_attribute_index, glm::value_ptr(DEFAULT_UV)))
        }

        // update vertex colors buffer
        vertex_attribute_index = 3; // @ shader offset
        if( !vertex_colors.empty() ) {
            GL_INVOKE(glBindBuffer(GL_ARRAY_BUFFER, _vertex_colors_buffer_object))
            GL_INVOKE(glBufferData(GL_ARRAY_BUFFER,
                         sizeof(vertex_colors[0]) * vertex_colors.size(),
                         &(vertex_colors.data()[0]),
                         GL_STATIC_DRAW))
            GL_INVOKE(glVertexAttribPointer(vertex_attribute_index, 4, GL_FLOAT, GL_FALSE, 0, 0))
            GL_INVOKE(glEnableVertexAttribArray(vertex_attribute_index))
            GL_INVOKE(glBindBuffer(GL_ARRAY_BUFFER, 0))
        } else {
            GL_INVOKE(glVertexAttrib4fv(vertex_attribute_index, glm::value_ptr(DEFAULT_COLOR)))
        }

        // update vertex tangents buffer
        vertex_attribute_index = 4; // @ shader offset
        if( !vertex_tangents.empty() ) {
            GL_INVOKE(glBindBuffer(GL_ARRAY_BUFFER, _vertex_tangents_buffer_object))
            GL_INVOKE(glBufferData(GL_ARRAY_BUFFER,
                         sizeof(vertex_tangents[0]) * vertex_tangents.size(),
                         &(vertex_tangents.data()[0]),
                         GL_STATIC_DRAW))
            GL_INVOKE(glVertexAttribPointer(vertex_attribute_index, 3, GL_FLOAT, GL_FALSE, 0, 0))
            GL_INVOKE(glEnableVertexAttribArray(vertex_attribute_index))
            GL_INVOKE(glBindBuffer(GL_ARRAY_BUFFER, 0))
        } else {
            GL_INVOKE(glVertexAttrib3fv(vertex_attribute_index, glm::value_ptr(DEFAULT_NORMAL)))
        }

        // update vertex bitangents buffer
        vertex_attribute_index = 5; // @ shader offset
        if( !vertex_bitangents.empty() ) {
            GL_INVOKE(glBindBuffer(GL_ARRAY_BUFFER, _vertex_bitangents_buffer_object))
            GL_INVOKE(glBufferData(GL_ARRAY_BUFFER,
                         sizeof(vertex_bitangents[0]) * vertex_bitangents.size(),
                         &(vertex_bitangents.data()[0]),
                         GL_STATIC_DRAW))
            GL_INVOKE(glVertexAttribPointer(vertex_attribute_index, 3, GL_FLOAT, GL_FALSE, 0, 0))
            GL_INVOKE(glEnableVertexAttribArray(vertex_attribute_index))
            GL_INVOKE(glBindBuffer(GL_ARRAY_BUFFER, 0))
        } else {
            GL_INVOKE(glVertexAttrib3fv(vertex_attribute_index, glm::value_ptr(DEFAULT_NORMAL)))
        }

        // bind vertex index buffer
        GL_INVOKE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertex_index_buffer_object))

        // render triangles
        if (_vertex_index_buffer_object) {
            GL_INVOKE(glDrawElements(GL_TRIANGLES, vertex_indices.size(), GL_UNSIGNED_INT, (void*)0))
        } else {
            GL_INVOKE(glDrawArrays(GL_TRIANGLES, 0, vertex_positions.size()))
        }
    }

    // unbind both vertex index buffers and vertex array object
    GL_INVOKE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
    GL_INVOKE(glBindVertexArray(0))
}
