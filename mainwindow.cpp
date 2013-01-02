#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	trayIcon = new QSystemTrayIcon(this);
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(ui->actionGetNewWord);
	trayIconMenu->addSeparator();
	//trayIconMenu->addAction(ui->actionConfigure);
	//trayIconMenu->addSeparator();
	trayIconMenu->addAction(ui->actionQuit);
	trayIcon->setContextMenu(trayIconMenu);
	trayIcon->setIcon(QIcon(":/images/icon.png"));
	trayIcon->show();
	setWindowFlags(Qt::Dialog);
	QSettings settings;
	QString dictionary;
	dictionary = settings.value("dictionary",QVariant("English for Russians (TOP2000)")).toString();
	dictionary.append(".dict");

	if (!QSqlDatabase::drivers().contains("QSQLITE")) {
		QMessageBox::critical(this, "Unable to load database", "This application needs the SQLITE driver");
	}
	QFile dictFile(dictionary);
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	if (dictFile.exists()) {
		db.setDatabaseName(dictionary);
		if (!db.open()) {
			QMessageBox::critical(this,trUtf8("Error"), db.lastError().text());
			qApp->quit();
		}
		else {
			if (! tables.contains("mdict", Qt::CaseInsensitive)) {
				QSqlQuery q;
				q.exec(QLatin1String("create table mdict(id integer primary key autoincrement, foreignword varchar, transcription varchar, nativeword varchar, lastshow datetime, iteration integer default 0)"));
			}
		}
	}
	else {
		QMessageBox::warning(this, "Dictionary not found!", "You can obtain dictionary files "
													" from <a href=\"http://www.zoonman.com/projects/memorizer/\">Memorizer page</a>");
		qApp->quit();
	}
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
	if (trayIcon->isVisible()) {
		hide();
	}
}

void MainWindow::on_actionQuit_triggered()
{
	qApp->quit();
}

void MainWindow::on_lineEdit_textChanged(QString )
{
		//
}

void MainWindow::on_actionGetNewWord_triggered()
{
	show();
}
