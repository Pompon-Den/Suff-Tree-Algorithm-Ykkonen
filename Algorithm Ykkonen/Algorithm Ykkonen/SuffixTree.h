#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

// ���������� ������, ���������� �������� ����������� ���������� true suffix tree
// �.�. ������� ������� �� ����� �������� ��������� ������-���� ������� ��������
#define TERMINATION_SYMBOL '$'

using namespace std;

class SuffixTree;

class Node
{
public:
    friend SuffixTree;

    // ����������� ���� ��� �������: ������������ ��� �������� ����� ���������� ������
    Node(Node* link, int start, int* end) : Node(link, start, end, -1)
    {
    }

    // ����������� � ��������: ������������ �� ��� �������� �������
    // ��� ��� �� �������� ����� ���������� ���������� �� ������
    Node(Node* link, int start, int* end, int ind) : suffix_link(link),
        start(start),
        end(end),
        suffix_index(ind)
    {
    }

private:
    map<char, Node*> children; // "������" �����
    Node* suffix_link;          // ���������� ������
    int start;                  // ����� ������� ������� ������� � �������� ������
    int* end;                   // ����� ���������� ������� ������� � �������� ������ 
    int suffix_index;           // ����� ��������
};

class SuffixTree
{
public:
    SuffixTree(string& text);             // ����������� �� ������
    void BuildSuffixTree();               // ���� ���������� ������
    ~SuffixTree()                         // ����������
    {
        DeleteSuffixTree(root);
    }
    void CountIndex(Node* node, vector<int>& vec); // ���� �������� ������� � ������, ������������ � ��������� node
    Node* root = new Node(nullptr, -1, new int(-1)); // ������
    void SearhP(string podstroka, string s); //����� ��������� � ������
private:
    void ExtendSuffixTree(int pos);   // ���������� ������
    void DeleteSuffixTree(Node* node) // �������� ������
    {
        for (auto it : node->children)
            DeleteSuffixTree(it.second);
        if (node->suffix_index == -1)
            delete node->end;
        delete node;
    }
   
    int EdgeLength(Node* node);                    // ������� ����� ��������� ������������� �� ����
    Node* lastCreatedInternalNode = nullptr;         // ��������� ��������� ��������� �������

    string text; // �����, �� �������� �������� ���������� ������

    Node* activeNode = nullptr;   // ��, ������ �������� ���������� �� ��������� ����
    int activeEdge = -1;          // ������ �������, ������� ������ �������� �� ������� ����
    int activeLength = 0;         // �� ������� �������� ������� � ����������� activeEdge
    int remainingSuffixCount = 0; // ������� ��������� �������� �������. �� ����, ������� ��������� �� ������� ���� �� �� �������.
    int leafEnd = -1;             // ���������� ����������, ���������� ���������� �������: � ��������� ��� �� ����������� ����� ������ � ���
};