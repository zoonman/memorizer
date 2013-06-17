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
	ui->timeEditStart->setTime(timeBegin);
	timeEnd = settings.value("timeEnd",QVariant("22:00:00")).toTime();
	ui->timeEditFinish->setTime(timeEnd);
	newWordsPerDay = settings.value("newWordsPerDay",QVariant("5")).toUInt();
	ui->spinBoxNewWords->setValue(newWordsPerDay);
	minimalTimeInterval = settings.value("minimalTimeInterval",QVariant("10")).toUInt();
	ui->spinBoxIntervalBetweenWords->setValue(minimalTimeInterval);
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
{ /*
	if (trayIcon->isVisible()) {
		// hide();event->ignore();
	}*/

}

void MainWindow::on_actionQuit_triggered()
{
	qApp->quit();
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

/*

Learning algorithm
1. Read configs
2. Start timer
3. OnTime
	stop timer
	Lookup for learning words
	if learning_words_count = 0 getNewWord (increase new words timer)
	show word
	store time for this word



	Learning structure
	30 min after
	1 day
	10 days
	30
	60


	--

	1. Start PC
	2. Words per hour 1..5


*/



void MainWindow::on_toolButton_clicked()
{
	QString my;
	my = "http://translate.google.com/translate_tts?ie=UTF-8&q=___&tl=en&total=1&idx=0&textlen=5&prev=input";
	my = my.replace(QRegExp("___"), QUrl::toPercentEncoding(ui->lineEditNewWord->text(),"",""));
	qDebug() << my;
	player.setMedia(QUrl(my));
	player.play();
}

void MainWindow::on_comboBoxDictionary_currentTextChanged(const QString &arg1)
{
	settings.setValue("dictionary",arg1);
}

void MainWindow::on_spinBoxNewWords_valueChanged(int arg1)
{
	settings.setValue("newWordsPerDay",arg1);
}

void MainWindow::on_timeEditStart_timeChanged(const QTime &time)
{
	settings.setValue("timeBegin",time);
}

void MainWindow::on_timeEditFinish_timeChanged(const QTime &time)
{
	settings.setValue("timeEnd",time);
}

void MainWindow::on_spinBoxIntervalBetweenWords_valueChanged(int arg1)
{
	settings.setValue("minimalTimeInterval",arg1);
}
