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

#include "LcdPanel.h"

#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTimer>

#include "Panel.h"

LcdPanel::LcdPanel(char* txt, QWidget* /*parent*/)
: hour(0)
, minute(0)
, second(0)
{
	label = new QLabel(QString(txt), this);
	min_colon = new QLabel(QString((char*)":"), this);
	sec_colon = new QLabel(QString((char*)":"), this);

	lcd_h = new QLCDNumber(2, this);
	lcd_m = new QLCDNumber(2, this);
	lcd_s = new QLCDNumber(2, this);

	button = new QPushButton(QString((char*)"Stop"), this);
	button->setMaximumSize(100, 50);
	button->setMinimumSize(70, 0);
//	button->setAutoFillBackground(true);
//	QPalette p;
//	p.setColor(QPalette::Background, QColor(0, 255, 0));
//	button->setPalette(p);
	connect(button, SIGNAL(clicked()), this, SLOT(button_clicked()));

	timer = new QTimer(this);
	timer->setSingleShot(false);
	timer->setInterval(1000);
	connect(timer, SIGNAL(timeout()), this, SLOT(tick_rtc()));

	connect(this, SIGNAL(valueChanged()), this, SLOT(update_display()));

	QHBoxLayout* l = new QHBoxLayout(this);
	l->addWidget(label, 1);
	l->addWidget(lcd_h);
	l->addWidget(min_colon);
	l->addWidget(lcd_m);
	l->addWidget(sec_colon);
	l->addWidget(lcd_s);
	l->addWidget(button);
}

LcdPanel::~LcdPanel()
{
	delete label;
	delete min_colon;
	delete sec_colon;

	delete lcd_h;
	delete lcd_m;
	delete lcd_s;

	delete button;
	
	delete timer;
}

long LcdPanel::get_seconds()
{
	long secs = (hour * 60 * 60) + (minute * 60) + second;

	return secs;
}

int LcdPanel::get_second()
{
	return second;
}

int LcdPanel::get_minute()
{
	return minute;
}

int LcdPanel::get_hour()
{
	return hour;
}

void LcdPanel::start()
{
	timer->start();
}

void LcdPanel::stop()
{
	timer->stop();
}

void LcdPanel::update_clock()
{
	if (++second >= 60)
	{
		second = 0;
		if (++minute >= 60)
		{
			minute = 0;
			++hour;
		}
	}

	if (/*((minute % 5) == 0) &&*/ (second % 5 == 0) && label->text() == Panel::TOTAL_TXT)
		time_logger.get_instance()->update_log();
}

void LcdPanel::set_value(int h, int m, int s)
{
	hour   = h;
	minute = m;
	second = s;

	emit valueChanged();
}

void LcdPanel::tick_rtc()
{
	update_clock();
	emit valueChanged();
}

void LcdPanel::update_display()
{
	lcd_h->display(hour);
	lcd_m->display(minute);
	lcd_s->display(second);
}

void LcdPanel::button_clicked()
{
//	QPalette palette;

	if (timer->isActive())
	{
		timer->stop();
		button->setText(QString((char*)"Start"));
		if (label->text() == Panel::USEFUL_TXT)
		{
			time_logger.get_instance()->add_pause();
			time_logger.get_instance()->update_log();
		}

//		palette.setColor(QPalette::Background, QColor(0, 0, 255));
	}
	else
	{
		button->setText(QString((char*)"Stop"));
		timer->start();
		if (label->text() == Panel::USEFUL_TXT)
		{
			time_logger.get_instance()->add_resume();
			time_logger.get_instance()->update_log();
		}

//		palette.setColor(QPalette::Background, QColor(0, 255, 0));
	}

//	button->setPalette(palette);
}

