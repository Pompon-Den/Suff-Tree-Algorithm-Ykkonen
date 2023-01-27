#include "SuffixTree.h"

SuffixTree::SuffixTree(string& str) : text(str)
{
        BuildSuffixTree();
}

void SuffixTree::BuildSuffixTree()
{
    activeNode = root;
    for (size_t i = 0; i <= text.length(); i++) {
        ExtendSuffixTree(i);
    }
}

int SuffixTree::EdgeLength(Node* node)
{
    return *(node->end) - (node->start) + 1;
}

void SuffixTree::ExtendSuffixTree(int phase)
{
    // установить lastCreatedInternalNode = null перед началом каждой фазы
    lastCreatedInternalNode = nullptr;
    leafEnd++;
    remainingSuffixCount++;

    while (remainingSuffixCount > 0)
    {
        if (activeLength == 0)//тогда ищем текущий символ из корня
            activeEdge = phase; // индекс текущего символа в тексте определяет дугу, по которой будем двигаться

        auto find = activeNode->children.find(text[activeEdge]);
        if (find == activeNode->children.end())
        {
                activeNode->children.insert(make_pair(           // добавим новую листовую дугу, исходящую из activeNode, начинающуся текущим символом
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
            Node* next = find->second;      //если есть дуга из activeNode, начнем по ней спускаться
            int edge_length = EdgeLength(next);

            if (activeLength >= edge_length)
            {
                activeEdge += edge_length;
                activeLength -= edge_length;
                activeNode = next;
                continue;
            }

            // если текущий символ есть на дуге,
            // т.е. суффикс уже есть в дереве, то просто увеличим activeLength
            if (text[next->start + activeLength] == text[phase])
            {
                if (lastCreatedInternalNode != nullptr && activeNode != root)    // если lastCreatedInternalNode != null
                    lastCreatedInternalNode->suffix_link = activeNode;           // это означает, что ранее было создание новой  вершины
                     activeLength++;                                             // установим суффлинку в activeNode
                break; 
            }

            //если текущего символа нет на дуге создадим новую внутреннюю вершинку
            Node* split = new Node(root, next->start, new int(next->start + activeLength - 1));
            activeNode->children[text[activeEdge]] = split;            // подвесим к activeNode новую вершинку
            next->start += activeLength;                               // у "следующей" вершинs изменим начало
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

// запушить в вектор листья  которые находятся в поддереве с корнем node
void SuffixTree::GetSuffixIndexs(Node* node, vector<int>& vec)
{
    if (!node)
        return;
    for (auto it : node->children) 
        GetSuffixIndexs(it.second, vec);
    if (node->suffix_index != -1) 
        vec.push_back(node->suffix_index);
}

void SuffixTree::PrintSuffixs() {
     vector <int> VecSuffTree;
     GetSuffixIndexs(root, VecSuffTree);
    for (int i = 0; i < VecSuffTree.size(); i++) {
        cout << "\"";
        for (int j = 0; j < VecSuffTree[i]; j++) {
            cout << text[j];
        }
        cout << "\"" << endl;
    }
}

bool SuffixTree::Find(string Pstr) {
    if (Pstr.empty()) {
        return true;
    } 
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
        //если нашли - спускаемся       
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