#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QtSql>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

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
private:
    Ui::MainWindow *ui;
		QSystemTrayIcon *trayIcon;
		QMenu *trayIconMenu;
private slots:
	void on_actionConfigure_triggered();
 void on_actionGetNewWord_triggered();
	void on_lineEdit_textChanged(QString );
	void on_actionQuit_triggered();
};

#endif // MAINWINDOW_H
