/*
 * IBMisc: Misc. Routines for IceBin (and other code)
 * Copyright (c) 2013-2016 by Elizabeth Fischer
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <ctime>

namespace ibmisc {
namespace time {


/*
Member      Type            Meaning         Range
tm_sec      int seconds after the minute    0-60*
tm_min      int minutes after the hour      0-59
tm_hour     int hours since midnight        0-23
tm_mday     int day of the month            1-31
tm_mon      int months since January        0-11
tm_year     int years since 1900    
tm_wday     int days since Sunday           0-6
tm_yday     int days since January 1        0-365
tm_isdst    int Daylight Saving Time        flag
    
The Daylight Saving Time flag (tm_isdst) is greater than zero if
Daylight Saving Time is in effect, zero if Daylight Saving Time is not
in effect, and less than zero if the information is not available.

* tm_sec is generally 0-59. The extra range is to accommodate for leap
*seconds in certain systems.  */

/** C++ version of standard struct tm */
class tm : public ::tm {
public:
    tm() {
        tm_year = 0;
        tm_mon = 0;
        tm_mday = 0;
        tm_isdst = 0;
        tm_yday = 0;
        tm_wday = 0;
        tm_hour = 0;
        tm_min = 0;
        tm_sec = 0;
    }

    tm(int year, int month, int mday)
    {
        tm_year = year - 1900;
        tm_mon = month - 1;
        tm_mday = mday;
        tm_isdst = 0;
        tm_yday = 0;
        tm_wday = 0;
        tm_hour = 0;
        tm_min = 0;
        tm_sec = 0;
    }

        int year() const { return tm_year + 1900; }
        int mon() const { return tm_mon + 1; }
        int month() const { return tm_mon + 1; }
        int mday() const { return tm_mday; }
    };

}}
