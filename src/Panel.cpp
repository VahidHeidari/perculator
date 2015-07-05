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

#include "Panel.h"

#include "LcdPanel.h"

#include <QVBoxLayout>
#include <QProgressBar>
#include <QTimer>

constexpr char* Panel::TOTAL_TXT;
constexpr char* Panel::USEFUL_TXT;

Panel::Panel(QWidget*)
{
	total  = new LcdPanel(TOTAL_TXT);
	useful = new LcdPanel(USEFUL_TXT);

	performance = new QProgressBar(this);
	performance->setMaximum(100);
	performance->setMinimum(0);
	performance->setValue(100);

	timer = new QTimer(this);
	timer->setInterval(1000);
	timer->setSingleShot(false);
	connect(timer, SIGNAL(timeout()), this, SLOT(update_performance()));
	timer->start();

	QVBoxLayout* l = new QVBoxLayout(this);
	l->addWidget(total);
	l->addWidget(useful);
	l->addWidget(performance);

	total->start();
	useful->start();
}

Panel::~Panel()
{
	delete total;
	delete useful;

	delete performance;

	delete timer;
}

void Panel::update_performance()
{
	long total_secs = total->get_seconds();
	long useful_secs = useful->get_seconds();

	int performance_percent = 100 * ((float)useful_secs / (float)total_secs);

	performance->setValue(performance_percent);
}

