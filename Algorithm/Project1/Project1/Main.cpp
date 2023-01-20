#include "SuffixTree.h"
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <chrono>

struct Test {
    string TestString;
    string TestSubstring;
};
int TestCount = 10;
static const char pool[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

int poolSize = sizeof(pool) - 1;

char getRandomChar()
{
    return pool[rand() % poolSize];
}


void RandomGeneralString(int len) 
{
    fstream fs("..\\..\\..\\Tests\\RandomString.txt", ios::out);
        Test test;
        srand(time(0));
        for (int j = 0; j < TestCount; j++) {
            int lengthString = 1 + rand() % (len - 1);
            for (int i = 0; i < lengthString; i++) {
                test.TestString += getRandomChar();
            }
            int lengthSubstring = 1 + rand() % (lengthString);
            for (int i = 0; i < lengthSubstring; i++) {
                test.TestSubstring += getRandomChar();
            }
            fs << test.TestString << "  " << test.TestSubstring << endl;
            test.TestString = "";
            test.TestSubstring = "";
        }
        cout << "Test generated: OK" << endl;
        fs.close();
}

void TestSystemRandonString() {
    //RandomGeneralString(10);
    fstream inputStrings("..\\..\\..\\Tests\\Test4.txt", ios::in);
    fstream outputTests("..\\..\\..\\Tests\\Test4Result.txt", ios::out);
    for (int i = 0; i < 4; i++) {
        string TestString;
        string TestSubstring;
        bool check;
        inputStrings >> TestString  ;
        getline(inputStrings ,TestSubstring);
        outputTests << "Test" << i+1 << ": ";
        double start_timeLowAlgorithm = clock();
        if (TestString.find(TestSubstring) != string::npos) {
            check = true;
        }
        else {
            check = false;
        }
        double end_timeLowAlgorithm = clock();
        SuffixTree* NewSuffTree = new SuffixTree(TestString);
        double start_time = clock();
        bool check2 = NewSuffTree->FindP(TestSubstring);
        double end_time = clock();
        outputTests << " " << check << " " << check2 << " "; // скрываемое поле проверки на верность работы алгоритма
        if (check == check2 ) {
            outputTests << "OK";
        }
        else {
            outputTests << "NO";
        }
        delete NewSuffTree;
        if ((end_time - start_time) <= (end_timeLowAlgorithm - start_timeLowAlgorithm)) {
            outputTests << " time FindSubstring: OK " << end_time - start_time << "  " << end_timeLowAlgorithm - start_timeLowAlgorithm << endl;
        }
        else {
            outputTests << " time FindSubstring: NO " << end_time - start_time << "  " << end_timeLowAlgorithm - start_timeLowAlgorithm << endl;
        }
    }
    inputStrings.close();
    outputTests.close();
}

int main()
{
 
    //vector <int> VecSuffTree;
    TestSystemRandonString();
    //RandomGeneralString(1000000);
    cout << "Open Tests.txt"  << endl;
   
    string s, s1;
    cout << "write string" << endl;
    cin >> s;
    cout << "write substring" << endl;
    cin >> s1;
    SuffixTree* NewSuffTree = new SuffixTree(s);
    if (NewSuffTree->FindP(s1)) {
        cout << "OK" << NewSuffTree->FindP(s1) << endl;
     }
    else {
        cout << "NO" << NewSuffTree->FindP(s1) << endl;
    }
    //NewSuffTree->SubstringOutput();
    delete NewSuffTree;
}

