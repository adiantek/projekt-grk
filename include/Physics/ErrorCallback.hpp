#pragma once

#include <PxConfig.h>
#include <PxPhysicsAPI.h>

namespace physics {
class ErrorCallback : physx::PxErrorCallback {
   public:
    enum LogLevel {
        __DEBUG = 0,
        WARNING = 1,
        _ERROR = 2,
        NONE = 3
    };
    ErrorCallback(LogLevel level);
    ~ErrorCallback();
    void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;

   private:
    LogLevel level;
};
}