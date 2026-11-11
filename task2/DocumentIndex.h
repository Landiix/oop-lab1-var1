#pragma once

#include "BinarySearchTree.h"
#include "SetAlgorithms.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <utility>
#include <vector>

struct Document {
    std::string id;
    std::string text;
};

class DocumentIndex {
public:
    bool add(Document document) {
        if (findById(document.id) != docs_.end()) {
            return false;
        }

        const std::vector<std::string> words = uniqueWords(document.text);
        docs_.push_back(std::move(document));
        const std::string& id = docs_.back().id;

        for (const std::string& word : words) {
            if (std::vector<std::string>* ids = index_.find(word)) {
                insertUniqueSorted(*ids, id);
            } else {
                index_.insert(word, std::vector<std::string>{id});
            }
        }
        return true;
    }

    bool remove(const std::string& id) {
        auto it = findById(id);
        if (it == docs_.end()) {
            return false;
        }

        const std::vector<std::string> words = uniqueWords(it->text);
        docs_.erase(it);

        for (const std::string& word : words) {
            std::vector<std::string>* ids = index_.find(word);
            if (ids == nullptr) {
                continue;
            }
            eraseSorted(*ids, id);
            if (ids->empty()) {
                index_.erase(word);
            }
        }
        return true;
    }

    std::vector<std::string> searchWord(std::string word) const {
        toLowerInPlace(word);
        const std::vector<std::string>* ids = index_.find(word);
        if (ids == nullptr) {
            return {};
        }
        return *ids;
    }

    std::vector<std::string> searchAll(std::vector<std::string> query) const {
        if (query.empty()) {
            return {};
        }

        std::vector<std::string> result;
        bool first = true;
        for (std::string& word : query) {
            toLowerInPlace(word);
            const std::vector<std::string>* ids = index_.find(word);
            if (ids == nullptr) {
                return {};
            }
            if (first) {
                result = *ids;
                first = false;
            } else {
                result = intersectSorted(result, *ids);
            }
            if (result.empty()) {
                return {};
            }
        }
        return result;
    }

    std::vector<std::pair<std::string, std::vector<std::string>>> indexedWords() const {
        return index_.inorder();
    }

private:
    BinarySearchTree<std::string, std::vector<std::string>> index_;
    std::vector<Document> docs_;

    std::vector<Document>::iterator findById(const std::string& id) {
        return std::find_if(docs_.begin(), docs_.end(),
                            [&](const Document& doc) { return doc.id == id; });
    }

    static void toLowerInPlace(std::string& text) {
        for (char& ch : text) {
            ch = static_cast<char>(
                std::tolower(static_cast<unsigned char>(ch)));
        }
    }

    static std::vector<std::string> uniqueWords(const std::string& text) {
        std::vector<std::string> words;
        std::string current;
        for (unsigned char ch : text) {
            if (std::isalnum(ch) != 0) {
                current.push_back(static_cast<char>(std::tolower(ch)));
            } else if (!current.empty()) {
                insertUniqueSorted(words, current);
                current.clear();
            }
        }
        if (!current.empty()) {
            insertUniqueSorted(words, current);
        }
        return words;
    }
};
