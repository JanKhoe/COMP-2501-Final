#pragma once
#ifndef TIMER_H
#define TIMER_H

namespace game {

    // A class implementing a simple timer
    class Timer {

    public:
        // Constructor and destructor
        Timer(void);
        Timer(float end_time);
        ~Timer();

        // Start the timer now: end time given in seconds
        void Start(float end_time);

        // Check if timer has finished
        bool Finished(void) const;

    private:
        float current_time;
        float finish_time;

    }; // class Timer

} // namespace game

#endif // TIMER_H_
