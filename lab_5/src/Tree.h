//
// Created by Anton on 12/9/2024.
//

#ifndef OS_LAB_5_BSEARCHTREE_H_
#define OS_LAB_5_BSEARCHTREE_H_

#include "iostream"
#include "vector"

namespace Tree{
    template<class T>
    class Node {
        T data;
        std::vector<std::shared_ptr<Node<T>>> children;
        std::shared_ptr<Node<T>> parent;

        public:
            explicit Node(const T& data) : data(data), children(), parent(nullptr) {}
            Node(const T& data, const std::shared_ptr<Node<T>>& parent) : data(data), children(), parent(parent) {}
            void attachNode(const T& value){
                children.push_back(std::make_shared<Node<T>>(value, this));
            }

            void detachNode(const T& d) {
                std::shared_ptr<Node<T>> value = nullptr;
                for (auto i = 0; i < children.size(); ++i){
                    if (children[i]->getData() == d){
                        value = children[i];
                        break;
                    }
                }

                if (value == nullptr) throw std::invalid_argument("Can't find child");
                children.erase(std::remove(children.begin(), children.end(),value), children.end());
            };

            T getData() { return data; }
            std::vector<std::shared_ptr<Node<T>>> getChildren() { return children; }
            ~Node() {
                std::cout << "Deleting node with value "  << data.getId() << std::endl;
            };
    };

    template<class T>
    class Tree {
        std::shared_ptr<Node<T>> root;
        public:
            explicit Tree(const T& data): root(std::make_shared<Node<T>>(data)){}
            std::shared_ptr<Node<T>> getRoot() { return root; }
            ~Tree() {
                std::cout << "Deleting tree..." << std::endl;
            };
    };
}

#endif //OS_LAB_5_BSEARCHTREE_H_
