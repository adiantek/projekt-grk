#include <Animator/AnimatedModel.hpp>
#include <Animator/Joint.hpp>

namespace Animator {

    AnimatedModel::AnimatedModel(GLuint model, GLuint texture, Joint* rootJoint, int jointCount) {
        this->model = model;
        this->texture = texture;
        this->rootJoint = rootJoint;
        this->jointCount = jointCount;
        // this->animator = new Animator::Animator(this);
        rootJoint->calculateInverseBindTransform(glm::mat4(0.0f));
    }

    AnimatedModel::~AnimatedModel() {
        this->destroy();
    }

    Joint* AnimatedModel::getRootJoint() {
        return this->rootJoint;
    }

    void AnimatedModel::destroy() {
        glDeleteTextures(1, &this->texture);
        glDeleteBuffers(1, &this->model);
    }

    void AnimatedModel::doAnimation() {
        // this->animator->doAnimation();
    }

    void AnimatedModel::Update() {
        // this->animator->Update();
    }

    glm::mat4* AnimatedModel::getJointTransform() {
        glm::mat4* jointMatrices = new glm::mat4[jointCount];
        this->addJointsToArray(this->rootJoint, jointMatrices);
        return jointMatrices;
    }

    void AnimatedModel::addJointsToArray(Joint* headJoint, glm::mat4 jointMatrices[]) {
        jointMatrices[headJoint->index] = headJoint->getTransform();

        for (Joint *child : headJoint->children) {
            this->addJointsToArray(child, jointMatrices);
        }
    }

}