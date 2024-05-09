#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <list>
#include <functional>

template<typename... _Args> 
class GameEvent {
    
    typedef std::function<void(_Args...)> Listener;

    public:
        void operator=(const GameEvent&) = delete;
        GameEvent<_Args...>& operator+=(const Listener &l) {
            this->listeners.emplace_back(l);
            return *this;
        }
        void operator()(_Args... args) {
            for (Listener l : listeners) {
                l(args...);
            }
        }

        int size() {
            return listeners.size();
        }

    private:
        std::list<Listener> listeners;
};

#endif