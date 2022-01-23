#include <Animator/JointIO.hpp>
#include <stdexcept>

using namespace Animator;

// powalilo ten microsoft, zeby takie az hacki stosowac tylko po to, aby zapisac po ludzku strukture do pliku

#ifdef __GNUC__
#define Packed __attribute__((packed));
#else
#define Packed /* nothing */
#endif

#ifdef _MSC_VER
#pragma pack(push)
#pragma pack(1)
#endif
typedef struct JointRaw {
    int index;
    int parent; // -1 if null
    char name[JOINT_MAX_LENGTH];
    float transform[4 * 4];
    float localBindTransform[4 * 4];
} JointRaw Packed;
#ifdef _MSC_VER
#pragma pack(pop)
#endif

char *JointIO::serialize(std::vector<Joint *> vector, size_t *outLen) {
    JointRaw *joints = new JointRaw[vector.size()];
    *outLen = vector.size() * sizeof(JointRaw);
    for (size_t i = 0; i < vector.size(); i++) {
        if (i != vector[i]->index) {
            throw std::invalid_argument("chyba jednak nie XD Index of the joint (same as index in the mesh joints array)");
        }
        joints[i].index = vector[i]->index;
        joints[i].parent = vector[i]->hasParent() ? vector[i]->parent->index : -1;
        memset(joints[i].name, 0, JOINT_MAX_LENGTH); // better compression
        memcpy(joints[i].name, vector[i]->getName().c_str(), vector[i]->getName().length());
        const float *transf = glm::value_ptr(vector[i]->getTransform());
        memcpy(joints[i].transform, transf, 4 * 4 * sizeof(float));
        const float *localBindTransf = glm::value_ptr(vector[i]->getLocalBindTransform());
        memcpy(joints[i].localBindTransform, localBindTransf, 4 * 4 * sizeof(float));
    }
    return (char *)joints;
}

std::vector<Joint *> JointIO::deserialize(char *data, size_t dataLen) {
    std::vector<Joint *> vector;
    JointRaw *joints = (JointRaw *)data;
    for (size_t i = 0; i < dataLen / sizeof(JointRaw); i++) {
        glm::mat4 transform;
        glm::mat4 localBindTransform;
        float *val = glm::value_ptr(transform);
        float *valLocalBindTransform = glm::value_ptr(localBindTransform);
        for (int j = 0; j < 16; j++) {
            val[j] = joints[i].transform[j];
            valLocalBindTransform[j] = joints[i].localBindTransform[j];
        }
        Joint *j = new Joint(joints[i].index, std::string(joints[i].name), transform);
        j->setLocalBindTransform(localBindTransform);
        vector.push_back(j);
    }
    
    for (size_t i = 0; i < dataLen / sizeof(JointRaw); i++) {
        if (joints[i].parent != -1) {
            vector[i]->setParent(vector[joints[i].parent]);
        }
    }
    return vector;
}