#pragma once

#include <functional>
#include <map>
#include <vector>

class GlobalEvents {
    public:
        inline static int ON_INIT = 0;
        inline static int ON_UPDATE = 1;

        static void emit(int event);
        static void on(int event, std::function<void()> callback);
    private:
        inline static std::map<int, std::vector<std::function<void()>>> events;
};