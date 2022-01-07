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
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

GameObject*  GameObject::setPosition(glm::vec3 position) {
    this->position = position;
    return this;
}

GameObject*  GameObject::setRotation(glm::vec3 rotation) {
    this->rotation = rotation;
    return this;
}

GameObject*  GameObject::setScale(glm::vec3 scale) {
    this->scale = scale;
    return this;
}

GameObject* GameObject::setMaterials(std::vector<Material *> materials) {
    this->materials = materials;
    return this;
}

GameObject* GameObject::setMaterial(Material *material) {
    this->materials = { material };
    return this;
}

GameObject* GameObject::setModel(Model* model) {
    this->model = model;
    std::cout << "Set model file: " << this->model->file << std::endl;
    return this;
}

glm::mat4 GameObject::getModelMatrix() {
    // TODO: Add rotation
    return 
        glm::translate(this->position)
        * glm::rotate(glm::radians(-90.0f), glm::vec3(1,0,0))
        // * glm::eulerAngleXYZ(glm::radians(this->rotation.x), glm::radians(this->rotation.y), glm::radians(this->rotation.z))
        // glm::lookAt(this->position, this->position + this->rotation, glm::vec3(0,1,0));
        * glm::scale(this->scale);
}

void GameObject::draw() {
    std::cout << "Drawing: " << this->name << std::endl;
    glm::mat4 viewMatrix = camera->getTransformationMatrix();
    glm::mat4 modelMatrix = this->getModelMatrix();
    std::vector<Mesh *> meshes = this->model->meshes;

    glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));
    glm::mat4 modelViewProjectionMatrix = viewMatrix * modelMatrix;

    // Iterate over meshes
    for (int i = 0; i < meshes.size(); i++) {
        Mesh* mesh = meshes[i];
        Material* material = this->materials[i];

        Core::RenderContext* context = this->model->meshes[i]->getRenderContext();

        if (material == nullptr) {
            material = Resources::MATERIALS.DEFAULT;
        }

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