#include "BinarySearchTree.h"
#include "DocumentIndex.h"
#include "SetAlgorithms.h"

#include "Test.h"

#include <iostream>
#include <string>
#include <vector>

int main() {
    std::cout << "Задание 2. Индекс документов — Усманов Максим, ИДБ-25-01, вариант 1\n\n";

    BinarySearchTree<int, std::string> tree;
    check(tree.insert(5, "five"), "insert 5");
    check(tree.insert(3, "three"), "insert 3");
    check(tree.insert(7, "seven"), "insert 7");
    check(!tree.insert(5, "FIVE") && tree.size() == 3,
          "повторный ключ не увеличивает size");
    check(tree.find(5) != nullptr && *tree.find(5) == "FIVE", "значение обновляется");

    const auto order = tree.inorder();
    check(order.size() == 3 && order[0].first == 3 && order[2].first == 7,
          "inorder по возрастанию");
    check(tree.erase(5) && !tree.contains(5), "удаление узла");

    check(intersectSorted(std::vector<int>{1, 2, 3, 5}, std::vector<int>{2, 4, 5}) ==
              std::vector<int>({2, 5}),
          "пересечение с общими элементами");
    check(intersectSorted(std::vector<int>{1, 2}, std::vector<int>{3, 4}).empty(),
          "пустое пересечение");

    DocumentIndex index;
    check(index.add({"doc1", "Object Oriented Programming and programming"}),
          "add doc1");
    check(index.add({"doc2", "Programming languages and algorithms"}), "add doc2");
    check(index.add({"doc3", "Binary search tree and algorithms"}), "add doc3");
    check(!index.add({"doc1", "dup"}), "дубликат id отклонён");

    const auto programming = index.searchWord("programming");
    check(programming.size() == 2 && programming[0] == "doc1" &&
              programming[1] == "doc2",
          "поиск слова без дублей id");
    check(index.searchWord("quantum").empty(), "отсутствующее слово");

    const auto both = index.searchAll({"programming", "algorithms"});
    check(both.size() == 1 && both[0] == "doc2", "AND-поиск");
    check(index.searchAll({"programming", "quantum"}).empty(),
          "AND без результата");

    std::cout << "\nИндекс (симметричный обход):\n";
    for (const auto& entry : index.indexedWords()) {
        std::cout << "  " << entry.first << " -> ";
        for (std::size_t i = 0; i < entry.second.size(); ++i) {
            std::cout << entry.second[i]
                      << (i + 1 < entry.second.size() ? ", " : "");
        }
        std::cout << '\n';
    }

    check(index.remove("doc2"), "удаление doc2");
    check(index.searchWord("programming").size() == 1,
          "после удаления programming только в doc1");

    bool hasLanguages = false;
    for (const auto& entry : index.indexedWords()) {
        if (entry.first == "languages") {
            hasLanguages = true;
        }
    }
    check(!hasLanguages, "слово удаляется из BST, если документов не осталось");

    printSummary("task2");
    return g_failed == 0 ? 0 : 1;
}
