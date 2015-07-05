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

#ifndef TIME_LOGGER_H_
#define TIME_LOGGER_H_

#include <string>
#include <iostream>
#include <list>

class TimeLogger {
public:
	static constexpr unsigned int VERSION = 0x100000;
	static constexpr unsigned int MAGIC_SIG = ('L'|('O'<<8)|('G'<<16)|('R'<<24));

	~TimeLogger() { close_log_file(); }

	class Date {
	public:
		//static constexpr Date zero(0, 0, 0);

		Date() : year(0), month(0), day(0) {}
		Date(int y, int m, int d) : year(y), month(m), day(d) {}

		/// Methods
		std::ostream& serialize(std::ostream& os) const;
		std::istream& deserialize(std::istream& is);

		std::ostream& print(std::ostream& os, std::string ind = "") const
		{
			os << ind << "Date : " << year << '\\' << month << '\\' << day << std::endl;
			return os;
		}

		void reset() { year = month = day = 0; }
		
		void fill_date();

		/// Setter, getters
		int get_year() const { return year; }
		void set_year(int y) { year = y; }

		int get_month() const { return month; }
		void set_month(int m) { month = m; }

		int get_day() const { return day; }
		void set_day(int d) { day = d; }

		/// Operators
		bool operator ==(const Date& right) const
		{
			return year == right.year
				&& month == right.month
				&& day == right.day;
		}

		bool operator !=(const Date& right) const
		{
			return !(*this == right);
		}

		Date& operator =(const Date& d)
		{
			year = d.year;
			month = d.month;
			day = d.day;
			return *this;
		}

		bool operator <(const Date& right) const;

		bool operator >(const Date& right) const;

		bool operator <=(const Date& right) const
		{
			return *this == right || *this < right;
		}

		bool operator >=(const Date& right) const
		{
			return *this == right || *this > right;
		}

	private:
		int year;
		int month;
		int day;
	};

	class Time {
	public:
		Time() : hour(0), minute(0), second(0), befor_origin(false) {}
		Time(int seconds) : hour(0), minute(0), second(0), befor_origin(false) {
			set_total_seconds(seconds);
		}
		Time(int h, int m, int s) ;
		Time(const Time& t) : hour(t.hour), minute(t.minute), second(t.second), befor_origin(t.befor_origin) {}

		/// Methods
		int get_total_seconds() const { return (befor_origin ? -1 : 1) * (hour * 60 * 60 + minute * 60 + second); }

		void set_total_seconds(int seconds);

		bool is_zero() const { return hour == 0 && minute == 0 && second == 0; }

		std::ostream& serialize(std::ostream& os) const;
		std::istream& deserialize(std::istream& is);

		std::ostream& print(std::ostream& os, std::string ind = "") const
		{
			os << ind << "Time : " << hour << ':' << minute << ':' << second << std::endl;
			return os;
		}

		void reset()
		{
			hour = minute = second = 0;
			befor_origin = false;
		}

		void fill_time();
		static Time get_now() { Time t; t.fill_time(); return t; }

		/// Setter, getters
		int get_hour() const { return hour; }
		void set_hour(int h) { hour = h; }

		int get_minute() const { return minute; }
		void set_minute(int m) { minute = m; }

		int get_second() const { return second; }
		void set_second(int s) { second = s; }

		bool is_befor_origin() const { return befor_origin; }
		void set_befor_origin() { befor_origin = true; }
		void unset_befor_origin() { befor_origin = false; }

		/// Operators
		Time operator +(const Time& right);

		Time operator +=(const Time& right)
		{
			*this + right;
			return *this;
		}

		Time operator -(const Time& right) const
		{
			Time t = *this;
			t.set_total_seconds(t.get_total_seconds() - right.get_total_seconds());
			return t;
		}

		bool operator ==(const Time& right) const
		{
			return right.hour == hour
				&& right.minute == minute
				&& right.second == minute;
		}

		bool operator !=(const Time& right) const { return !(*this == right); }

		Time& operator =(const Time& t)
		{
			hour = t.hour;
			minute = t.minute;
			second = t.second;
			befor_origin = t.befor_origin;

			return *this;
		}

		bool operator <(const Time& right) const;
		bool operator >(const Time& right) const;

		bool operator <=(const Time& right) const { return *this == right || *this < right; }

		bool operator >=(const Time& right) const { return *this == right || *this > right; }

	private:
		int hour;
		int minute;
		int second;
		bool befor_origin;
	};

	class PausedTime {
	public:
		/// Methods
	/*	Time& get_duration() const {
			return end - start;
		}
		*/

		std::ostream& serialize(std::ostream& os) const;
		std::istream& deserialize(std::istream& is);

		std::ostream& print(std::ostream& os, std::string ind = "") const {
			os << ind << "Paused Time : " << std::endl;

			start.print(os, ind + '\t');

			end.print(os, ind + '\t');

			return os;
		}

		void reset() {
			start.reset();
			end.reset();
		}

		TimeLogger::Time start;
		TimeLogger::Time end;		/// If end time is all zero, then this record is not updated.
	};

	class Record {
		friend class TimeLogger;
	public:
		typedef std::list<TimeLogger::PausedTime> PausedTimeList;

		Record() {}

		/// Methods
		Time get_unuseful_time();
		void calculate_performance();

		bool is_commited() const { return !presence.end.is_zero(); }

		void add_pause();
		void add_resume();

		void reset()
		{
			date.reset();
			presence.reset();
			last_update_time.reset();
			performance = 0;
			paused_times.clear();
		}

		/// Setter, getters
		Date get_date() const { return date; }
		PausedTime get_presence() const { return presence; }
		Time get_last_update_time() const { return last_update_time; }
		int get_performance() const { return performance; }

		std::ostream& print(std::ostream& os, std::string ind = "") const;
		std::ostream& serialize(std::ostream& os) const;
		std::istream& deserialize(std::istream& is);

	private:
		Date date;
		PausedTime presence;
		Time last_update_time;
		Time corrupted_time;
		int performance;
		PausedTimeList paused_times;
	};

	bool open_print(std::string path);
	bool open_log_file();
	bool close_log_file();
	bool update_log();

	void add_pause()
	{
		record.add_pause();
	}

	void add_resume()
	{
		record.add_resume();
	}

private:
	/// Utility function
	std::string get_today_file_name() const;
	bool serialize();
	bool deserialize(std::string file_name, Record& record);
	bool deserialize(std::string file_name) { return deserialize(file_name, record); }

	Record record;
};

#endif

