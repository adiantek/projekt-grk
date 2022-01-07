#include <vector>
#include <iostream>
#include <glm/ext.hpp>

#include <Render_Utils.h>
#include <Resources/GameObject.hpp>
#include <Resources/Resources.hpp>
#include <Camera/Camera.hpp>
#include <ResourceLoader.hpp>

GameObject::GameObject(std::string name) {
    this->name = name;

    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

void GameObject::setPosition(glm::vec3 position) {
    this->position = position;
}

void GameObject::setRotation(glm::vec3 rotation) {
    this->rotation = rotation;
}

GameObject* GameObject::setMaterials(std::vector<Material *> materials) {
    this->materials = materials;
    return this;
}

GameObject* GameObject::setModel(Model* model) {
    this->model = model;
    std::cout << "Set model file: " << this->model->file << std::endl;
    return this;
}

glm::mat4 GameObject::getModelMatrix() {
    // TODO: Add rotation and scale
    return glm::translate(this->position) * glm::eulerAngleX(-90.0f);
}

void GameObject::draw() {
    glm::mat4 viewMatrix = camera->getTransformationMatrix();
    glm::mat4 modelMatrix = this->getModelMatrix();

    // std::cout << "Model file: " << this->model->file << std::endl;

    std::vector<Mesh *> meshes = this->model->meshes;
    // printf("Render context size: %d\n", meshes[0]->getRenderContext()->size);

    glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));
    glm::mat4 modelViewProjectionMatrix = viewMatrix * modelMatrix;

    // Iterate over meshes
    for (int i = 0; i < meshes.size(); i++) {
        Mesh* mesh = meshes[i];
        Material* material = Resources::MATERIALS.DEFAULT; // this->materials[i];

        Core::RenderContext* context = this->model->meshes[i]->getRenderContext();

        if (material == nullptr) {
            material = Resources::MATERIALS.DEFAULT;
        }

        // printf("Mesh: %d, %d\n", i, mesh->vertices.size());

        // if (material->program == nullptr) {
        //     printf("PROGRAM IN NULL_PTR\n");
        // }


        glUseProgram(*material->program);
        material->loadToProgram(material->program);

        // Set light
        glUniform3f(glGetUniformLocation(*material->program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

        // printf("Mesh: %d\n", i);
        // printf("Mesh name: %s\n", mesh->name.c_str());
        // printf("Mesh vertices count: %d\n", mesh->vertices.size());
        // printf("Mesh indices count: %d\n", mesh->indices.size());
        // printf("Mesh render context size: %d\n", mesh->getRenderContext()->size);

        // Set material
        // glUniform3fv(NULL, 1, (float*)&(mesh->material.diffuse));
        // glUniform3fv(NULL, 1, (float*)&(mesh->material.specular));
        // glUniform1f(NULL, mesh->material.shininess);

        // Set textures
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, mesh->material.diffuseTexture);
        // glUniform1i(NULL, 0);

        

        // Set transformation
        glUniformMatrix4fv(glGetUniformLocation(*material->program, "modelMatrix"), 1, GL_FALSE, (float*)&(modelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(*material->program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&(modelViewProjectionMatrix));

        
        // glGetUniformLocation(*material->program, "lightDir");
        // printf("light dir location: %d\n", glGetUniformLocation(*material->program, "lightDir"));
        
        

        // Set vertices
        // glBindBuffer(GL_ARRAY_BUFFER, mesh->getRenderContext()->vertices);
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        // glEnableVertexAttribArray(0);

        // Set normals
        // glBindBuffer(GL_ARRAY_BUFFER, mesh->getRenderContext()->normals);
        // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        // glEnableVertexAttribArray(1);

        // Set texture coordinates
        // glBindBuffer(GL_ARRAY_BUFFER, mesh->getRenderContext()->textureCoordinates);
        // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

        Core::DrawContext(*context);
    }
}