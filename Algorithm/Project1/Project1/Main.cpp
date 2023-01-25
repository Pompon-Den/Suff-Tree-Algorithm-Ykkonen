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
 

/* Функция тестирующая входные данные из файла nameFile с оценкой времени работы построения дерева в файл
с именемReady + nameFile*/
void TestSystem(string nameFile) { 
    string fullNameFileInput = "..\\..\\..\\Tests\\" + nameFile + ".txt";
    string fullNameFileOutput = "..\\..\\..\\Tests\\Ready" + nameFile + ".txt";
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
        auto beginSuffixTreeBuild = chrono::steady_clock::now();
        SuffixTree* NewSuffTree = new SuffixTree(test.TestString);
        auto endSuffixTreeBuild = chrono::steady_clock::now();
        auto timeWorkSuffixTreeBuild = std::chrono::duration_cast<std::chrono::nanoseconds>( endSuffixTreeBuild - beginSuffixTreeBuild );
        auto beginFindSub = chrono::steady_clock::now();
        bool FindSubstring = NewSuffTree->FindSub(test.TestSubstring);
        auto endFindSub = chrono::steady_clock::now();
        auto timeWorkFindSub = std::chrono::duration_cast<std::chrono::nanoseconds>(endFindSub - beginFindSub);
        if (FindSubstring) {
            check2 = true;
        }
        else {
            check2 = false;
        }
        //output << test.TestString << " " << test.TestSubstring << " ";
        output << "Time Build " << timeWorkSuffixTreeBuild.count() << "ns " << timeWorkFindSub.count() << "ns ";
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

/* Тестирует файл со строками фиксированной длины для дальнейшего использования
в построение графиков*/
void testForChart(string nameFile) {
    string fullNameFileInput = "..\\..\\..\\Tests\\" + nameFile + ".txt";
    string fullNameFileOutput = "..\\..\\..\\Tests\\Ready" + nameFile + ".txt";
    fstream input(fullNameFileInput, ios::in);
    fstream output(fullNameFileOutput, ios::out);

    string countString;
    Test test;
    getline(input, countString);
    int IntCountString = stoi(countString);


    for (int i = 0; i < IntCountString; i++) {
        getline(input, test.TestString);

        auto beginSuffixTreeBuild = chrono::steady_clock::now();
        SuffixTree* NewSuffTree = new SuffixTree(test.TestString);
        auto endSuffixTreeBuild = chrono::steady_clock::now();
        auto timeWorkSuffixTreeBuild = chrono::duration_cast<std::chrono::microseconds>(endSuffixTreeBuild - beginSuffixTreeBuild);

        output << test.TestString.length() << " " << timeWorkSuffixTreeBuild.count() << endl;
        delete NewSuffTree;
    }
    cout << "Open " << fullNameFileOutput;
    input.close();
    output.close();
}
  
/* Генерирует файл с countString  не случайных строк длиной полученой из названия файла, например GeneralString("100", 10)
 сгенирирует файл с 10 строками длины равной 100 символов
 изпользуется для генерации тестов для составления графиков*/
void GeneralString(string fileName, int countString)
{
    string fullFileNameOutput = "..\\..\\..\\Tests\\" + fileName + ".txt";
    fstream fs(fullFileNameOutput, ios::out);
    Test test;
    int lengthString = stoi(fileName);
    srand(time(0));
    fs << countString << endl;
    for (int j = 0; j < countString; j++) {
        for (int i = 0; i < lengthString; i++) {
            test.TestString += getRandomChar();
        }
        fs << test.TestString << endl;
        test.TestString = "";
    }
    cout << "Test generated: OK" << endl;
    fs.close();
}

int main()
{
    //RandomGeneralString("RandomTest.txt", 100, 1000, 3000);
    TestSystem("manualTests");
    //GeneralString("10", 100);
    //testForChart("10");

}

