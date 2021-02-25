#include <iostream>
#include <random>
#include "TrafficLight.h"
 
/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] { return !_queue.empty(); }); // pass unique lock to condition variable

    // remove last vector element from queue
    T msg = std::move(_queue.back());
    _queue.pop_back();

    return msg;
}
template <typename T>
void MessageQueue<T>::send(T &&msg)
{
   std::lock_guard<std::mutex> uLock(_mutex);
    _queue.push_back(std::move(msg));
    _cond.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    while (1)
    {
        if(_msgs.receive() == green)
        {
            return;
        }
    }
    
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}
void TrafficLight::simulate()
{
   threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases,this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{ 
    std::chrono::time_point<std::chrono::system_clock> time_update;
    double wait_tim = rand() % 6 + 4;//defined gap between toggles = 4sec to 6sec
    // init stop watch
    time_update = std::chrono::system_clock::now();
    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - time_update).count();
        if (timeSinceLastUpdate >= wait_tim)
        {
            if(_currentPhase == red)
                _currentPhase = green;
            else
                _currentPhase = red;

            _msgs.send(std::move(_currentPhase));
            //reset clock
            time_update = std::chrono::system_clock::now();
            wait_tim = rand() % 6 + 4;//defined gap between toggles = 4sec to 6sec
        }
    }
}
