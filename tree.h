#pragma once

#include <iostream>
#include <exception>
#include "generate.h"
#include "ArraySequence.h"
#include "UI.h"


using namespace std;


template <class T>
class BST {
private:
    struct Node {
        T key;
        Node *right;
        Node *left;

        Node(const T &item = T()) : key(item), right(nullptr), left(nullptr) {}
    };

    Node *root;

    // Вспомогательная функция для рекурсивного вывода дерева
    void printBT(const string &prefix, const Node *node, bool isLeft) const {
        if (node != nullptr) {
            cout << prefix;
            cout << (isLeft ? "├──" : "└──");
            cout << node->key << endl;
            printBT(prefix + (isLeft ? "│   " : "    "), node->left, true);
            printBT(prefix + (isLeft ? "│   " : "    "), node->right, false);
        }
    }

public:

    BST() : root(nullptr) {}

    BST(int size, T (*generate)()) : BST() {
        for (int i = 0; i < size; i++) {
            add(generate());
        }
    }

    ~BST() {
        clear(root);
    }


    void clear(Node *node) {
        if (node == nullptr) {
            return;
        }
        
        if (node -> right == nullptr) {
            return;
        }
        
        if (node -> left == nullptr) {
            return;
        }
        
        clear(node -> left);
        clear(node -> right);
        delete node;
    }
    


    void add(const T &item) {
        Node *new_node = new Node(item);

        if (root == nullptr) {
            root = new_node;
            return;
        }

        Node *current = root;
        while (current->key != item) {
            if (current->key > item) {
                if (current->left == nullptr) {
                    current->left = new_node;
                    return;
                }

                current = current->left;

            } else if (current->key < item) {
                if (current->right == nullptr) {
                    current->right = new_node;
                    return;
                }

                current = current->right;

            }
        }

    }


    T find_min (Node* node) const {
        if (node == nullptr) {
            return 2000000000;
        }

        return std::min(find_min(node -> left), std::min(find_min(node -> right), node -> key));
    }

    T find_max(Node* node) const {
        if (node == nullptr) {
            return -2000000000;
        }

        return std::max(find_max(node -> left), std::max(find_max(node -> right), node -> key));
    }

    Node* get_root(const string& s) {

//        cout << "func called with " << s << endl;

        int n = s.length();

//        cout << "n: " << n << endl;

        if (n == 2 && s[0] == '(' && s[1] == ')') {
//            cout << "empty brackests" << endl;
            return nullptr;
        }

        if (s[0] != '(' || s[n - 1] != ')') {
//            cout << "no brackests in ends" << endl;
            throw invalid_argument("Некоректное дерево1.");
            return nullptr;
        }



        int l1 = -1, l2 = -1;
        int r1 = -1, r2 = -1;
        int number = 0;

        int start = 1;

        int n_brackets = 0;

        if (s[1] == '(') {
//            cout << "s[1] = (" << endl;
            for (int i = 1; i < n - 1; i++) {

                if (s[i] == '(')
                    n_brackets++;
                if (s[i] == ')')
                    n_brackets--;

                if (n_brackets == 0) {
                    l1 = 1;
                    l2 = i;
                    start = i + 1;
//                    cout << "first brackests: l1 = " << l1 << " l2 = " << l2 << endl;
                    break;
                }

            }

            if (n_brackets != 0) {
                throw invalid_argument("Некоректное дерево2.");
                return nullptr;
            }

        }
//        cout << "start: " << start << endl;

        if ('0' <= s[start] && s[start] <= '9') {

//            cout << "start -> digit" << endl;

            number = s[start] - '0';
            for (int i = start + 1; i < n - 1; i++) {
                if ('0' <= s[i] && s[i] <= '9') {
                    number = 10 * number + s[i] - '0';
                } else if (s[i] != '(') {
                    throw invalid_argument("Некоректное дерево3.");
                    return nullptr;
                } else {
                    r1 = i;
                    break;
                }
            }
//            cout << "number " << number << " r1: " << r1 << endl;

            if (r1 != -1) {
//                cout << "r1 != -1" << endl;
                n_brackets = 0;
                for (int i = r1; i < n - 1; i++) {

                    if (s[i] == '(')
                        n_brackets++;
                    if (s[i] == ')')
                        n_brackets--;
//                    cout << "! r1: " << r1 << " i: " << i << endl;

                    if (n_brackets == 0) {
                        if (i == n - 2) {
                            r2 = i;
                        } else {
                            throw invalid_argument("Некоректное дерево4.");
                            return nullptr;
                        }
                    }

                }

            }

//            cout << "l1: " << l1 << " l2: " << l2 << " r1: " << r1 << " r2: " << r2 << endl;

            Node* node = new Node(number);

            if (l1 != -1)
                node->left = get_root(s.substr(l1, l2 - l1 + 1));

            if (r1 != -1)
                node->right = get_root(s.substr(r1, r2 - r1 + 1));

            return node;


        } else {
            throw invalid_argument("Некоректное дерево.5");
            return nullptr;
        }

    }

    void res(const string & str) {
        Node* node = get_root(str);
        if (is_sorted(node)) {
            clear(root);
            root = get_root(str);
        }

        else {
            throw invalid_argument("Неотсортированное дерево");
        }
    }

    void test_res_function(BST& bst) {
        vector<pair<string, bool>> tests = {
                {"()", true},
                {"((5))", false},
                {")5(", false},
                {"(5()10)", false},
                {"(5(7)(10))", false},
                {"((7)5(10))", false},
                {"(5(10))", true},
                {"((3)5(10))", true},
                {"((3(4))5)", true},
                {"((((1)3(4))5((6(7(8)))9))10((((11(12))13(14))15(17))20(22)))", true},
                {"(()5())", true},
                {"((5)10(((7)12(14))20(25)))", false}
        };

        for (const auto& test : tests) {
            string input = test.first;
            bool expected = test.second;
            cout << "Тест: " << input << " - ";
            try {
                bst.res(input);
                if (expected) {
                    cout << "Успешно (ожидалось: корректное)" << endl;
                } else {
                    cout << "Ошибка (ожидалось: некорректное, но было принято)" << endl;
                }
            } catch (const invalid_argument& e) {
                if (!expected) {
                    cout << "Успешно (ожидалось: некорректное)" << endl;
                } else {
                    cout << "Ошибка (ожидалось: корректное, но было отвергнуто): " << e.what() << endl;
                }
            } catch (const exception& e) {
                cout << "Неожиданная ошибка: " << e.what() << endl;
            }
        }
    }

    bool is_sorted(Node * node) const {
        if (node == nullptr) {
            return true;
        }
        bool result = true;

        if (node -> left != nullptr) {
            result = result && (node -> left -> key < node -> key);
            result = result && find_max(node -> left ) < node -> key;
        }

        if (node -> right != nullptr) {
            result = result && (node -> right -> key > node -> key);
            result = result && find_min(node -> right) > node -> key;

        }
        return result;

    }
    
    void remove(const T& item) {
        
        if (root == nullptr) {
            return;
        }
        
        root = del(root, item);
        
    }


    Node* del(Node* node, const T& item) {
        if (node == nullptr) {
            return node;
        }

        if (item < node->key) {
            node->left = del(node->left, item);
            
        } else if (item > node->key) {
            node->right = del(node->right, item);
            
        } else {
            // Узел не имеет потомков
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            
            // Узел имеет потомка (правого или левого)
            else if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
                
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            
            // Узел имеет двух потомков
            else {
                Node* minNode = min_node(node->right);
                node->key = minNode->key;
                node->right = del(node->right, minNode->key);
                return node;
            }
        }

        return node;
    }
    

    
    bool find_elem(const T &elem) {
        Node *current = root;

        while (current != nullptr) {
            if (current->key == elem) {
                return true;
            } else if (current->key > elem) {
                current = current-> left;
            } else {
                current = current->right;
            }
        }

        return false;
    }
    
    Node* find_node(const T  & elem) {
        
        Node *current = root;

        while (current != nullptr) {
            if (current->key == elem) {
                return current;
            } else if (current->key > elem) {
                current = current-> left;
            } else {
                current = current->right;
            }
        }

        return nullptr;
        
    }

    T max() const {
        Node* new_node = root;
        
        if (root == nullptr) {
            throw invalid_argument("");
        }
        
        if (new_node -> right == nullptr) {
            return new_node -> key;
        }
        
        else {
            while(new_node -> right != nullptr) {
                new_node = new_node -> right;
            }
        }
        return new_node -> key;
    }
    
    
    string fix_traversal() {
        return toString("ЛКП");
    }
    
    // Функция для получения отсортированного вектора ключей из дерева
    void inorderTraversal(Node* node, ArraySequence<T>& keys) {
        if (node != nullptr) {
            inorderTraversal(node->left, keys);
            keys.Append(node->key);
            inorderTraversal(node->right, keys);
        }
    }

    // Функция для построения сбалансированного дерева из отсортированного вектора
    Node* buildBalancedBST(ArraySequence<T>& keys, int start, int end) {
        if (start > end) {
            return nullptr;
        }

        int mid = (start + end) / 2;
        Node* node = new Node(keys.Get(mid));

        node->left = buildBalancedBST(keys, start, mid - 1);
        node->right = buildBalancedBST(keys, mid + 1, end);

        return node;
    }

    // Функция для балансировки дерева
    void balanceTree() {
        ArraySequence<T> keys;
        inorderTraversal(root, keys);
        root = buildBalancedBST(keys, 0, keys.GetLength() - 1);
    }
    
    
    
    string toString(string traversal, Node *leaf = nullptr, bool isFirst = true)
    {
        static string ans = "";
        
        if(isFirst){
            ans = "";
            leaf = root;
        }
        
        
        if (leaf)
        {
            if(traversal == "КЛП"){
                ans += to_string(leaf->key) + " ";
                toString(traversal, leaf->left, false);
                toString(traversal, leaf->right, false);
            }
            else if(traversal == "КПЛ"){
                ans += to_string(leaf->key) + " ";
                toString(traversal, leaf->right, false);
                toString(traversal, leaf->left, false);
            }
            else if(traversal == "ЛПК"){
                toString(traversal, leaf->left, false);
                toString(traversal, leaf->right, false);
                ans += to_string(leaf->key) + " ";
            }
            else if(traversal == "ЛКП"){
                toString(traversal, leaf->left, false);
                ans += to_string(leaf->key) + " ";
                toString(traversal, leaf->right, false);
            }
            else if(traversal == "ПЛК"){
                toString(traversal, leaf->right, false);
                toString(traversal, leaf->left, false);
                ans += to_string(leaf->key) + " ";
            }
            else if(traversal == "ПКЛ"){
                toString(traversal, leaf->right, false);
                ans += to_string(leaf->key) + " ";
                toString(traversal, leaf->left, false);
            }
        
        }
        return ans;
    }
    
    Node* max_node(Node* node) const {
        
        Node* current = node;
        
        if (current == nullptr) {
            throw invalid_argument("");
        }
        
        while (current && current->right != nullptr) {
            current = current->right;
        }
        
        
        return current;
    
    }

    T min() const {
        Node* new_node = root;
        
        if (root == nullptr) {
            throw invalid_argument("");
        }
        
        if (new_node -> left == nullptr) {
            return new_node -> key;;
        }
        else {
            while(new_node -> left != nullptr) {
                new_node = new_node -> left;
            }
        }
        return new_node -> key;
    }
    
    Node* min_node(Node* node) const {
        
        Node* current = node;
        
        if (current == nullptr) {
            throw invalid_argument("");
        }
        
        while (current && current->left != nullptr) {
            current = current->left;
        }
        
        
        return current;
    
    }


    // Оригинальная функция print для сравнения
    void print(Node *node, int indent) const {
        if (node == nullptr) {
            return;
        }

        cout << string(indent * 4, ' ') << node->key << endl;
        print(node->left, indent + 1);
        print(node->right, indent + 1);

    }

    void print_tree() const {

        print(root, 0);
    }

    // Функция для вывода дерева с использованием символов /
    void print_tree_branches() const {
        printBT("", root, false);
    }
    
    
    void map_recursive_int(Node* node, T (*func)(T)) {
        if (node != nullptr) {
            node->key = func(node->key);
            map_recursive_int(node->left, func);
            map_recursive_int(node->right, func);
        }
    }
    
    void map_recursive_string(Node* node, T (*func)(T)) {
        if (node != nullptr) {
            node->key = func(node->key);
            map_recursive_string(node->left, func);
            map_recursive_string(node->right, func);
        }
    }

    
    void map(T (*func)(T), int type) {
        
        if (type == 1){ // int
            map_recursive_int(root, func);
        }
        
        else { // string
            map_recursive_string(root, func);
        }
    }
    
    
    void where_recursive_int(Node* node, bool (*func)(T)) {
         if (node != nullptr) {
             where_recursive_int(node->left, func);
             where_recursive_int(node->right, func);

             if (func(node -> key)) {
                 node -> key += 1;
             }
         }

     }

     void where_recursive_string(Node* node, bool (*func)(T)) {
         if (node != nullptr) {
             where_recursive_string(node->left, func);
             where_recursive_string(node->right, func);

             if (func(node -> key)) {
                 node -> key += 'R';
             }
         }

     }
    
    void where(bool (*func)(T), int type) {
        
        
        if (type == 1) { // int
            where_recursive_int(root, func);
        }
        
        else if (type == 2){ // string
            where_recursive_string(root, func);
        }
    
    }
    
    
    T reduce_recursive(Node* node, T (*func)(T, T)) const {
        if (node == nullptr) {
            return T();
        }
        
        T leftRes = reduce_recursive(node->left, func);
        T rightRes = reduce_recursive(node->right, func);
        return func(func(node->key, leftRes), rightRes);
    
    }

    
    T reduce(T (*func)(T, T)) const {
        return reduce_recursive(root, func);
    }
    
    Node* subtree_recursive(Node* node) {
        Node* newRoot = nullptr;
        
        if (node != nullptr) {
            newRoot = new Node(node -> key);
            newRoot -> left = subtree_recursive(node -> left);
            newRoot -> right = subtree_recursive(node -> right);
        }
        else {
            return nullptr;
        }
        
        return newRoot;
        
    }
    
    void subtree(T data) {
        
        Node* result = subtree_recursive(find_node(data));
        printBT("", result, false);
        
    }
    

    bool is_subtree(BST subroot) const {
        return is_subtree_recursive(root, subroot.root);
    }

    bool is_subtree_recursive(Node* node, Node* subroot) const {
        if (subroot == nullptr) {
            return true;
        }
        if (node == nullptr) {
            return false;
        }
        if (node->key == subroot->key && isSameTree(node, subroot)) {
            return true;
        }
        return is_subtree_recursive(node->left, subroot) || is_subtree_recursive(node->right, subroot);
    }

    bool isSameTree(Node* node1, Node* node2) const {
        if (node1 == nullptr && node2 == nullptr) {
            return true;
        }
        if (node1 == nullptr || node2 == nullptr) {
            return false;
        }
        return node1->key == node2->key && isSameTree(node1->left, node2->left) && isSameTree(node1->right, node2->right);
    }
        
    
    void clear_UI() {
        clear(root);
        root = nullptr; // Устанавливаем корень в nullptr после очистки
    }
    
};
