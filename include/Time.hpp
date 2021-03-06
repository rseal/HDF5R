// Copyright (c) 2010 Ryan Seal <rlseal -at- gmail.com>
//
// This file is part of HDF5 for Radar (HDF5R) Software.
//
// HDF5R is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//  
// HDF5R is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HDF5R.  If not, see <http://www.gnu.org/licenses/>.
#ifndef TIME_HPP
#define TIME_HPP

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

class Time{
    struct timeval tv_;
    struct tm* ptm_;
    std::string timeStr_;
    char time_string_[40];
public:
    const std::string& GetTime() {
	gettimeofday (&tv_, NULL);
	strftime (time_string_, sizeof (time_string_), "%H:%M:%S", localtime(&tv_.tv_sec));
	timeStr_ = time_string_;
	return timeStr_;
    }	
};

#endif
