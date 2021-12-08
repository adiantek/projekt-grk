#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

#ifndef LOGGER_H
#define LOGGER_H

#if defined (__cplusplus)
extern "C" {
#endif

#define DEBUG 0
#define INFO 1
#define NOTICE 2
#define WARN 3
#define ERROR 4

extern const char* LOG_LEVELS[5];

void VLOG(int priority, const char *tag, const char *format, va_list arg);

void _LOGD(const char *tag, const char *format, ...);
void _LOGI(const char *tag, const char *format, ...);
void _LOGN(const char *tag, const char *format, ...);
void _LOGW(const char *tag, const char *format, ...);
void _LOGE(const char *tag, const char *format, ...);
void LOG(int priority, const char *tag, const char *format, ...);

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define LOGD(...) _LOGD(__FILE__ ":" STR(__LINE__), ##__VA_ARGS__)
#define LOGI(...) _LOGI(__FILE__ ":" STR(__LINE__), ##__VA_ARGS__)
#define LOGN(...) _LOGN(__FILE__ ":" STR(__LINE__), ##__VA_ARGS__)
#define LOGW(...) _LOGW(__FILE__ ":" STR(__LINE__), ##__VA_ARGS__)
#define LOGE(...) _LOGE(__FILE__ ":" STR(__LINE__), ##__VA_ARGS__)

#if defined (__cplusplus)
}
#endif

#endif // LOGGER_H