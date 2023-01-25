#include "SuffixTree.h"

SuffixTree::SuffixTree(string& str) : text(str)
{
    text += TERMINATION_SYMBOL;
    BuildSuffixTree();
}

void SuffixTree::BuildSuffixTree()
{
    activeNode = root;
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

    // глобальный end дл€ листьев
    leafEnd++;

    // сколько суффиксов осталось создать
    remainingSuffixCount++;

    while (remainingSuffixCount > 0)
    {
        //тогда ищем текущий символ из корн€
        if (activeLength == 0)
            activeEdge = phase; // индекс текущего символа в тексте определ€ет дугу, по которой будем двигатьс€

        // ищем текущий символ в начале исход€щих из activeNode дуг
        auto find = activeNode->children.find(text[activeEdge]);

        // не нашли
        if (find == activeNode->children.end())
        {
            // добавим новую листовую дугу, исход€щую из activeNode, начинающус€ текущим символом
            activeNode->children.insert(make_pair(
                text[activeEdge],
                new Node(root, phase, &leafEnd, phase - remainingSuffixCount + 1)));
            //создали новую внутр. вершину, установим на нее суффлинку последней созданной вершины
            if (lastCreatedInternalNode != nullptr)
            {
                lastCreatedInternalNode->suffix_link = activeNode;
                lastCreatedInternalNode = nullptr;
            }
        }
        else
        {
            //если есть дуга из activeNode, начнем по ней спускатьс€
            Node* next = find->second;
            int edge_length = EdgeLength(next);

            if (activeLength >= edge_length)
            {
                activeEdge += edge_length;
                activeLength -= edge_length;
                activeNode = next;
                continue;
            }

            // правило 3: если текущий символ есть на дуге,
            // т.е. суффикс уже есть в дереве, то просто увеличим activeLength
            if (text[next->start + activeLength] == text[phase])
            {
                // если lastCreatedInternalNode != null
                // это означает, что 2-е правило было применено ранее (создание новой вн. вершины)
                // установим суффлинку в activeNode
                if (lastCreatedInternalNode != nullptr && activeNode != root)
                    lastCreatedInternalNode->suffix_link = activeNode;
                activeLength++;
                break; // выйдем из цикла while
            }

            //если текущего символа нет на дуге
            // создадим новую внутреннюю вершинку
            Node* split = new Node(root, next->start, new int(next->start + activeLength - 1));
            activeNode->children[text[activeEdge]] = split;            // подвесим к activeNode новую вершинку
            next->start += activeLength;            // у "следующей" вершинки изменим начало
            split->children.insert(make_pair(text[phase], new Node(root, phase, &leafEnd, phase - remainingSuffixCount + 1))); // подвесим новую листовую вершинку
            split->children.insert(make_pair(text[next->start], next));
            if (lastCreatedInternalNode != nullptr)
                lastCreatedInternalNode->suffix_link = split;
            lastCreatedInternalNode = split;
        }

        remainingSuffixCount--;

        if (activeNode == root && activeLength > 0)
        {
            activeLength--;
            activeEdge++;
        }
        else if (activeNode != root) // если же activeNode != root, то переход по суффиксной ссылке
            activeNode = activeNode->suffix_link;
    }
}

// запушить в вектор листь€  которые наход€тс€ в поддереве с корнем node
void SuffixTree::CountIndex(Node* node, vector<int>& vec)
{
    if (!node)
        return;
    for (auto it : node->children) 
        CountIndex(it.second, vec);
    if (node->suffix_index != -1) 
        vec.push_back(node->suffix_index);
}

void SuffixTree::SuffixOutput() {
     vector <int> VecSuffTree;
     CountIndex(root, VecSuffTree);
    for (int i = 0; i < VecSuffTree.size(); i++) {
        cout << "\"";
        for (int j = 0; j < VecSuffTree[i]; j++) {
            cout << text[j];
        }
        cout << "\"" << endl;
    }
}

bool SuffixTree::FindSub(string Pstr) {
    Node* current_node = root;
    string finded_str = "";
    int depth_edge = 0;
    int total_depth = 0;

    // ищем ребро
    auto finded_edge = current_node->children.find(Pstr[0]);
    if (finded_edge == current_node->children.end()) {
        return false;
    }
    Node* finded_node = finded_edge->second;

    while (Pstr != finded_str) {
        //если не находим
        if (finded_edge == current_node->children.end()) {
            return false;
        }
        //если нашли - спускаемс€       
        while (depth_edge <= EdgeLength(finded_node)) {
            if (finded_str == Pstr) {
                return true;
            }
            if (depth_edge != EdgeLength(finded_node)) {    
                finded_str += text[finded_node->start + depth_edge];
                if (finded_str[depth_edge] != Pstr[depth_edge]) {
                    return false;
                }
                depth_edge++;
                continue;
            }
            break;
        }
        // спустились к концу ребра, смена вершины
        current_node = finded_node;
        total_depth += depth_edge;
        depth_edge = 0;
        finded_edge = current_node->children.find(Pstr[total_depth]);
        if (finded_edge == current_node->children.end()) {
            return false;
        }
        finded_node = finded_edge->second;
    }
}