#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QMainWindow>
#include <time.h>
#include <stdlib.h>

class functions
{
public:
    functions();
    void getColorOfNumber(QString &numberColor, int num);
    int getColorOfNumber(int num);
    void generateRandomJackpot(int &jackpotNumber);
    bool checkIfInInputIsNumber(QString text);
};

#endif // FUNCTIONS_H
