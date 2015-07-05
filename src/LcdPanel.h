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

#ifndef LCD_PANEL_H
#define LCD_PANEL_H

#include <QWidget>

#include "TimeLogger.h"
#include "Singleton.h"

class QLabel;
class QLCDNumber;
class QPushButton;
class QTimer;

extern Singleton<TimeLogger> time_logger;

class LcdPanel : public QWidget
{
	Q_OBJECT
public:
	LcdPanel(char* txt, QWidget* parent = 0);
	~LcdPanel();

	long get_seconds();
	int get_second();
	int get_minute();
	int get_hour();
	void start();
	void stop();

public slots:
	void set_value(int, int, int);
	void button_clicked();
	void tick_rtc();
	void update_display();

signals:
	void valueChanged();

private:
	void update_clock();

	int hour;
	int minute;
	int second;

	QLabel* label;
	QLabel* min_colon;
	QLabel* sec_colon;

	QLCDNumber* lcd_h;
	QLCDNumber* lcd_m;
	QLCDNumber* lcd_s;

	QPushButton* button;

	QTimer* timer;
};

#endif
