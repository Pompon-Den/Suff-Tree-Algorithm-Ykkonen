#include "SuffixTree.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

int main()
{
    string s;
    vector <int> SuffTree;
    cout << "Hello World!" << endl;
    cout << "write sting:" << endl;
    cin >> s;
    SuffixTree* NewSuffTree = new SuffixTree(s);
    NewSuffTree->CountIndex(NewSuffTree->root, SuffTree);
    for (int i = 0; i < SuffTree.size(); i++) {
        cout << "\"";
        for (int j = 0; j < SuffTree[i]; j++) {
            cout << s[j];
        }
        cout << "\"";
        cout << endl;
    }
    // cout << "write podstoka" << endl;
    // string p;
    //cin >> p;
   // NewSuffTree->SearhP(p, s);

    delete NewSuffTree;
}