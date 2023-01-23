#include "SuffixTree.h"
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <chrono>

struct Test {
    string TestString;
    string TestSubstring;
};
static const char pool[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

int poolSize = sizeof(pool) - 1;

/*Вспомогательная функция для RandomGeneralString,  выбирает случайный символ из массива символов pool*/
char getRandomChar()
{
    return pool[rand() % poolSize];
}

/*Генерирует в файл с именем filename  countString случайны[ строк длиной в диапозоне от lengthStringMin до lengthStringMax и для каждой строки подстроку
Пример  RandomGeneralString("RandomTest.txt", 10, 5, 15);  */
void RandomGeneralString(string fileName, int countString, int lengthStringMin, int lengthStringMax) 
{
    string fullFileNameOutput = "..\\..\\..\\Tests\\" + fileName;
    fstream fs(fullFileNameOutput, ios::out);
        Test test;
        srand(time(0));
        fs << countString << endl;
        for (int j = 0; j < countString; j++) {
            int lengthString = lengthStringMin + rand() % (lengthStringMax - 1);
            for (int i = 0; i < lengthString; i++) {
                test.TestString += getRandomChar();
            }
            int lengthSubstring = 1 + rand() % (lengthString - 1);
            for (int i = 0; i < lengthSubstring; i++) {
                test.TestSubstring += getRandomChar();
            }
            fs << test.TestString << endl << test.TestSubstring << endl;
            test.TestString = "";
            test.TestSubstring = "";
        }
        cout << "Test generated: OK" << endl;
        fs.close();
}



void TestSystem(string nameFile) {
    string fullNameFileInput = "..\\..\\..\\Tests\\" + nameFile;
    string fullNameFileOutput = "..\\..\\..\\Tests\\Ready" + nameFile ;
    fstream input(fullNameFileInput, ios::in);
    fstream output(fullNameFileOutput, ios::out);
    
    string countString;
    Test test;
    getline(input, countString);
    int IntCountString = stoi(countString);
  

    for (int i = 0; i < IntCountString; i++) {
        getline(input, test.TestString);
        getline(input, test.TestSubstring);
        bool check1, check2;
        output << "Test" << i + 1 << ": ";
        auto beginFind = chrono::steady_clock::now();
        if (test.TestString.find(test.TestSubstring) != string::npos) {
            check1 = true;
        }
        else {
            check1 = false;
        }
        auto endFind = chrono::steady_clock::now();
        auto timeWorkFind = std::chrono::duration_cast<std::chrono::nanoseconds>(endFind - beginFind);
        SuffixTree* NewSuffTree = new SuffixTree(test.TestString);
        auto beginFindSub = chrono::steady_clock::now();
        if (NewSuffTree->FindSub(test.TestSubstring)) {
            check2 = true;
        }
        else {
            check2 = false;
        }
        auto endFindSub = chrono::steady_clock::now();
        auto timeWorkFindSub = std::chrono::duration_cast<std::chrono::nanoseconds>(endFindSub - beginFindSub);
        output << countString << " " << timeWorkFind.count() << "nc " << timeWorkFindSub.count() << "ns ";
        output << check1 << " " << check2 << " ";
        if (check1 == check2) {
            output << "OK" << endl;
        }
        else {
            output << "NO" << endl;
        }
        delete NewSuffTree;
    }
    cout << "Open " << fullNameFileOutput;
    input.close();
    output.close();
}

int main()
{
    //RandomGeneralString("RandomTest.txt", 10, 1000, 1500);
    TestSystem("RandomTest.txt");


  /*
    string s, s1;
    bool check1, check2;
    cout << "write string" << endl;
    cin >> s;
    cout << "write substring" << endl;
    cin >> s1;
    if (s.find(s1) != string::npos) {
        check1 = true;
    }
    else {
        check1 = false;
    }
    SuffixTree* NewSuffTree = new SuffixTree(s);
    if (NewSuffTree->FindP(s1)) {
        check2 = true;
     }
    else {
        check2 = false;
    }
    cout << check1 << " " << check2 << " ";
    if (check1 == check2) {
        cout << "OK" << endl;
    }
    else {
        cout << "NO" << endl;
    }

    //NewSuffTree->SuffixOutput();
    delete NewSuffTree;
    
    */
}

