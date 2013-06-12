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
	trayIconMenu->addAction(ui->actionConfigure);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(ui->actionQuit);
	trayIcon->setContextMenu(trayIconMenu);
	trayIcon->setIcon(QIcon(":/images/icon.png"));
	trayIcon->show();
	setWindowFlags(Qt::Dialog);
	// Load configuration
	dictionary = settings.value("dictionary",QVariant("English for Russians (TOP2500)")).toString();
	dictionary.append(".dict");
	timeBegin = settings.value("timeBegin",QVariant("19:00:00")).toTime();
	timeEnd = settings.value("timeEnd",QVariant("22:00:00")).toTime();
	newWordsPerDay = settings.value("newWordsPerDay",QVariant("5")).toUInt();
	minimalTimeInterval = settings.value("minimalTimeInterval",QVariant("10")).toUInt() * 10;
	// Plug Database
	if (!QSqlDatabase::drivers().contains("QSQLITE")) {
		QMessageBox::critical(this, "Unable to load database", "This application needs the SQLITE driver");
	}
	// Looking up for dictionary files
	QDir dir = QDir::current();
	QStringList filters;
	filters << "*.dict";
	dir.setNameFilters(filters);
	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		ui->comboBoxDictionary->addItem(fileInfo.fileName().left(fileInfo.fileName().length()-5), fileInfo.fileName());
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
			QStringList tables = db.tables();
			if (! tables.contains("mdict", Qt::CaseInsensitive)) {
				QSqlQuery query;
				query.exec(QLatin1String("create table mdict(id integer autoincrement primary key, foreignword varchar, transcription varchar, nativeword varchar, lastshow datetime, iteration integer default 0)"));
			}
			else {
				// Everything is good
				// Now we can learn
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
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actionConfigure_triggered()
{
	ui->stackedWidget->setCurrentIndex(1);
}
