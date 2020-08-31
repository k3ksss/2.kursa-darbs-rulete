#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QDateEdit>
#include <time.h>
#include <stdlib.h>
#include <QMessageBox>
#include <QInputDialog>
#include "functions.h"
#include <QTimer>
#include <QTime>
#include <QTableView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString username;
    int money;
    functions f;
    QString numberColor;
    int jackpotNumber, prize;
    QTimer *timer_1s;
    const int ADMINCODE = 1414;

    int colorX = 0;
    int numberX = 0;
    int betweennumberX = 0;
    bool bonuss = 0;
    int addmoneylimit = 0;

    bool canBet;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_registerBtn_login_clicked();

    void on_loginBtn_login_clicked();

    void on_pushButton_clicked();

    void on_registerBtn_register_clicked();

    void on_gotoLoginButton_register_clicked();

    void on_placeBetOnNumberButton_game_clicked();

    void on_ALLINButton_game_clicked();

    void on_depositMoneyButton_game_clicked();

    void on_placeBetOnNumberDiffButton_game_clicked();

    void on_placeBetOnColorsButton_game_clicked();

    void UpdateTime();

    void UpdateBonuss();

    void setGameSettings();

    void setGameSettingsInLabels();

    void saveWinBetInHistory(int won);

    void on_toAdminButton_game_clicked();

    void on_exitButton_game_clicked();

    void on_toGameButton_game_clicked();

    void on_loadDataButton_admin_clicked();

    void on_saveEditButton_admin_clicked();

    void on_loadHistoryButton_admin_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
};

#endif // MAINWINDOW_H
