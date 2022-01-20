#include <glm/ext.hpp>
#include <vector>
#include <stdbool.h>

#include <Animator/Joint.hpp>

namespace Animator {

    Joint::Joint() {
        this->index = -1;
        this->name = "";
        this->parent = NULL;
        this->children = std::vector<Joint*>();
    }

    Joint::Joint(int index, std::string name, glm::mat4 transform, Joint* parent, std::vector<Joint*> children) {
        this->index = index;
        this->name = name;
        this->setParent(parent);
        this->children = children;
        this->transform = transform;
    }

    Joint::Joint(int index, std::string name, glm::mat4 transform, std::vector<Joint*> children) {
        this->index = index;
        this->name = name;
        this->setParent(parent);
        this->children = children;
        this->transform = transform;
    }

    Joint::Joint(int index, std::string name, glm::mat4 transform, Joint* parent) {
        this->index = index;
        this->name = name;
        this->setParent(parent);
        this->children = {};
        this->transform = transform;
    }

    Joint::Joint(int index, std::string name, glm::mat4 transform) {
        this->index = index;
        this->name = name;
        this->children = {};
        this->parent = NULL;
        this->transform = transform;
    }

    Joint::~Joint() {}

    Joint* Joint::setParent(Joint* parent) {
        this->parent = parent;
        this->_hasParent = true;
        return this;
    }

    Joint* Joint::addChild(Joint* child) {
        this->children.push_back(child);
        return this;
    }

    glm::mat4 Joint::getTransform() {
        return this->transform;
    }

    glm::mat4 Joint::getLocalBindTransform() {
        return this->localBindTransform;
    }

    Joint* Joint::setTransform(glm::mat4 transform) {
        this->transform = transform;
        return this;
    }

    Joint* Joint::setLocalBindTransform(glm::mat4 localBindTransform) {
        this->localBindTransform = localBindTransform;
        return this;
    }

    glm::mat4 Joint::getInverseBindTransform() {
        return this->inverseBindTransform;
    }

    void Joint::calculateInverseBindTransform(glm::mat4 parentBindTransform) {
        glm::mat4 bindTransform = parentBindTransform * this->localBindTransform;
        this->inverseBindTransform = glm::inverse(bindTransform);

        for (Joint* child : this->children) {
            child->calculateInverseBindTransform(bindTransform);
        }
    }

    glm::vec3 Joint::getOrigin() {
        return glm::vec3(glm::inverse(this->localBindTransform) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }

    bool Joint::hasParent() {
        return this->_hasParent;
    }

    bool Joint::hasChildren() {
        return this->children.size() > 0;
    }

}