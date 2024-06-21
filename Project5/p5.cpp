#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class HuffNode {
public:
    HuffNode* left;
    HuffNode* right;
    char alpha;
    HuffNode(int ascii) {
        alpha = ascii;
        left = nullptr;
        right = nullptr;
    }
    ~HuffNode() {
        delete left;
        delete right;
    }
};

HuffNode* buildTree(vector<int>& level) {
    vector<HuffNode*> pQueue;
    HuffNode* root = new HuffNode(level[0]);
    level.erase(level.begin());
    pQueue.push_back(root);

    while(pQueue.size() > 0) {
        HuffNode* left = new HuffNode(level[0]);
        HuffNode* right = new HuffNode(level[1]);

        pQueue[0]->left = left;
        pQueue[0]->right = right;
        
        if(level[0] >= 128) {
            pQueue.push_back(left);
        }
        if(level[1] >= 128) {
            pQueue.push_back(right);
        }
        level.erase(level.begin());
        level.erase(level.begin());
        pQueue.erase(pQueue.begin());
    }

    return root;
};

void decode(HuffNode* root, string msg) {
    HuffNode* curNode = root;
    for(int i = 0; i <= int(msg.length()); i++) {
        if(!(curNode->left && curNode->right)) {
            cout << curNode->alpha;
            curNode = root;
        }
        if(msg[i] == '0') {
            curNode = curNode->left;
        }
        else {
            curNode = curNode->right;
        }
    }
}

int main(int argc, char *argv[]) {
    string levelOrder = argv[2];
    string codedMSG = argv[3];
    vector<int> level;
    string ascii, line, msg;
    
    //read in the levelOrder traversal to vector<int>
    ifstream inFS(levelOrder);
    while(getline(inFS, line)) {
        istringstream iss(line);
        while(getline(iss, ascii, ' ')) {
            level.push_back(stoi(ascii));
        }
        iss.clear();
    }
    inFS.close();

    //build Huffman tree concept using function, returning root
    HuffNode* root = buildTree(level);

    inFS.open(codedMSG);
    while(getline(inFS, line)) {
        istringstream iss(line);
        while(getline(iss, ascii)) {
            msg += ascii;
        }
    }
    inFS.close();

    decode(root, msg);

    return 0;
}