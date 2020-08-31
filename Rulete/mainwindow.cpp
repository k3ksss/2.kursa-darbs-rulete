#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

    canBet = true;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("J:/Skola/Programmesana/2.Kurss/Rulete/database.db");

    timer_1s = new QTimer(this);
    QObject::connect(timer_1s, SIGNAL(timeout()), this, SLOT(UpdateTime()));
    timer_1s->start(1000);
    QObject::connect(timer_1s, SIGNAL(timeout()), this, SLOT(UpdateBonuss()));

    if(db.open())
        qDebug()<<("CONNECTED TO DATABASE!");
    else
        qDebug()<<("FAILED CONNECT TO DATABASE!");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_registerBtn_login_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->usernameF_register->clear();
    ui->passwordF_register->clear();
    ui->nameF_register->clear();
    ui->surnameF_register->clear();
    ui->personalcodeF_register->clear();
    ui->birthdaydateEdit_register->clear();
}

void MainWindow::on_loginBtn_login_clicked()
{
    QSqlQuery qry;
    db.open();
    QString password;

    username = ui->usernameF_login->text();
    password = ui->passwordF_login->text();

    if(qry.exec("SELECT * FROM user WHERE username='"+username+"' and password='"+password+"'")){
        int c = 0;
        while(qry.next()) {
            c++;
        }
        if(c == 1){
            qry.exec("SELECT money FROM user WHERE username='"+username+"'");
            while(qry.next()){
                ui->moneyLabel_game->setText(qry.value(0).toString());
                money = qry.value(0).toInt();
            }
            setGameSettings();
            ui->stackedWidget->setCurrentIndex(2);

        }else {
            ui->wrongL_login->setText("Lietotajvards un parole nepareiza!");
        }
    }
    db.close();
}

void MainWindow::on_pushButton_clicked()//reg to log
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->usernameF_login->clear();
    ui->passwordF_login->clear();
}

void MainWindow::on_registerBtn_register_clicked()
{
    QSqlQuery qry;
    db.open();
    QString username, password, name, surname, personalcode;

    username = ui->usernameF_register->text();
    password = ui->passwordF_register->text();
    name = ui->nameF_register->text();
    surname = ui->surnameF_register->text();
    personalcode = ui->personalcodeF_register->text();

    int year = ui->birthdaydateEdit_register->date().year();
    int diffyear = 2019-year;

    ui->errorL_register->clear();

    if(diffyear >= 18 && f.checkIfInInputIsNumber(personalcode) && password.length() >= 8){
        if(qry.exec("INSERT INTO user(name,surname,username,password,personalcode,money)""VALUES('"+name+"','"+surname+"','"+username+"','"+password+"','"+personalcode+"','5')")){
            ui->errorL_register->setText("Reģistrācija veiksmīga!");
        }else {
            ui->errorL_register->setText("Reģistrācija neveiksmīga!");
        }
    }else{
        ui->errorL_register->setText("Mēģiniet vēlreiz..");
    }

    db.close();
}

void MainWindow::on_gotoLoginButton_register_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->usernameF_login->clear();
    ui->passwordF_login->clear();
}

void MainWindow::on_placeBetOnNumberButton_game_clicked()
{
    int betMoney = ui->placeBetMoneyLineEdit_game->text().toInt();
    int betNumber = ui->numberLineEdit_game->text().toInt();
    ui->numberLineEdit_game->clear();
    int luckyNumber = 0;
    int won = 0;

    f.generateRandomJackpot(jackpotNumber);
    ui->BonusLabel_game->setText(QString::number(prize));
    qDebug()<<(jackpotNumber);

    QSqlQuery qry;
    db.open();

    if(money >= betMoney && betNumber >= 0 && betNumber <= 36 && money > 0 && betMoney > 0 && f.checkIfInInputIsNumber(ui->placeBetMoneyLineEdit_game->text()) && canBet){
        canBet = false;
        for(int x = 0;x <= 3; x++){
            qrand();
            luckyNumber = qrand() % 36 + 0;
            ui->numberLabel_game->setText(QString::number(luckyNumber));
            f.getColorOfNumber(numberColor,luckyNumber);
            ui->numberLabel_game->setStyleSheet(numberColor);
            QEventLoop loop;
            QTimer::singleShot(1000, &loop, SLOT(quit()));
            loop.exec();
        }
        if(luckyNumber == betNumber){
            if(jackpotNumber == luckyNumber && bonuss){
                won = (betMoney * numberX) + prize;
                saveWinBetInHistory(won - betMoney);
                QMessageBox::information(this,"Jūs uzvarējāt!","Jūs uzvarējāt "+QString::number(won)+"$!!!",QMessageBox::Ok);
                money = money + (won - betMoney);
                ui->moneyLabel_game->setText(QString::number(money));
                qry.exec("UPDATE user SET money='"+QString::number(money)+"' WHERE username='"+username+"'");
            }else{
                won = betMoney * numberX;
                saveWinBetInHistory(won - betMoney);
                QMessageBox::information(this,"Jūs uzvarējāt!","Jūs uzvarējāt "+QString::number(won)+"$!!!",QMessageBox::Ok);
                money = money + (won - betMoney);
                ui->moneyLabel_game->setText(QString::number(money));
                qry.exec("UPDATE user SET money='"+QString::number(money)+"' WHERE username='"+username+"'");
            }
        }else {
            if(jackpotNumber == luckyNumber && bonuss){
                won = prize;
                saveWinBetInHistory(won - betMoney);
                QMessageBox::information(this,"Jūs uzvarējāt!","Jūs uzvarējāt "+QString::number(won)+"$!!!",QMessageBox::Ok);
                money = money + won;
                ui->moneyLabel_game->setText(QString::number(money));
                qry.exec("UPDATE user SET money='"+QString::number(money)+"' WHERE username='"+username+"'");
            }else{
                QMessageBox::critical(this,"Jūs zaudējāt!","Jūs zaudējāt "+QString::number(betMoney)+"$!!!",QMessageBox::Ok);
                money = money - betMoney;
                ui->moneyLabel_game->setText(QString::number(money));
                qry.exec("UPDATE user SET money='"+QString::number(money)+"' WHERE username='"+username+"'");
            }
        }
    }else {
        QMessageBox::critical(this,"ERROR","Nevar uzlikt likmi!",QMessageBox::Ok);
    }
    canBet = true;
    db.close();
    ui->placeBetMoneyLineEdit_game->clear();
}

void MainWindow::on_ALLINButton_game_clicked()
{
    ui->placeBetMoneyLineEdit_game->setText(QString::number(money));
}

void MainWindow::on_depositMoneyButton_game_clicked()
{
    bool ok;
    QString deposit = QInputDialog::getText(this,"Iemaksāt naudu","Ievadiet summu, kuru velaties ieskaitīt savā kontā",QLineEdit::Normal,"",&ok);

    if(ok && deposit.toInt() >= 0 && deposit.toInt() <= addmoneylimit && f.checkIfInInputIsNumber(deposit)){
        QSqlQuery qry;
        db.open();
        money = deposit.toInt() + money;
        ui->moneyLabel_game->setText(QString::number(money));
        qry.exec("UPDATE user SET money='"+QString::number(money)+"' WHERE username='"+username+"'");
        QMessageBox::information(this,"Informācija","Nauda ieskaitīta veiksmīgi!",QMessageBox::Ok);
    }else {
        QMessageBox::critical(this,"ERROR","Nauda ieskaitīšana neizdevās, mēģiniet vēlreiz!",QMessageBox::Ok);
    }
    db.close();
}

void MainWindow::on_placeBetOnNumberDiffButton_game_clicked()
{
    int betMoney = ui->placeBetMoneyLineEdit_game->text().toInt();
    bool lessthannine;
    int luckyNumber = 0;
    int won = 0;

    QSqlQuery qry;
    db.open();

    if(ui->numberDiffComboBox_game->currentIndex() == 0){
        lessthannine = true;
    }else {
        lessthannine = false;
    }

    if(money >= betMoney && money > 0 && betMoney > 0 && f.checkIfInInputIsNumber(ui->placeBetMoneyLineEdit_game->text()) && canBet){
        canBet = false;
        for(int x = 0;x <= 3; x++){
            qrand ();
            luckyNumber = qrand() % 36 + 0;
            ui->numberLabel_game->setText(QString::number(luckyNumber));
            f.getColorOfNumber(numberColor,luckyNumber);
            ui->numberLabel_game->setStyleSheet(numberColor);
            QEventLoop loop;
            QTimer::singleShot(1000, &loop, SLOT(quit()));
            loop.exec();
        }

        if((lessthannine && luckyNumber <= 18) || (!lessthannine && luckyNumber >= 19)){
            won = betMoney * betweennumberX;
            saveWinBetInHistory(won - betMoney);
            QMessageBox::information(this,"Jūs uzvarējāt!","Jūs uzvarējāt "+QString::number(won)+"$!!!",QMessageBox::Ok);
            money = money + betMoney;
            ui->moneyLabel_game->setText(QString::number(money));
            qry.exec("UPDATE user SET money='"+QString::number(money)+"' WHERE username='"+username+"'");

        }else {
            QMessageBox::critical(this,"Jūs zaudējāt!","Jūs zaudējāt "+QString::number(betMoney)+"$!!!",QMessageBox::Ok);
            money = money - betMoney;
            ui->moneyLabel_game->setText(QString::number(money));
            qry.exec("UPDATE user SET money='"+QString::number(money)+"' WHERE username='"+username+"'");
        }
    }else {
        QMessageBox::critical(this,"ERROR","Nevar uzlikt likmi!",QMessageBox::Ok);
    }
    canBet = true;
    db.close();
    ui->placeBetMoneyLineEdit_game->clear();
}

void MainWindow::on_placeBetOnColorsButton_game_clicked()
{
    int betMoney = ui->placeBetMoneyLineEdit_game->text().toInt();
    int color = ui->colorComboBox_game->currentIndex(); // black 0, red 1, green 2
    int luckyNumber = 0;
    int won = 0;

    QSqlQuery qry;
    db.open();

    if(money >= betMoney && money > 0 && betMoney > 0 && f.checkIfInInputIsNumber(ui->placeBetMoneyLineEdit_game->text()) && canBet){
        canBet = false;
        for(int x = 0;x <= 3; x++){
            qrand();
            luckyNumber = qrand() % 36 + 0;
            ui->numberLabel_game->setText(QString::number(luckyNumber));
            f.getColorOfNumber(numberColor,luckyNumber);
            ui->numberLabel_game->setStyleSheet(numberColor);
            QEventLoop loop;
            QTimer::singleShot(1000, &loop, SLOT(quit()));
            loop.exec();
        }

        if(f.getColorOfNumber(luckyNumber) == color){
            won = betMoney * colorX;
            saveWinBetInHistory(won - betMoney);
            QMessageBox::information(this,"Jūs uzvarējāt!","Jūs uzvarējāt "+QString::number(won)+"$!!!",QMessageBox::Ok);
            money = money + (won - betMoney);
            ui->moneyLabel_game->setText(QString::number(money));
            qry.exec("UPDATE user SET money='"+QString::number(money)+"' WHERE username='"+username+"'");

        }else {
            QMessageBox::critical(this,"Jūs zaudējāt!","Jūs zaudējāt "+QString::number(betMoney)+"$!!!",QMessageBox::Ok);
            money = money - betMoney;
            ui->moneyLabel_game->setText(QString::number(money));
            qry.exec("UPDATE user SET money='"+QString::number(money)+"' WHERE username='"+username+"'");
        }
    }else {
        QMessageBox::critical(this,"ERROR","Nevar uzlikt likmi!",QMessageBox::Ok);
    }
    canBet = true;
    db.close();
    ui->placeBetMoneyLineEdit_game->clear();
}

void MainWindow::UpdateTime(){
    ui->dateLabel_game->setText(QTime::currentTime().toString("hh:mm:ss"));
}

void MainWindow::UpdateBonuss(){
    qrand();
    prize = qrand() % 999 + 1;
    ui->BonusLabel_game->setText(QString::number(prize));
}

void MainWindow::on_toAdminButton_game_clicked()
{
    bool ok;
    QString pass = QInputDialog::getText(this,"ADMIN","Ievadiet administrātora paroli:",QLineEdit::Normal,"",&ok);

    if(ok && pass == QString::number(ADMINCODE)){
        ui->stackedWidget->setCurrentIndex(3);
        setGameSettingsInLabels();
    }else {
        QMessageBox::critical(this,"ERROR","Administrātora parole nav pareiza, mēģiniet vēlreiz!",QMessageBox::Ok);
    }
}

void MainWindow::on_exitButton_game_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->usernameF_login->clear();
    ui->passwordF_login->clear();
}

void MainWindow::on_toGameButton_game_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_loadDataButton_admin_clicked()
{
    setGameSettings();
    setGameSettingsInLabels();
}

void MainWindow::setGameSettingsInLabels(){
    ui->colorXLabel_admin->setText(QString::number(colorX));
    ui->numberXLabel_admin->setText(QString::number(numberX));
    ui->diffnumbXLabel_admin->setText(QString::number(betweennumberX));
    ui->bonusBOOLLabel_admin->setText(QString::number(bonuss));
    ui->addmoneylimitLable_admin->setText(QString::number(addmoneylimit));
}

void MainWindow::setGameSettings(){
    QSqlQuery qry;
    db.open();

    qry.prepare("SELECT colorX, numberX, bonuss, diffbetweennumbersX, addmoneylimit FROM gameSettings");

    if(qry.exec()){
        while(qry.next()){
            colorX = qry.value(0).toInt();
            numberX = qry.value(1).toInt();
            betweennumberX = qry.value(3).toInt();
            bonuss = qry.value(2).toInt();
            addmoneylimit = qry.value(4).toInt();
        }
    }
    db.close();
}

void MainWindow::on_saveEditButton_admin_clicked()
{
    QString changeColorX = ui->colorXlineEdit_admin->text(), changeNumberX = ui->numberXlineEdit_admin->text(),
            changeDiffBetweenNumbersX = ui->diffnumbXlineEdit_admin->text(), changeBonuss = ui->bonusBOOLlineEdit_admin->text(),
            changeAddMoneyLimit = ui->addmoneylimitlineEdit_admin->text();

    QSqlQuery qry;
    db.open();

    bool ok = false;

    if(changeColorX != "" && f.checkIfInInputIsNumber(changeColorX)) {
        ok = qry.exec("UPDATE gameSettings SET colorX='"+changeColorX+"' WHERE ID='1'");
    }
    if(changeNumberX != "" && f.checkIfInInputIsNumber(changeNumberX)) {
        ok = qry.exec("UPDATE gameSettings SET numberX='"+changeNumberX+"' WHERE ID='1'");
    }
    if(changeDiffBetweenNumbersX != "" && f.checkIfInInputIsNumber(changeDiffBetweenNumbersX)) {
        ok = qry.exec("UPDATE gameSettings SET diffbetweennumbersX='"+changeDiffBetweenNumbersX+"' WHERE ID='1'");
    }
    if(changeBonuss != "" && f.checkIfInInputIsNumber(changeBonuss)) {
        ok = qry.exec("UPDATE gameSettings SET bonuss='"+changeBonuss+"' WHERE ID='1'");
    }
    if(changeAddMoneyLimit != "" && f.checkIfInInputIsNumber(changeAddMoneyLimit)) {
        ok = qry.exec("UPDATE gameSettings SET addmoneylimit='"+changeAddMoneyLimit+"' WHERE ID='1'");
    }

    ui->colorXlineEdit_admin->clear();
    ui->numberXlineEdit_admin->clear();
    ui->diffnumbXlineEdit_admin->clear();
    ui->bonusBOOLlineEdit_admin->clear();
    ui->addmoneylimitlineEdit_admin->clear();

    db.close();
    setGameSettings();
    setGameSettingsInLabels();

    if(ok){
        QMessageBox::information(this,"Informācija","Veiksmīgi saglabāts datubāzē!",QMessageBox::Ok);
    }else {
        QMessageBox::critical(this,"ERROR","Neizdevās saglabāt, mēģiniet vēlreiz!",QMessageBox::Ok);
    }
}

void MainWindow::saveWinBetInHistory(int won){
    QSqlQuery qry;
    db.open();

    qry.exec("INSERT INTO betHistory(username,won)""VALUES('"+username+"','"+QString::number(won)+"')");

    db.close();
}

void MainWindow::on_loadHistoryButton_admin_clicked()
{
    QSqlQuery qry;
    QSqlQueryModel * model = new QSqlQueryModel;
    QTableView * table = ui->HistoryTableView_admin;

    db.open();

    qry.exec("SELECT username, won FROM betHistory ORDER BY won DESC");
    model->setQuery(qry);
    table->setModel(model);
    db.close();
}
