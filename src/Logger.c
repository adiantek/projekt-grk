#include <Logger.h>
#include <opengl.h>

const char *LOG_LEVELS[] = {"D", "I", "N", "W", "E"};

void VLOG(int priority, const char *tag, const char *format, va_list arg)
{
    printf("[ %9.3f ] %1s [ %s ] ", glfwGetTime(), LOG_LEVELS[priority], tag);
    vprintf(format, arg);
    printf("\n");
}

void _LOGD(const char *tag, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    VLOG(DEBUG, tag, format, args);
    va_end(args);
}

void _LOGI(const char *tag, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    VLOG(INFO, tag, format, args);
    va_end(args);
}

void _LOGN(const char *tag, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    VLOG(NOTICE, tag, format, args);
    va_end(args);
}

void _LOGW(const char *tag, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    VLOG(WARN, tag, format, args);
    va_end(args);
}

void _LOGE(const char *tag, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    VLOG(ERROR, tag, format, args);
    va_end(args);
}

void LOG(int priority, const char *tag, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    VLOG(priority, tag, format, args);
    va_end(args);
}
