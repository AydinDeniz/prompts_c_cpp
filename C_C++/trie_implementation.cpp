
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

class TrieNode {
public:
    bool isEndOfWord; // Marks the end of a word
    std::unordered_map<char, TrieNode*> children; // Map to hold child nodes

    TrieNode() : isEndOfWord(false) {}
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    // Insert a word into the Trie
    void insert(const std::string& word) {
        TrieNode* node = root;
        for (char ch : word) {
            if (node->children.find(ch) == node->children.end()) {
                node->children[ch] = new TrieNode();
            }
            node = node->children[ch];
        }
        node->isEndOfWord = true;
    }

    // Search for a word in the Trie
    bool search(const std::string& word) {
        TrieNode* node = root;
        for (char ch : word) {
            if (node->children.find(ch) == node->children.end()) {
                return false;
            }
            node = node->children[ch];
        }
        return node->isEndOfWord;
    }

    // Check if any words start with a given prefix
    bool startsWith(const std::string& prefix) {
        TrieNode* node = root;
        for (char ch : prefix) {
            if (node->children.find(ch) == node->children.end()) {
                return false;
            }
            node = node->children[ch];
        }
        return true;
    }

    // Suggest words based on a prefix
    void suggestionsHelper(TrieNode* node, std::string currentPrefix, std::vector<std::string>& suggestions) {
        if (node->isEndOfWord) {
            suggestions.push_back(currentPrefix);
        }
        for (auto& child : node->children) {
            suggestionsHelper(child.second, currentPrefix + child.first, suggestions);
        }
    }

    std::vector<std::string> getSuggestions(const std::string& prefix) {
        TrieNode* node = root;
        std::vector<std::string> suggestions;
        for (char ch : prefix) {
            if (node->children.find(ch) == node->children.end()) {
                return suggestions; // No suggestions if prefix not found
            }
            node = node->children[ch];
        }
        suggestionsHelper(node, prefix, suggestions);
        return suggestions;
    }

    // Destructor to free memory
    ~Trie() {
        deleteTrie(root);
    }

private:
    void deleteTrie(TrieNode* node) {
        for (auto& child : node->children) {
            deleteTrie(child.second);
        }
        delete node;
    }
};

int main() {
    Trie trie;
    trie.insert("apple");
    trie.insert("app");
    trie.insert("application");
    trie.insert("apt");
    trie.insert("bat");
    trie.insert("bath");
    trie.insert("batman");

    // Search test
    std::cout << "Searching 'apple': " << (trie.search("apple") ? "Found" : "Not Found") << std::endl;
    std::cout << "Searching 'bat': " << (trie.search("bat") ? "Found" : "Not Found") << std::endl;
    std::cout << "Searching 'batman': " << (trie.search("batman") ? "Found" : "Not Found") << std::endl;
    std::cout << "Searching 'hello': " << (trie.search("hello") ? "Found" : "Not Found") << std::endl;

    // Prefix test
    std::cout << "Prefix 'app': " << (trie.startsWith("app") ? "Exists" : "Doesn't Exist") << std::endl;
    std::cout << "Prefix 'bat': " << (trie.startsWith("bat") ? "Exists" : "Doesn't Exist") << std::endl;

    // Suggestions test
    std::string prefix = "ba";
    std::vector<std::string> suggestions = trie.getSuggestions(prefix);
    std::cout << "Suggestions for prefix '" << prefix << "':" << std::endl;
    for (const auto& suggestion : suggestions) {
        std::cout << suggestion << std::endl;
    }

    return 0;
}
