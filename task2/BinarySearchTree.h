#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <vector>

template <typename Key, typename Value, typename Compare = std::less<Key>>
class BinarySearchTree {
public:
    BinarySearchTree() = default;
    explicit BinarySearchTree(Compare compare) : compare_(std::move(compare)) {}

    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;
    BinarySearchTree(BinarySearchTree&&) noexcept = default;
    BinarySearchTree& operator=(BinarySearchTree&&) noexcept = default;

    bool empty() const { return root_ == nullptr; }
    std::size_t size() const { return size_; }
    bool contains(const Key& key) const { return findNode(root_.get(), key) != nullptr; }

    Value* find(const Key& key) {
        Node* node = findNode(root_.get(), key);
        return node != nullptr ? &node->value : nullptr;
    }

    const Value* find(const Key& key) const {
        const Node* node = findNode(root_.get(), key);
        return node != nullptr ? &node->value : nullptr;
    }

    bool insert(Key key, Value value) {
        return insertRec(root_, std::move(key), std::move(value));
    }

    bool erase(const Key& key) {
        bool removed = false;
        root_ = eraseRec(std::move(root_), key, removed);
        if (removed) {
            --size_;
        }
        return removed;
    }

    std::vector<std::pair<Key, Value>> inorder() const {
        std::vector<std::pair<Key, Value>> result;
        walk(root_.get(), result);
        return result;
    }

private:
    struct Node {
        Key key;
        Value value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node(Key k, Value v) : key(std::move(k)), value(std::move(v)) {}
    };

    Compare compare_{};
    std::unique_ptr<Node> root_;
    std::size_t size_ = 0;

    Node* findNode(Node* node, const Key& key) const {
        while (node != nullptr) {
            if (compare_(key, node->key)) {
                node = node->left.get();
            } else if (compare_(node->key, key)) {
                node = node->right.get();
            } else {
                return node;
            }
        }
        return nullptr;
    }

    bool insertRec(std::unique_ptr<Node>& node, Key key, Value value) {
        if (node == nullptr) {
            node = std::make_unique<Node>(std::move(key), std::move(value));
            ++size_;
            return true;
        }
        if (compare_(key, node->key)) {
            return insertRec(node->left, std::move(key), std::move(value));
        }
        if (compare_(node->key, key)) {
            return insertRec(node->right, std::move(key), std::move(value));
        }
        node->value = std::move(value);
        return false;
    }

    std::unique_ptr<Node> eraseRec(std::unique_ptr<Node> node,
                                   const Key& key,
                                   bool& removed) {
        if (node == nullptr) {
            return nullptr;
        }
        if (compare_(key, node->key)) {
            node->left = eraseRec(std::move(node->left), key, removed);
            return node;
        }
        if (compare_(node->key, key)) {
            node->right = eraseRec(std::move(node->right), key, removed);
            return node;
        }

        removed = true;
        if (node->left == nullptr) {
            return std::move(node->right);
        }
        if (node->right == nullptr) {
            return std::move(node->left);
        }

        Node* successor = node->right.get();
        while (successor->left != nullptr) {
            successor = successor->left.get();
        }
        node->key = successor->key;
        node->value = successor->value;
        bool unused = false;
        node->right = eraseRec(std::move(node->right), node->key, unused);
        return node;
    }

    void walk(const Node* node, std::vector<std::pair<Key, Value>>& out) const {
        if (node == nullptr) {
            return;
        }
        walk(node->left.get(), out);
        out.emplace_back(node->key, node->value);
        walk(node->right.get(), out);
    }
};
