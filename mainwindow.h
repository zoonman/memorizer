#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QObject>
#include <QtWidgets>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QMessageBox>
#include <QFile>
#include <QDir>
#include <QtSql>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtMultimedia/QMediaPlayer>
#include <QSettings>
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
		void closeEvent(QCloseEvent *event);
		void getWord();

private:
    Ui::MainWindow *ui;
		QSystemTrayIcon *trayIcon;
		QMenu *trayIconMenu;
		unsigned int wordsShowed;
		unsigned int newWordsPerDay;
		unsigned int minimalTimeInterval;
		QTime timeBegin;
		QTime timeEnd;
		QDate currentDate;
		QSettings settings;
		QString dictionary;
		unsigned int wordId;
		QMediaPlayer player;

private slots:
	void on_actionConfigure_triggered();
	void on_actionGetNewWord_triggered();
	void on_actionQuit_triggered();

	void on_toolButton_clicked();
	void on_comboBoxDictionary_currentTextChanged(const QString &arg1);
	void on_spinBoxNewWords_valueChanged(int arg1);
	void on_timeEditStart_timeChanged(const QTime &time);
	void on_timeEditFinish_timeChanged(const QTime &time);
	void on_spinBoxIntervalBetweenWords_valueChanged(int arg1);
};

#endif // MAINWINDOW_H
