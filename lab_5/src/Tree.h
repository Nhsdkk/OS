//
// Created by nikit on 12/25/2024.
//

#ifndef OS_LAB_5_SRC_TREE_H_
#define OS_LAB_5_SRC_TREE_H_

#include "iostream"
#include "vector"

#include <memory>

namespace Tree {

template<class T>
class Node {
    T data;
    std::vector<std::shared_ptr<Node<T>>> children;


 public:
    explicit Node(const T& data) : data(data), children() {}
    Node(const T& data, const std::shared_ptr<Node<T>>& parent) : data(data), children() {}
    void attachNode(const T& nodeToAttachTo, const T& value){
        if (data != nodeToAttachTo) {
            for (std::shared_ptr<Node<T>> child: children){
                child->attachNode(nodeToAttachTo, value);
            }
            return;
        }
        children.push_back(std::make_shared<Node<T>>(value));
    }

    bool exists(const T& d){
        if (data == d){
            return true;
        }
        bool result = false;
        for (std::shared_ptr<Node<T>> child: children){
            result |= child->exists(d);
        }
        return result;
    }

    void detachNode(const T& d) {
        std::shared_ptr<Node<T>> value = nullptr;
        for (auto i = 0; i < children.size(); ++i){
            if (children[i]->getData() == d){
                value = children[i];
                break;
            }
        }

        if (value != nullptr) children.erase(std::remove(children.begin(), children.end(),value), children.end());
        for (std::shared_ptr<Node<T>> child: children){
            child->detachNode(d);
        }
    };

    T getData() { return data; }
    std::vector<std::shared_ptr<Node<T>>> getChildren() { return children; }
    ~Node() {
        std::cout << "Deleting node with value "  << data << std::endl;
    };
};

template<class T>
class Tree {
    std::shared_ptr<Node<T>> root;
 public:
    explicit Tree(const T& data): root(std::make_shared<Node<T>>(data)){}
    std::shared_ptr<Node<T>> getRoot() { return root; }
    void remove(const T& data){
        root->detachNode(data);
    }
    void attach(const T& nodeToAttachTo, const T& value){
        root->attachNode(nodeToAttachTo, value);
    }
    bool exists(const T& data){
        return root->exists(data);
    }
    ~Tree() {
        std::cout << "Deleting tree..." << std::endl;
    };
};

} // Tree

#endif //OS_LAB_5_SRC_TREE_H_
