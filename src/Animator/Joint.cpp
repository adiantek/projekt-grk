#include <Animator/Joint.hpp>
#include <glm/ext.hpp>
#include <list>

namespace Animator {

    Joint::Joint(int index, std::string name, glm::mat4 localBindTransform, Joint* parent, std::list<Joint> children) {
        this->index = index;
        this->name = name;
        this->parent = parent;
        this->children = children;
        this->localBindTransform = localBindTransform;
    }

    Joint::Joint(int index, std::string name, glm::mat4 localBindTransform) {
        this->index = index;
        this->name = name;
        this->children = {};
        this->localBindTransform = localBindTransform;
    }

    Joint::~Joint() {}

    void Joint::addChild(Joint child) {
        this->children.push_back(child);
    }

    glm::mat4 Joint::getTransform() {
        return this->transform;
    }

    void Joint::setTransform(glm::mat4 transform) {
        this->transform = transform;
    }

    glm::mat4 Joint::getInverseBindTransform() {
        return this->inverseBindTransform;
    }

    void Joint::calculateInverseBindTransform(glm::mat4 parentBindTransform) {
        glm::mat4 bindTransform = parentBindTransform * this->localBindTransform;
        this->inverseBindTransform = glm::inverse(bindTransform);

        for (Joint child : this->children) {
            child.calculateInverseBindTransform(bindTransform);
        }
    }

}