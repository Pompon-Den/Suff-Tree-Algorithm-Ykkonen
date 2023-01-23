#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#define TERMINATION_SYMBOL '$'

using namespace std;

class SuffixTree;

class Node
{
public:
    friend SuffixTree;

    // конструктор ноды без индекса: используется при создании новых внутренних вершин
    Node(Node* link, int start, int* end) : Node(link, start, end, -1)
    {
    }

    // конструктор с индексом: исп-ся при создании листьев
    Node(Node* link, int start, int* end, int ind) : suffix_link(link),
        start(start),
        end(end),
        suffix_index(ind)
    {
    }


    map<char, Node*> children; 
    Node* suffix_link;          
    int start;                  
    int* end;                   
    int suffix_index;           
};

class SuffixTree
{
public:
    SuffixTree(string& text);             // конструктор по строке
    void BuildSuffixTree();               // построение дерева
    ~SuffixTree()                         // деструктор
    {
        DeleteSuffixTree(root);
    }


    void ExtendSuffixTree(int pos);   
    void DeleteSuffixTree(Node* node) 
    {
        for (auto it : node->children)
            DeleteSuffixTree(it.second);
        if (node->suffix_index == -1)
            delete node->end;
        delete node;
    }
    void CountIndex(Node* node, vector<int>& vec); 
    int EdgeLength(Node* node);                    // длина дуги
    bool FindSub(string Pstr);                       // поиск подстроки в дереве
    void SuffixOutput();                        // вывод всех суффиксов


    Node* root = new Node(nullptr, -1, new int(-1));
    Node* lastCreatedInternalNode = nullptr;         

    string text; 

    Node* activeNode = nullptr;   // вершина для расширения из неё
    int activeEdge = -1;          // индекс символа, который задает движение из текущей ноды
    int activeLength = 0;         // на сколько символов ступаем в направлении activeEdge
    int remainingSuffixCount = 0; // сколько суффиксов осталось создать
    int leafEnd = -1;             // глобальная переменная, определяет содержимое листьев: её инкремент как бы приписывает новый символ к ним
};