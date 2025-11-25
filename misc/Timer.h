#include <iostream>
#include <sys/time.h>
#include <unistd.h>


class Timer
{
public:
    Timer()
    {
        gettimeofday(&m_StartTime, NULL);
    }
    ~Timer()
    {
        gettimeofday(&m_EndTime, NULL);
        std::cout << "Time elapsed: " << (m_EndTime.tv_sec - m_StartTime.tv_sec) * 1000.0 + (m_EndTime.tv_usec - m_StartTime.tv_usec) / 1000.0 << " ms"<< std::endl;
    }
private:
    struct timeval m_StartTime;
    struct timeval m_EndTime;
};