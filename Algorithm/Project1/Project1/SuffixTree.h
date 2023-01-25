/*
Этот заголовочный файл содержит библтотеку С++ с реализацией алгоритма Укконена

    Построение суффиксного дерево по строке:
        SuffixTree* название = new SuffixTree(название_строки);

    Деструктор - освобождает память
        delete название ;

    Поиск подстроки в суффиксном дереве, возвращает булевое значение:
        название->FindSub(название_строки);

    Вывод всех суффиксов строки в дереве в консоль:
        название->SuffixOutput();

*/

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

    Node(Node* link, int start, int* end) : Node(link, start, end, -1)
    {
    }

    Node(Node* link, int start, int* end, int ind) : suffix_link(link),
        start(start),
        end(end),
        suffix_index(ind)
    {
    }
private:
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
    void BuildSuffixTree();               
    ~SuffixTree()                        
    {
        DeleteSuffixTree(root);
    }
 
    bool FindSub(string Pstr);
    void SuffixOutput();

private:
    void DeleteSuffixTree(Node* node) 
    {
        for (auto it : node->children)
            DeleteSuffixTree(it.second);
        if (node->suffix_index == -1) 
            delete node->end;
        delete node;
    }              

    void ExtendSuffixTree(int pos);
    void CountIndex(Node* node, vector<int>& vec);
    int EdgeLength(Node* node);


    Node* root = new Node(nullptr, -1, new int(-1));
    Node* lastCreatedInternalNode = nullptr;         

    string text; 

    Node* activeNode = nullptr;   // вершина для расширения из неё
    int activeEdge = -1;          // индекс символа, который задает движение из текущей ноды
    int activeLength = 0;         // на сколько символов ступаем в направлении activeEdge
    int remainingSuffixCount = 0; // сколько суффиксов осталось создать
    int leafEnd = -1;             // глобальная переменная, определяет содержимое листьев: её инкремент как бы приписывает новый символ к ним
};