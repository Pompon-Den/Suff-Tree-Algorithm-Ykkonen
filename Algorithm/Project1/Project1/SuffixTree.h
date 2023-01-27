/*
Этот заголовочный файл содержит библитотеку С++ с реализацией алгоритма Укконена

    Подключение библиотеки:
        #include "SuffixTree.h"

    Построение суффиксного дерево по строке:
        SuffixTree* название = new SuffixTree(название_строки);

    Деструктор - освобождает память
        delete название ;

    Поиск подстроки в суффиксном дереве, возвращает булевое значение:
        название->FindSub(название_строки);

    Вывод всех суффиксов строки в дереве в консоль:
        название->SuffixOutput();

*/

#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#define TERMINATION_SYMBOL '$' // ЧТО ТО С НИМ СДЕЛАТЬ НУЖНО  НЕ ДОЛЛАР

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
    SuffixTree(string& text);             // Конструктор по строке
    void BuildSuffixTree();               
    ~SuffixTree()                        
    {
        DeleteSuffixTree(root);         // Деструктор
    }
 
    bool Find(string Pstr);
    void PrintSuffixs();

private:
    void DeleteSuffixTree(Node* node) 
    {
        for (auto it : node->children)
            DeleteSuffixTree(it.second);
        if (node->suffix_index == -1) 
            delete node->end;
        delete node;
    }              

    void ExtendSuffixTree(int pos);                     // Вставка символа
    void GetSuffixIndexs(Node* node, vector<int>& vec); // Сбор индексов суффиксов в  вектор
    int EdgeLength(Node* node);                         // Подсчет длинны ребра


    Node* root = new Node(nullptr, -1, new int(-1));    // Корень
    Node* lastCreatedInternalNode = nullptr;            // Последняя созданная внутренняя вершина

    string text;                                        // Строка по которой строиться дерево

    Node* activeNode = nullptr;                         // Вершина для расширения из неё
    int activeEdge = -1;                                // Индекс символа, который задает движение из текущей ноды
    int activeLength = 0;                               // На сколько символов ступаем в направлении activeEdge
    int remainingSuffixCount = 0;                       // сколько суффиксов осталось создать. по сути, сколько суффиксов на прошлом шаге мы не создали
    int leafEnd = -1;                                   // глобальная переменная, определяет содержимое листьев: её инкремент как бы приписывает новый символ к ним
};
#endif