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
    activeNode = root;                //в самом начале корень и будет местом расширения
    for (size_t i = 0; i < text.length(); i++)
        ExtendSuffixTree(i);
}

int SuffixTree::EdgeLength(Node* node)
{
    return *(node->end) - (node->start) + 1;
}

void SuffixTree::ExtendSuffixTree(int phase)
{
    // установить lastCreatedInternalNode = null перед началом каждой фазы
    lastCreatedInternalNode = nullptr;

    // глобальный end для листьев
    // инкремент leafEnd воплощает правило 1,  наращивание листьев
    leafEnd++;

    // сколько суффиксов осталось создать
    remainingSuffixCount++;

    while (remainingSuffixCount > 0)
    {
        // если activeLength равна 0, тогда ищем текущий символ из корня
        if (activeLength == 0)
            activeEdge = phase; // индекс текущего символа в тексте определяет дугу, по которой будем двигаться

        // ищем текущий символ в начале исходящих из activeNode дуг
        auto find = activeNode->children.find(text[activeEdge]);

        // не нашли
        if (find == activeNode->children.end())
        {
            // добавим новую листовую дугу, исходящую из activeNode, начинающуся текущим символом (под листовой дугой имеется ввиду ребро)
            activeNode->children.insert(make_pair(
                text[activeEdge],
                new Node(root, phase, &leafEnd, phase - remainingSuffixCount + 1)));
            // и коль скоро создали новую внутр. вершинку, установим на нее суфф-Ссылку последней созданной внутренней вершины (Правило 2)
            if (lastCreatedInternalNode != nullptr)
            {
                lastCreatedInternalNode->suffix_link = activeNode;
                lastCreatedInternalNode = nullptr;
            }
        }
        else
        {
            // а если-таки есть дуга из activeNode, начинающаяся текущим символом, пойдем по ней спускаться
            Node* next = find->second;
            int edge_length = EdgeLength(next); // нашли длину дуги(ребра)

            // спуск по дуге(-ам)
            if (activeLength >= edge_length)
            {
                activeEdge += edge_length;
                activeLength -= edge_length;
                activeNode = next;
                continue; // таким образом мы будем спускаться, покуда не станет activeLength < edge_length
            }

            // правило 3: если текущий символ есть на дуге,
            // т.е. суффикс уже есть в дереве, то просто увеличим activeLength
            // как бы "шагнем вперед" по дуге
            if (text[next->start + activeLength] == text[phase])
            {
                // если lastCreatedInternalNode != null
                // это означает, что 2-е правило было применено ранее (создание новой внутренней  вершины)
                // установим суффлинку в activeNode
                if (lastCreatedInternalNode != nullptr && activeNode != root)
                    lastCreatedInternalNode->suffix_link = activeNode;
                activeLength++;
                break; // выйдем из цикла while
            }

            // сюда попали, если текущего символа нет на дуге
            // создадим новую внутреннюю вершинку
            Node* split = new Node(root, next->start, new int(next->start + activeLength - 1));
            // подвесим к activeNode новую вершинку
            activeNode->children[text[activeEdge]] = split;
            // у "следующей" вершинки изменим, очевидно, начало, ибо мы ее "обрубили"
            next->start += activeLength;
            // подвесим новую листовую вершинку
            split->children.insert(make_pair(text[phase], new Node(root, phase, &leafEnd, phase - remainingSuffixCount + 1)));
            // подвесим отрубленную вершинку
            split->children.insert(make_pair(text[next->start], next));
            // и не забудем про установку ссылок при создании новой внутренней вершины
            if (lastCreatedInternalNode != nullptr)
                lastCreatedInternalNode->suffix_link = split;
            lastCreatedInternalNode = split;
        }

        remainingSuffixCount--;

        // если activeNode == root, тогда согласно правилу 2, мы декементируем activeLength и инкрементируем activeEdge
        // это, можно сказать, альтернатива суфф. ссылке в случае, когда activeNode == root
        if (activeNode == root && activeLength > 0)
        {
            activeLength--;
            activeEdge++;
        }
        else if (activeNode != root) // если же activeNode != root, то радостно скачем по имеющейся суффиксной ссылке
            activeNode = activeNode->suffix_link;
    }
}

// запушить в вектор листья (вернее, их индексы), которые находятся в поддереве с корнем node
void SuffixTree::CountIndex(Node* node, vector<int>& vec)
{
    if (!node) // конец рекурсии: пустое поддерево
        return;
    for (auto it : node->children) // рекурсивный вызов для всех поддеревьев
        CountIndex(it.second, vec);
    if (node->suffix_index != -1) // как дошли до листа, добавляем в вектор его индекс
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
            int ff = EdgeLength(nextV); // нашли длину дуги(ребра)
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
 
