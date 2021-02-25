#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

enum TrafficLightPhase{red,green};

template <class T>
class MessageQueue
{
public:
void send(T &&_phase);
T receive();
private:
    std::mutex _mutex;
    std::condition_variable _cond;
    std::deque <TrafficLightPhase>_queue;
};

class TrafficLight: public TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();
    //~TrafficLight::TrafficLight();
    // getters / setters

    // typical behaviour methods
    void simulate();
    void waitForGreen();
    TrafficLightPhase getCurrentPhase();
private:
    // typical behaviour methods
    void cycleThroughPhases();
    TrafficLightPhase _currentPhase;
    MessageQueue <TrafficLightPhase> _msgs;
    std::condition_variable _condition;
    std::mutex _mutex;
};

#endif
