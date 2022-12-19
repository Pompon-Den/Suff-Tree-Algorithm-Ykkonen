#include "SuffixTree.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

SuffixTree::SuffixTree(string& str) : text(str)
{
    text += TERMINATION_SYMBOL;
    BuildSuffixTree();
}

void SuffixTree::BuildSuffixTree()
{
    activeNode = root;                //� ����� ������ ������ � ����� ������ ����������
    for (size_t i = 0; i < text.length(); i++)
        ExtendSuffixTree(i);
}

int SuffixTree::EdgeLength(Node* node)
{
    return *(node->end) - (node->start) + 1;
}

void SuffixTree::ExtendSuffixTree(int phase)
{
    // ���������� lastCreatedInternalNode = null ����� ������� ������ ����
    lastCreatedInternalNode = nullptr;

    // ���������� end ��� �������
    // ��������� leafEnd ��������� ������� 1,  ����������� �������
    leafEnd++;

    // ������� ��������� �������� �������
    remainingSuffixCount++;

    while (remainingSuffixCount > 0)
    {
        // ���� activeLength ����� 0, ����� ���� ������� ������ �� �����
        if (activeLength == 0)
            activeEdge = phase; // ������ �������� ������� � ������ ���������� ����, �� ������� ����� ���������

        // ���� ������� ������ � ������ ��������� �� activeNode ���
        auto find = activeNode->children.find(text[activeEdge]);

        // �� �����
        if (find == activeNode->children.end())
        {
            // ������� ����� �������� ����, ��������� �� activeNode, ����������� ������� �������� (��� �������� ����� ������� ����� �����)
            activeNode->children.insert(make_pair(
                text[activeEdge],
                new Node(root, phase, &leafEnd, phase - remainingSuffixCount + 1)));
            // � ���� ����� ������� ����� �����. ��������, ��������� �� ��� ����-������ ��������� ��������� ���������� ������� (������� 2)
            if (lastCreatedInternalNode != nullptr)
            {
                lastCreatedInternalNode->suffix_link = activeNode;
                lastCreatedInternalNode = nullptr;
            }
        }
        else
        {
            // � ����-���� ���� ���� �� activeNode, ������������ ������� ��������, ������ �� ��� ����������
            Node* next = find->second;
            int edge_length = EdgeLength(next); // ����� ����� ����(�����)

            // ����� �� ����(-��)
            if (activeLength >= edge_length)
            {
                activeEdge += edge_length;
                activeLength -= edge_length;
                activeNode = next;
                continue; // ����� ������� �� ����� ����������, ������ �� ������ activeLength < edge_length
            }

            // ������� 3: ���� ������� ������ ���� �� ����,
            // �.�. ������� ��� ���� � ������, �� ������ �������� activeLength
            // ��� �� "������ ������" �� ����
            if (text[next->start + activeLength] == text[phase])
            {
                // ���� lastCreatedInternalNode != null
                // ��� ��������, ��� 2-� ������� ���� ��������� ����� (�������� ����� ����������  �������)
                // ��������� ��������� � activeNode
                if (lastCreatedInternalNode != nullptr && activeNode != root)
                    lastCreatedInternalNode->suffix_link = activeNode;
                activeLength++;
                break; // ������ �� ����� while
            }

            // ���� ������, ���� �������� ������� ��� �� ����
            // �������� ����� ���������� ��������
            Node* split = new Node(root, next->start, new int(next->start + activeLength - 1));
            // �������� � activeNode ����� ��������
            activeNode->children[text[activeEdge]] = split;
            // � "���������" �������� �������, ��������, ������, ��� �� �� "��������"
            next->start += activeLength;
            // �������� ����� �������� ��������
            split->children.insert(make_pair(text[phase], new Node(root, phase, &leafEnd, phase - remainingSuffixCount + 1)));
            // �������� ����������� ��������
            split->children.insert(make_pair(text[next->start], next));
            // � �� ������� ��� ��������� ������ ��� �������� ����� ���������� �������
            if (lastCreatedInternalNode != nullptr)
                lastCreatedInternalNode->suffix_link = split;
            lastCreatedInternalNode = split;
        }

        remainingSuffixCount--;

        // ���� activeNode == root, ����� �������� ������� 2, �� ������������� activeLength � �������������� activeEdge
        // ���, ����� �������, ������������ ����. ������ � ������, ����� activeNode == root
        if (activeNode == root && activeLength > 0)
        {
            activeLength--;
            activeEdge++;
        }
        else if (activeNode != root) // ���� �� activeNode != root, �� �������� ������ �� ��������� ���������� ������
            activeNode = activeNode->suffix_link;
    }
}

// �������� � ������ ������ (������, �� �������), ������� ��������� � ��������� � ������ node
void SuffixTree::CountIndex(Node* node, vector<int>& vec)
{
    if (!node) // ����� ��������: ������ ���������
        return;
    for (auto it : node->children) // ����������� ����� ��� ���� �����������
        CountIndex(it.second, vec);
    if (node->suffix_index != -1) // ��� ����� �� �����, ��������� � ������ ��� ������
        vec.push_back(node->suffix_index);

}

void  SuffixTree::SearhP(string podstroka, string s) {
    int indexP = 0;
    int podsroka_length = podstroka.length();
    Node* activeV = root;
    while (podsroka_length > 0) {
        auto f = activeV->children.find(s[indexP]);
        if (f == activeV->children.end()) {
            cout << "NO";
            return;
        }
        else {
            Node* nextV = f->second;
            int ff = EdgeLength(nextV); // ����� ����� ����(�����)
            if (ff >= podsroka_length) {
                cout << "YES";
                return;
            }
            else {
                podsroka_length -= ff;
                activeV = nextV;
                indexP += ff;
                continue;
            }

        }
    }
} 
 
