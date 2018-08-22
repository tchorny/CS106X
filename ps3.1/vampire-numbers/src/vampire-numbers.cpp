/**
 * File: vampire-numbers.cpp
 * -------------------------
 * Provides a program that repeatedly prompts the user for positive
 * numbers with an even number of digits and informs them
 * whether the number is a Vampire number or not.
 */

#include <string>    // for string
#include <iostream>  // for cout, endl
using namespace std;

#include "console.h" 
#include "simpio.h" // for getInteger
#include "strlib.h" // for stringToInteger, integerToString

int intLength (int num);
int ithDigit (int num, int i);
int restNum (int num, int i);
static bool isVampireNumber(int number, int& first, int& second, const int target);
static bool isVampireNumber(int number, int& first, int& second);

static int getPossibleVampireNumber() {
    while (true) {
        int response = getInteger("Enter a positive integer [or 0 to quit]: ");
        if (response >= 0) return response;
        cout << "Ah, sorry.  I need a nonnegative response.  Try again... " << endl;
    }
}

int intLength (int num) {
    if (num == 0) return 0;
    int i = 0;
    do {
        i++;
        num /= 10;
    } while (num != 0);

    return i;
}

int ithDigit (int num, int i) {

    for (int j = 1; j < i; j++) num /= 10;
    return num % 10;
}

int restNum (int num, int i) {

    int result = 0;
    for (int j = intLength(num); j > i; j--) result = 10* result + ithDigit(num, j);
    for (int j = i - 1; j > 0; j--) result = 10* result + ithDigit(num, j);
    return result;
}

static bool isVampireNumber(int number, int& first, int& second, const int target) {

    int numLen = intLength(number);
    int firstLen = intLength(first);
    int secondLen = intLength(second);

    if (numLen == 1) {

        if (firstLen <= secondLen) first = 10 * first + number;
        else second = 10 * second + number;

        if ((intLength(first) == intLength(second)) && (first * second == target)) return true;
        else {
            if (firstLen <= secondLen) first /= 10;
            else second /= 10;
            return false;
        }
    }

    for (int i = 1; i <= numLen; i++) {

        int digit = ithDigit(number, i);
        int rest = restNum(number, i);

        if (firstLen <= secondLen) first = 10 * first + digit;
        else second = 10 * second + digit;

        if (isVampireNumber(rest, first, second, target)) return true;
        else {
            if (firstLen <= secondLen) first /= 10;
            else second /= 10;
        }
    }

    return false;
}

static bool isVampireNumber(int number, int& first, int& second) {
    return isVampireNumber(number, first, second, number);
}

int main() {
    cout << "Here's a program that tells you whether or not a "
         << "number you enter is Vampire." << endl << endl;
    while (true) {
        int number = getPossibleVampireNumber();
        if (number == 0) break;
        int first = 0, second = 0;
        if (isVampireNumber(number, first, second)) {
            cout << "Woo! " << number << " is a Vampire number, and "
                 << first << " and " << second << " are its fangs." << endl << endl;
        } else {
            cout << "Nope! The number " << number << " isn't Vampire." << endl << endl;
        }
    }
    cout << endl;
    cout << "All done!" << endl;
    return 0;
}
