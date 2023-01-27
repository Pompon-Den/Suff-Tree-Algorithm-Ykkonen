/*
���� ������������ ���� �������� ����������� �++ � ����������� ��������� ��������

    ����������� ����������:
        #include "SuffixTree.h"

    ���������� ����������� ������ �� ������:
        SuffixTree* �������� = new SuffixTree(��������_������);

    ���������� - ����������� ������
        delete �������� ;

    ����� ��������� � ���������� ������, ���������� ������� ��������:
        ��������->FindSub(��������_������);

    ����� ���� ��������� ������ � ������ � �������:
        ��������->SuffixOutput();

*/

#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#define TERMINATION_SYMBOL '$' // ��� �� � ��� ������� �����  �� ������

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
    SuffixTree(string& text);             // ����������� �� ������
    void BuildSuffixTree();               
    ~SuffixTree()                        
    {
        DeleteSuffixTree(root);         // ����������
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

    void ExtendSuffixTree(int pos);                     // ������� �������
    void GetSuffixIndexs(Node* node, vector<int>& vec); // ���� �������� ��������� �  ������
    int EdgeLength(Node* node);                         // ������� ������ �����


    Node* root = new Node(nullptr, -1, new int(-1));    // ������
    Node* lastCreatedInternalNode = nullptr;            // ��������� ��������� ���������� �������

    string text;                                        // ������ �� ������� ��������� ������

    Node* activeNode = nullptr;                         // ������� ��� ���������� �� ��
    int activeEdge = -1;                                // ������ �������, ������� ������ �������� �� ������� ����
    int activeLength = 0;                               // �� ������� �������� ������� � ����������� activeEdge
    int remainingSuffixCount = 0;                       // ������� ��������� �������� �������. �� ����, ������� ��������� �� ������� ���� �� �� �������
    int leafEnd = -1;                                   // ���������� ����������, ���������� ���������� �������: � ��������� ��� �� ����������� ����� ������ � ���
};
#endif