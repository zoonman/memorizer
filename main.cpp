#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName("Memorizer");
	a.setOrganizationDomain("www.zoonman.com");
	a.setOrganizationName("ZM Software");
	if (!QSystemTrayIcon::isSystemTrayAvailable()) {
		QMessageBox::critical(0, QObject::tr("Systray"),
													QObject::tr("I couldn't detect any system tray "
																			"on this system."));
		return 1;
	}
	QApplication::setQuitOnLastWindowClosed(false);

	MainWindow w;
	w.show();

	return a.exec();
}
