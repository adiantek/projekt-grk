#include <Logger.h>

#include <Physics/ErrorCallback.hpp>
namespace physics {
ErrorCallback::ErrorCallback(LogLevel level) {
    this->level = level;
}

ErrorCallback::~ErrorCallback() {}

void ErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) {
    if (code & physx::PxErrorCode::Enum::eDEBUG_INFO && this->level <= __DEBUG) {
        LOGD("PhysX debug info, file \"%s\", line %d: %s", file, line, message);
    }
    if (code & physx::PxErrorCode::Enum::eDEBUG_WARNING && this->level <= WARNING) {
        LOGW("PhysX warning, file \"%s\", line %d: %s", file, line, message);
    }
    if (code & physx::PxErrorCode::Enum::eINVALID_PARAMETER && this->level <= _ERROR) {
        LOGE("PhysX invalid parameter, file \"%s\", line %d: %s", file, line, message);
    }
    if (code & physx::PxErrorCode::Enum::eINVALID_OPERATION && this->level <= _ERROR) {
        LOGE("PhysX invalid operation, file \"%s\", line %d: %s", file, line, message);
    }
    if (code & physx::PxErrorCode::Enum::eOUT_OF_MEMORY && this->level <= _ERROR) {
        LOGE("PhysX out of memory, file \"%s\", line %d: %s", file, line, message);
    }
    if (code & physx::PxErrorCode::Enum::eINTERNAL_ERROR && this->level <= _ERROR) {
        LOGE("PhysX internal error, file \"%s\", line %d: %s", file, line, message);
    }
    if (code & physx::PxErrorCode::Enum::eABORT) {
        LOGE("PhysX unrecoverable error, file \"%s\", line %d: %s", file, line, message);
        LOGE("Stopping aplication...");
        exit(1);
    }
    if (code & physx::PxErrorCode::Enum::ePERF_WARNING && this->level <= WARNING) {
        LOGW("PhysX performance warning, file \"%s\", line %d: %s", file, line, message);
    }
}
}  // namespace physics