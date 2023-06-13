#pragma once

#include <ctime>
#include <string>

/*
std::cout << "This is a test" << std::endl;
time_t now = time(0);

tm* ltm = new tm;
localtime_s(ltm, &now);

std::cout << "Year:" << 1900 + ltm->tm_year << std::endl;
std::cout << "Month: " << 1 + ltm->tm_mon << std::endl;
std::cout << "Day: " << ltm->tm_mday << std::endl;

delete ltm;
return 0;
*/

static class TimeConverter
{
public:
	static time_t GetTimeNow()
	{
		return time(0);
	}
	static void AssignLocalTime(tm* ltm)
	{
		time_t now = GetTimeNow();
		localtime_s(ltm, &now);
	}
	static int GetDay(tm* const ltm)
	{
		return ltm->tm_mday;
	}
	static int GetMonth(tm* const ltm)
	{
		return 1 + ltm->tm_mon;
	}
	static int GetYear(tm* const ltm)
	{
		return 1900 + ltm->tm_year;
	}
	static int GetDayI()
	{
		int output;
		tm* ltm = new tm;
		AssignLocalTime(ltm);
		output = GetDay(ltm);
		delete ltm;
		return output;
	}
	static int GetMonthI()
	{
		int output;
		tm* ltm = new tm;
		AssignLocalTime(ltm);
		output = GetMonth(ltm);
		delete ltm;
		return output;
	}
	static int GetYearI()
	{
		int output;
		tm* ltm = new tm;
		AssignLocalTime(ltm);
		output = GetYear(ltm);
		delete ltm;
		return output;
	}
	static std::string TimeToString(const time_t& time)
	{
		if (time == -1) {
			return "-";
		}

		tm* ltm = new tm;
		localtime_s(ltm, &time);
		
		std::string output = "";
		output += std::to_string(ltm->tm_mday);
		output += "/";
		output += std::to_string(1 + ltm->tm_mon);
		output += "/";
		output += std::to_string(1900 + ltm->tm_year);

		delete ltm;
		return output;
	}
	static time_t RawToTime(const int day, const int month, const int year)
	{
		tm* ltm = new tm;
		AssignLocalTime(ltm);

		ltm->tm_hour = 6; // set time at the start of the day
		ltm->tm_min = 0;

		ltm->tm_mday = day;
		ltm->tm_mon = month - 1;
		ltm->tm_year = year - 1900;
		time_t t = mktime(ltm);
		std::cout << t << std::endl;
		std::cout << TimeToString(t) << std::endl;
		delete ltm;
		return t;
	}
};