#ifndef TIME_HPP
#define TIME_HPP

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

class Time{
    struct timeval tv_;
    struct tm* ptm_;
    string timeStr_;
    char time_string_[40];
public:
    const string& GetTime() {
	gettimeofday (&tv_, NULL);
	strftime (time_string_, sizeof (time_string_), "%H:%M:%S", localtime(&tv_.tv_sec));
	timeStr_ = time_string_;
	return timeStr_;
    }	
};

#endif
