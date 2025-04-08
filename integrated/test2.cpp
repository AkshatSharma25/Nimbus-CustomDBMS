#include "headers/bptree.h"
#include <iostream>

int main() {
    // Test with integer keys
    BPlusTree<int, std::string> intTree;
    intTree.insert(10, "Ten");
    intTree.insert(20, "Twenty");
    intTree.insert(5, "Five");
    intTree.insert(15, "Fifteen");
    intTree.insert(25, "Twenty-five");
    
    std::cout << "Integer B+ Tree Structure:" << std::endl;
    intTree.print();
    
    // Test search
    auto test_search = [&](int key) {
        auto result = intTree.search(key);
        if (result) {
            std::cout << "Found " << key << ": " << *result << std::endl;
        } else {
            std::cout << "Key " << key << " not found" << std::endl;
        }
    };
    
    test_search(10);
    test_search(15);
    test_search(99); // Should not be found
    
    // Test range query
    std::cout << "\nRange query [8, 22]:" << std::endl;
    for (const auto& p : intTree.range_query(8, 22)) {
        std::cout << p.first << ": " << p.second << std::endl;
    }
    
    // Test with string keys
    BPlusTree<std::string, std::string> stringTree;
    stringTree.insert("apple", "hello");
    stringTree.insert("banana", "world");
    stringTree.insert("orange", "hello");
    
    std::cout << "\nString B+ Tree Structure:" << std::endl;
    stringTree.print();
    
    // Test string search
    auto str_result = stringTree.search("banana");
    if (str_result) {
        std::cout << "\nFound 'banana': " << *str_result << std::endl;
    } else {
        std::cout << "'banana' not found" << std::endl;
    }
    
    str_result = stringTree.search("grape");
    if (str_result) {
        std::cout << "Found 'grape': " << *str_result << std::endl;
    } else {
        std::cout << "'grape' not found" << std::endl;
    }
    
    return 0;
}