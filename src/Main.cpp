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

#include <memory>
#include <iostream>

#include <qapplication.h>
#include <qwidget.h>

#include "TimeLogger.h"
#include "Singleton.h"
#include "SingletonApplication.h"

#ifdef __linux__
#include <signal.h>
#endif

Singleton<TimeLogger> time_logger;

#ifdef __linux__
void sig_handler(int /*sig*/)
{
	time_logger.get_instance()->close_log_file();
	exit(0);
}
#endif

int main(int argc, char** argv)
{
#ifdef __linux__
	signal(SIGINT, sig_handler);
	//signal(SIGKILL, sig_handler);
	//signal(SIGHUP, sig_handler);
	//signal(SIGTERM, sig_handler);
#endif

	std::unique_ptr<SingletonApplication> app;
	try {
		app = std::unique_ptr<SingletonApplication>(new SingletonApplication("perculator"));
	} catch (SingletonException* e) {
		std::cerr << e->what() << std::endl;
		delete e;
		return 1;
	}

	QApplication a(argc, argv);

	std::unique_ptr<Panel> window(new Panel());
	window->setMaximumSize(400, 150);
	window->setMinimumSize(400, 150);
	window->resize(400, 150);
	window->setWindowTitle(QString((char*)"PerCulator"));
	window->show();

	time_logger.get_instance()->open_log_file();

	return a.exec();
}

