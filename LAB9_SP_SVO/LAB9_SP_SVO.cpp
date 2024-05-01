#include <iostream>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <string>

struct Magazine {
    std::string name;
    std::string publisher;
    double price;
    std::string ISSN;
};

class BinarySearchTree {
private:
    struct Node {
        Magazine data;
        Node* left;
        Node* right;
        Node(const Magazine& magazine) : data(magazine), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void destroyTree(Node* leaf) {
        if (leaf != nullptr) {
            destroyTree(leaf->left);
            destroyTree(leaf->right);
            delete leaf;
        }
    }

    void insert(const Magazine& magazine, Node*& leaf) {
        if (leaf == nullptr) {
            leaf = new Node(magazine);
        }
        else if (magazine.publisher + magazine.name < leaf->data.publisher + leaf->data.name) {
            insert(magazine, leaf->left);
        }
        else if (magazine.publisher + magazine.name > leaf->data.publisher + leaf->data.name) {
            insert(magazine, leaf->right);
        }
        else {
            throw std::invalid_argument("Duplicate key: magazine already exists in the tree.");
        }
    }

    Node* search(const std::string& key, Node* leaf) {
        if (leaf == nullptr) {
            return nullptr;
        }

        if (key == leaf->data.publisher + leaf->data.name) {
            return leaf;
        }
        else if (key < leaf->data.publisher + leaf->data.name) {
            return search(key, leaf->left);
        }
        else {
            return search(key, leaf->right);
        }
    }

    Node* findMin(Node* leaf) {
        while (leaf->left != nullptr) {
            leaf = leaf->left;
        }
        return leaf;
    }

    void remove(const std::string& key, Node*& leaf) {
        if (leaf == nullptr) {
            return;
        }

        if (key == leaf->data.publisher + leaf->data.name) {
            if (leaf->left == nullptr && leaf->right == nullptr) {
                delete leaf;
                leaf = nullptr;
            }
            else if (leaf->left == nullptr) {
                Node* temp = leaf;
                leaf = leaf->right;
                delete temp;
            }
            else if (leaf->right == nullptr) {
                Node* temp = leaf;
                leaf = leaf->left;
                delete temp;
            }
            else {
                Node* minRight = findMin(leaf->right);
                leaf->data = minRight->data;
                remove(minRight->data.publisher + minRight->data.name, leaf->right);
            }
        }
        else if (key < leaf->data.publisher + leaf->data.name) {
            remove(key, leaf->left);
        }
        else {
            remove(key, leaf->right);
        }
    }

    void getAllDescriptions(std::vector<std::string>& descriptions, Node* leaf) {
        if (leaf != nullptr) {
            getAllDescriptions(descriptions, leaf->left);
            std::string description = "Name: " + leaf->data.name +
                ", Publisher: " + leaf->data.publisher +
                ", Price: " + std::to_string(leaf->data.price) +
                ", ISSN: " + leaf->data.ISSN;
            descriptions.push_back(description);
            getAllDescriptions(descriptions, leaf->right);
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() {
        destroyTree(root);
    }

    void insert(const Magazine& magazine) {
        insert(magazine, root);
    }

    void remove(const std::string& key) {
        remove(key, root);
    }

    bool contains(const std::string& key) {
        return search(key, root) != nullptr;
    }

    std::vector<std::string> getAllDescriptions() {
        std::vector<std::string> descriptions;
        getAllDescriptions(descriptions, root);
        return descriptions;
    }
};

int main() {
    try {
        BinarySearchTree bst;
        Magazine mag1{ "Magazine1", "Publisher1", 10.99, "1234-5678" };
        Magazine mag2{ "Magazine2", "Publisher2", 8.99, "5678-1234" };

        bst.insert(mag1);
        bst.insert(mag2);

        std::vector<std::string> allDescriptions = bst.getAllDescriptions();
        for (const auto& description : allDescriptions) {
            std::cout << description << std::endl;
        }

        bst.remove("Publisher1Magazine1");

        std::cout << "Magazine1 is in the tree: " << std::boolalpha << bst.contains("Publisher1Magazine1") << std::endl;
        std::cout << "Magazine3 is in the tree: " << std::boolalpha << bst.contains("Publisher3Magazine3") << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}