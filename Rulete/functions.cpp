#include "functions.h"

functions::functions()
{

}

void functions::getColorOfNumber(QString &numberColor, int num){
    if(num == 0){//green
        numberColor = "QLabel{color: rgb(0,250,0);}";
    }else if(num == 1 || num == 3 || num == 5 || num == 7 || num == 9 || num == 11 || num == 13 || num == 15 || num == 17){//red
        numberColor = "QLabel{color: rgb(255, 0, 0);}";
    }else {
        numberColor = "QLabel{color: rgb(0, 0, 0);}";
    }
}

int functions::getColorOfNumber(int num){
    if(num == 0){//green
        return 2;
    }else if(num == 1 || num == 3 || num == 5 || num == 7 || num == 9 || num == 11 || num == 13 || num == 15 || num == 17){//red
        return 1;
    }else {
        return 0;
    }
}

void functions::generateRandomJackpot(int &jackpotNumber){
    qrand();
    jackpotNumber = qrand() % 36 + 0;
}

bool functions::checkIfInInputIsNumber(QString text){
    bool success;
    text.toInt(&success, 10);

    if (success) {
        return true;
    }
    else {
        return false;
    }
}
