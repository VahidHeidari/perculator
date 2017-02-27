/**
 * Perculator (Performance Calculator and viewer) is a simple time tracker.
 *
 * Copyright (C) 2015  Vahid Heidari (DeltaCode)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TimeLogger.h"

#include <fstream>
#include <iostream>
#include <ctime>
#include <cstring>

using namespace std;

//constexpr TimeLogger::Date TimeLogger::Date::zero(0, 0, 0);

constexpr unsigned int TimeLogger::VERSION;// = 0x100000;
constexpr unsigned int TimeLogger::MAGIC_SIG;// = ('L'|('O'<<8)|('G'<<16)|('R'<<24));

constexpr size_t BUFF_SIZE = 100;


void TimeLogger::Date::fill_date()
{
	time_t now;
	struct tm* tm_now;
	time(&now);
	tm_now = localtime(&now);
	year = tm_now->tm_year + 1900;
	month = tm_now->tm_mon + 1;
	day = tm_now->tm_mday;
}

ostream& TimeLogger::Date::serialize(ostream& os) const
{
	os.write((const char*)&year, sizeof(year));
	os.write((const char*)&month, sizeof(month));
	os.write((const char*)&day, sizeof(day));
	return os;
}

istream& TimeLogger::Date::deserialize(istream& is)
{
	is.read((char*)&year, sizeof(year));
	is.read((char*)&month, sizeof(month));
	is.read((char*)&day, sizeof(day));
	return is;
}

bool TimeLogger::Date::operator <(const Date& right) const
{
	if (year < right.year)
		return true;
	else if (year > right.year)
		return false;
	// Years are equal.

	if (month < right.month)
		return true;
	else if (month > right.month)
		return false;
	// Months are equal.

	return day < right.day;
}

bool TimeLogger::Date::operator >(const Date& right) const
{
	if (year > right.year)
		return true;
	else if (year < right.year)
		return false;

	if (month > right.month)
		return true;
	else if (month < right.month)
		return false;

	return day > right.day;
}


TimeLogger::Time::Time(int h, int m, int s)
: hour(h)
, minute(m)
, second(s)
, befor_origin(false)
{
	if (h > 0) {
		h *= -1;
		befor_origin = true;
	}

	if (m < 0) {
		minute *= -1;
		befor_origin = true;
	}

	if (s < 0) {
		second *= -1;
		befor_origin = true;
	}
}

void TimeLogger::Time::set_total_seconds(int seconds)
{
	if (seconds < 0) {
		befor_origin = true;
		seconds *= -1;
	}
	else
		befor_origin = false;

	second = seconds % 60;
	seconds /= 60;

	minute = seconds % 60;
	seconds /= 60;

	hour = seconds;
}

void TimeLogger::Time::fill_time()
{
	time_t now;
	struct tm* tm_now;
	time(&now);
	tm_now = localtime(&now);
	hour = tm_now->tm_hour;
	minute = tm_now->tm_min;
	second = tm_now->tm_sec;
}

TimeLogger::Time TimeLogger::Time::operator +(const Time& right)
{
	second += right.second;
	if (second > 60) {
		second -= 60;
		++minute;
	}

	minute += right.minute;
	if (minute > 60) {
		minute -= 60;
		++hour;
	}

	hour += right.hour;
	
	return *this;
}

bool TimeLogger::Time::operator <(const Time& right) const
{
	if (hour < right.hour)
		return true;
	else if (hour > right.hour)
		return false;
	// Hours are equal.

	if (minute < right.minute)
		return true;
	else if (minute > right.minute)
		return false;
	// Minutes are equal.

	return second < right.second;
}

bool TimeLogger::Time::operator >(const Time& right) const
{
	if (hour > right.hour)
		return true;
	else if (hour < right.hour)
		return false;

	if (minute > right.minute)
		return true;
	else if (minute < right.minute)
		return false;

	return second > right.second;
}

ostream& TimeLogger::Time::serialize(ostream& os) const
{
	os.write((const char*)&hour, sizeof(hour));
	os.write((const char*)&minute, sizeof(minute));
	os.write((const char*)&second, sizeof(second));
	return os;
}

istream& TimeLogger::Time::deserialize(istream& is)
{
	is.read((char*)&hour, sizeof(hour));
	is.read((char*)&minute ,sizeof(minute));
	is.read((char*)&second ,sizeof(second));
	return is;
}


ostream& TimeLogger::PausedTime::serialize(ostream& os) const
{
	start.serialize(os);
	end.serialize(os);
	return os;
}

istream& TimeLogger::PausedTime::deserialize(istream& is)
{
	start.deserialize(is);
	end.deserialize(is);
	return is;
}


TimeLogger::Time TimeLogger::Record::get_unuseful_time()
{
	TimeLogger::Time unuseful;

	//for (TimeLogger::PausedTime p : paused_times) {		// Only C++11
	for (PausedTimeList::const_iterator p = paused_times.begin(), end = paused_times.end(); p != end; ++p) {
		if (!p->end.is_zero())
			unuseful += p->end - p->start;
		else
			if (last_update_time > p->start)
				unuseful += last_update_time - p->start;
	}

	return unuseful;
}

void TimeLogger::Record::calculate_performance()
{
	float total;
	if (is_commited())
		total = (presence.end - presence.start).get_total_seconds();
	else
		total = (last_update_time - presence.start).get_total_seconds();

	float unuseful = get_unuseful_time().get_total_seconds();
	float useful = total - unuseful;
	performance = (int)(useful / total * 100.0f);
}

ostream& TimeLogger::Record::print(ostream& os, string ind) const
{
	os << ind << "Record" << endl;
	date.print(os, ind + '\t');

	os << ind << '\t' << "Presence :" << endl;
	presence.print(os, ind + "\t\t");

	os << ind << '\t' << "Last update time :" << endl;
	last_update_time.print(os, ind + "\t\t");

	os << ind << '\t' << "Corrupted time :" << endl;
	corrupted_time.print(os, ind + "\t\t");

	os << ind << "\tPerformance : " << performance << endl;
	os << ind << "\tPaused Times : " << paused_times.size() << endl;

	//for (PausedTime& p : paused_times) // Only C++11
	for (PausedTimeList::const_iterator p = paused_times.begin(), end = paused_times.end(); p != end; ++p)
		p->print(os, ind + "\t\t");
	return os;
}

std::ostream& TimeLogger::Record::serialize(ostream& os) const
{
	date.serialize(os);
	presence.serialize(os);
	last_update_time.serialize(os);
	corrupted_time.serialize(os);

	os.write((const char*)&performance, sizeof(performance));

	size_t size = paused_times.size();
	os.write((const char*)&size, sizeof(size));
	//for (PausedTime& p : paused_times) // Only C++11
	for (PausedTimeList::const_iterator p = paused_times.begin(), end = paused_times.end(); p != end; ++p)
		p->serialize(os);
	return os;
}

istream& TimeLogger::Record::deserialize(istream& is)
{
	date.deserialize(is);
	presence.deserialize(is);
	last_update_time.deserialize(is);
	corrupted_time.deserialize(is);

	is.read((char*)&performance, sizeof(performance));

	size_t size;
	is.read((char*)&size, sizeof(size));

	for (size_t i = 0; i < size; ++i) {
		PausedTime p;
		p.deserialize(is);
		paused_times.push_back(p);
	}
	return is;
}

void TimeLogger::Record::add_pause()
{
	// Close last paused time, if any, first.
	if (!paused_times.empty()) {
		PausedTime p = paused_times.back();

		if (p.end.is_zero())
			p.end = Time::get_now();		// Close last paused time.
	}

	// Add new paused time.
	TimeLogger::PausedTime new_paused;
	new_paused.start = Time::get_now();
	paused_times.push_back(new_paused);
}

void TimeLogger::Record::add_resume()
{
	// Check there is any paused time.
	if (!paused_times.empty()) {
		PausedTimeList::iterator p = --paused_times.end();

		if (p->end.is_zero())
			p->end = Time::get_now();
	}
}


static bool is_file_exist(string path)
{
	ifstream file(path.c_str());//, ios_base::
	return file.good();
}

bool TimeLogger::open_print(string path)
{
	if (!is_file_exist(path)) {
		cerr << "Could not open print log file!" << endl;
		return false;
	}

	Record r;
	if (!deserialize(path, r)) {
		cerr << "Could not open and print log file!" << endl;
		return false;
	}

	r.print(cout);
	return true;
}

bool TimeLogger::open_log_file()
{
	string file_name = get_today_file_name();

	// Try existing file
	if (is_file_exist(file_name)) {
		cerr << "File exists!" << endl;
		cerr << "Check for aborted transactions . . ." << endl;

		if (!deserialize(file_name)) {
			cerr << "Could not open and read log file '" << file_name << "'!" << endl;
			cerr << "Trying to create new on . . ." << endl;
		} else {

			// Check un-commited file.
			if (!record.is_commited()) {
				cerr << "Wow . . ." << endl;
				cerr << "There is a un-commited file today!" << endl;
				cerr << "Calculating corrupted time . . ." << endl;
				if (!record.last_update_time.is_zero())
					record.corrupted_time += (Time::get_now() - record.last_update_time);
				else if (!record.presence.start.is_zero())
					record.corrupted_time += (Time::get_now() - record.presence.start);
				else
					cerr << "Could not calculate corrupted time!" << endl;
			} else {		// Resume commited file.
				cerr << "Yes . . ." << endl;
				cerr << "There is a commited file today!" << endl;
				cerr << "Continuing . . ." << endl;
				record.corrupted_time += (Time::get_now() - record.presence.end);
				record.presence.end.reset();
			}

			record.print(cout);
			cerr << "Checking completed." << endl;

			return true;
		}
	} 

	// Create new one
	record.reset();
	record.presence.start.fill_time();
	record.date.fill_date();

	if (!serialize()) {
		cerr << "Could not create and open file '" << file_name << "'!" << endl;
		return false;
	}

	cerr << "Creating done." << endl;
	return true;
}

bool TimeLogger::close_log_file()
{
	if (!update_log()) {
		cerr << "Could not commit log file!" << endl;
		return false;
	}

	record.presence.end = Time::get_now();		// End of presecne time update

	if (record.paused_times.back().end.is_zero())
		record.paused_times.back().end = Time::get_now();		// Paused times uptate

	if (!serialize()) {
		cerr << "Could not commit and close log file." << endl;
		return false;
	}

	return true;
}

bool TimeLogger::update_log()
{
	Time t;
	t.fill_time();
	record.last_update_time = t;

	if (!serialize()) {
		cerr << "Could not open log file for update at ";
		t.print(cerr);
		cerr << " !" << endl;
		return false;
	}

	return true;
}

string TimeLogger::get_today_file_name() const
{
	time_t now;
	struct tm* tm_now;
	time(&now);
	tm_now = localtime(&now);
	char buff[BUFF_SIZE] = { 0 };
	strftime(buff, BUFF_SIZE, "%a %b %Y-%m-%d", tm_now);
	string filename(buff, strlen(buff));
	filename.append(".log", 4);
#ifdef __linux__
	filename = "/home/user/TimeLogger/" + filename;
#endif
	return filename;
}

bool TimeLogger::serialize()
{
	fstream logfile;
	logfile.open(get_today_file_name().c_str(), ios_base::binary | ios_base::out);

	if (!logfile.is_open()) {
		cerr << "Could not open file for serializing!" << endl;
		return false;
	}

	// Header
	logfile.write((const char*)&MAGIC_SIG, sizeof(MAGIC_SIG));
	logfile.write((const char*)&VERSION, sizeof(VERSION));

	record.calculate_performance();
	record.serialize(logfile);
	record.print(cout);

	return true;
}

bool TimeLogger::deserialize(string file_name, Record& record)
{
	fstream logfile;
	logfile.open(file_name.c_str(), ios_base::binary | ios_base::in);

	if (!logfile.is_open()) {
		cerr << "Could not open and deserialize file '" << file_name << "'!" << endl;
		return false;
	}

	logfile.seekp(0, ios_base::end);
	size_t file_size = logfile.tellp();
	logfile.seekp(0, ios_base::beg);

	if (!file_size) {
		cerr << "Cuold not deserialize empty file!" << endl;
		return false;
	}

	// Header
	unsigned int magic_sig;
	logfile.read((char*)&magic_sig, sizeof(MAGIC_SIG));
	if (magic_sig != MAGIC_SIG) {
		cerr << "This file is not in the TimeLogger format!" << endl;
		return false;
	}

	unsigned int version;
	logfile.read((char*)&version, sizeof(VERSION));
	if (version != VERSION) {
		cerr << "Version missmatch!" << endl;
		return false;
	}

	record.deserialize(logfile);
	return true;
}

