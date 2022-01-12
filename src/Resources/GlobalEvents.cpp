#include <vector>
#include <functional>
#include <map>

#include <Resources/GlobalEvents.hpp>

void GlobalEvents::emit(int event) {
    if (GlobalEvents::events.find(event) != GlobalEvents::events.end()) {
        for (auto callback : GlobalEvents::events[event]) {
            callback();
        }
    }
}

void GlobalEvents::on(int event, std::function<void()> callback) {
    if (GlobalEvents::events.find(event) == GlobalEvents::events.end()) {
        GlobalEvents::events[event] = std::vector<std::function<void()>>();
    }
    GlobalEvents::events[event].push_back(callback);
}