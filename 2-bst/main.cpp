#include <iostream>
#include <memory>

using namespace std;

/*
 * In this question, you will implement a generic binary search tree data
 * structure. A binary search tree contains nodes that store four items:
 * a pointer to a generic data value, a pointer to the left child node,
 * a pointer to the right child node, and a pointer to the parent node.
 * Binary search trees are designed such all values in the left subtree of
 * a node must be smaller than the value in the node, and all values in the
 * right subtree of a node must be larger than the value in the node. For
 * this question, we will design a tree that does not allow duplicates.
 * A subtree of a node is recursively defined as the tree consisting of the
 * node and its subtrees, rooted at the associated child of that node. That
 * is, all nodes that can be reached by following the pointer to the left
 * child of node A are in node A's subtree. For this question, we will
 * create a generic binary search tree template that can store any type
 * that implements the appropriate operations.
 */

/*
 * Design a class to store a node in the binary search tree. All of the
 * pointers in a node should be implemented using smart pointers. The
 * pointer to the data should be a unique pointer. The pointers to other
 * nodes should be designed in a manner that allows proper memory
 * management using the concepts we discussed in class.
 */

template<typename T> class Node {
public:
    shared_ptr<Node> left;
    shared_ptr<Node> right;
    shared_ptr<Node> parent;
    unique_ptr<T> data;

    explicit Node(T* data): data(unique_ptr<T>(data)), left(nullptr), right(nullptr), parent(nullptr) {}
    Node(T* data, Node* parent): data(unique_ptr<T>(data)), left(nullptr), right(nullptr), parent(shared_ptr<T>(parent)) {}
};

/*
 * Design a class to represent a binary search tree. The class should
 * contain a pointer to the root node in the tree and the number of
 * elements in the tree. Remember to update the count in other functions as
 * appropriate. Your class should have all of the standard functionality
 * for classes.
 */

template<typename T> class BST {
public:
    shared_ptr<Node<T>> root;
    size_t count;
    BST() : root(nullptr), count(0) {}
    BST(Node<T>* root, size_t count): root(root), count(count) {}
    // TODO: copy constructor

    /*
     * Write a constructor that takes as input an array of values and
     * creates a binary search tree containing values. The method should
     * return a pointer to the created binary search tree. You should
     * insert elements into the tree in the order they appear in the array.
     * You are allowed to add additional arguments to your function as
     * necessary.
     */

    BST(T* arr, size_t len) {
        count = len;
        for(size_t i = 0; i < len; i++) {
            insert(arr[i]);
        }
    }

    void insert_node(Node<T>* node) {
        count++;
        if(root == nullptr) {
            node->parent = nullptr;
            root = shared_ptr<Node<T>>(node);
            return;
        }

        auto cur = root;
        while(true) {
            if(node->data < cur->data) {
                if(cur->left == nullptr) {
                    node->parent = cur;
                    cur->left = shared_ptr<Node<T>>(node);
                    return;
                }
                cur = cur->left;
            } else {
                if(cur->right == nullptr) {
                    node->parent = cur;
                    cur->right = shared_ptr<Node<T>>(node);
                    return;
                }
                cur = cur->right;
            }
        }

    }

    void insert(T item) {
        insert_node(new Node<T>(&item));
    }

    /*
     * Write a destructor for your binary search tree.
     */

    ~BST() = default;

    /*
     * Write a method that takes as input a value of the appropriate type
     * and adds the value to the binary search tree. You should follow the
     * BST property to ensure that you add the new node to the right place
     * in the tree. This should not change any other part of the tree.
     */

    // see above for insert method

    /*
     * Write a method that takes as input a value of the appropriate type
     * and removes the value from the tree. The method should modify the
     * original tree, not create a copy. The tree must be updated to
     * maintain its structure. If possible the largest element in the
     * subtree rooted at the left child should be moved to the position
     * occupied by the removed value. This may cause additional issues
     * that need to be fixed. The function should return number of values
     * removed (0 or 1). Beware of memory leaks.
     */
    uint8_t remove(T val) {
        // first, find the node
        auto node = root;
        while(*node->data.get() != val) {
            node = val < *node->data.get() ? node->left : node->right;
            if(node == nullptr) return 0;
        }

        // now, remove the node
        count--;

        // if the node has no children, just remove it
        if(node->left == nullptr && node->right == nullptr) {
            if(node->parent == nullptr) {
                root = nullptr;
            } else if(node->parent->left == node) {
                node->parent->left = nullptr;
            } else {
                node->parent->right = nullptr;
            }
            return 1;
        }

        // if the node has one child, replace it with the child
        if(node->left == nullptr) {
            if(node->parent == nullptr) {
                root = node->right;
            } else if(node->parent->left == node) {
                node->parent->left = node->right;
            } else {
                node->parent->right = node->right;
            }
            return 1;
        }
        if(node->right == nullptr) {
            if(node->parent == nullptr) {
                root = node->left;
            } else if(node->parent->left == node) {
                node->parent->left = node->left;
            } else {
                node->parent->right = node->left;
            }
            return 1;
        }
        // use the left child's rightmost node to replace the node
        auto replacement = node->left;
        while(replacement->right != nullptr) {
            replacement = replacement->right;
        }
        // replace the node with the replacement
        if(node->parent == nullptr) {
            root = replacement;
        } else if(node->parent->left == node) {
            node->parent->left = replacement;
        } else {
            node->parent->right = replacement;
        }
        replacement->parent->right = nullptr;
        replacement->parent = node->parent;

        // reinsert the left and right children
        if(node->left != replacement) {
            insert_node(node->left.get());
        }
        insert_node(node->right.get());

        return 1;
    }

};

int main() {
    // test the BST
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    BST<int> bst(arr, 9);

    // test the remove method
    bst.remove(5);
    bst.remove(1);

    // test the insert method
    bst.insert(10);

    return 0;
}
