#include <GLFW/glfw3.h>
#include <iostream>

#include "Timer.h"

using namespace std;

namespace game {

    Timer::Timer(void)
    {
        finish_time = -1;
    }

    Timer::Timer(float end_time) {
        Start(end_time);
        cout << glfwGetTime() << endl;
    }

    Timer::~Timer(void)
    {
    }

    void Timer::Start(float end_time)
    {
        current_time = glfwGetTime();
        cout << glfwGetTime() << "||" << end_time << endl;
        finish_time = glfwGetTime() + end_time;
        cout << finish_time << endl;
    }

    bool Timer::Finished(void) const
    {
        if (glfwGetTime() > finish_time && finish_time != -1) {
            return true;
        }
        return false;
    }

} // namespace game
