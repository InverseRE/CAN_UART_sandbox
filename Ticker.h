/* -*-*- mode: c++ -*-*- */

#ifndef __TICKER_H__
#define __TICKER_H__

#include <Arduino.h>

class Ticker
{
    using time_t = unsigned long;

private:
    time_t time_mark = 0;

public:

    bool mark() const
    {
        return time_mark and time_mark < millis();
    }

    void set_next(time_t duration)
    {
        time_mark = duration + millis();
    }
};

#endif  /* __TICKER_H__ */
