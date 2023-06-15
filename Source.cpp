#include <iostream>
#include <vector>
#include<queue>

using namespace std;



const double a = 0.75;
int max_size = 0;

struct Node {
    int data;
    Node* left;
    Node* right;
};

// create a new node
Node* createNode(int data) {
    Node* newNode = new Node();
    if (!newNode) {
        cout << "Memory error\n";
        return NULL;
    }
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}


void levelOrderTraversal(Node* root) { // shows data in level order traversal.
    if (root == NULL) {
        return;
    }
    cout << "Level Order Traversal: " << endl;
    queue<Node*> queue;
    queue.push(root);

    while (!queue.empty()) {
        Node* current = queue.front();
        queue.pop();

        cout << current->data << " ";

        if (current->left) {
            queue.push(current->left);
        }
        if (current->right) {
            queue.push(current->right);
        }
    }
    cout << endl;
}

void extractdata(Node* x, vector<int>& values) { //extracts data in an inorder traversal way.
    if (x == NULL) {
        return;
    }
    extractdata(x->left, values);
    values.push_back(x->data);
    extractdata(x->right, values);
}

int sizetree(Node* x) { // calculates the size of the tree
    if (x == NULL) {
        return 0;
    }
    return 1 + sizetree(x->left) + sizetree(x->right);
}

Node* buildnewTree(vector<int> values, int start, int end) { // builds a new 1/2 balanced tree from extracted data.
    if (start > end) {
        return NULL;
    }

    int mid = (start + end) / 2;
    Node* root = createNode(values[mid]);

    root->left = buildnewTree(values, start, mid - 1);
    root->right = buildnewTree(values, mid + 1, end);

    return root;
}
Node* reassigntree(Node* x, Node* parent) {
    cout << "Imbalanced tree: ";
    levelOrderTraversal(x);
    vector<int> result;
    extractdata(x, result); //extracts data from imbalnced tree.
    sort(result.begin(),result.end()); // sorts extracted data.
    Node* newroot = buildnewTree(result,0,result.size()-1); //builds new tree
    if (parent != NULL) { //assigns new root of tree to parent if the parent exists.
        if (x == parent->left) { parent->left = newroot; }
        else if (parent->right == x) { parent->right = newroot; }
    }
    else { // If there is no parent it compares the tree size to the max size.
        if (max_size < sizetree(newroot)) { max_size = sizetree(newroot); };
    }
    cout << "Tree Reassigned!" << endl;
    levelOrderTraversal(newroot);
    return newroot;
}

Node* checkNode(Node* x, Node*parent) {
    if (sizetree(x->left) <= a * sizetree(x) && sizetree(x->right) <= a * sizetree(x)) {
        return x; // tree is a-balanced.
    }
    else {
        cout << "Imbalance detected!" << endl; //tree is not a-balanced.
        Node* newroot = reassigntree(x,parent);
        return newroot;
    }

}


Node* insertNode(Node* root, int data,Node* parent) {
    if (root == NULL) { // if the node is null it creates a new one and inserts the data.
        root = createNode(data);
        cout << data << " inserted!" << endl;
    }
    if (data < root->data) {  
        root->left = insertNode(root->left, data,root); // if data are smaller check left child if it is null.
    }
    else if (data > root->data) {
        root->right = insertNode(root->right, data,root); // if data are not smaller check right child if it is null.
    }
    else {
    }
    root = checkNode(root,parent); //check if tree is a-balanced.
    return root;
}



void findrange(Node* x, int N1 , int N2) { //prints data in the subtree of root in an inorder traversal way.
    if (x == NULL) {
        return;
    }
    findrange(x->left, N1 , N2);
    if (x->data > N1 && x->data < N2) { cout << x->data << " "; }
    findrange(x->right, N1 , N2);
}

Node* findNeighbour(Node* root, int data, string direction) {
    if (direction == "p") { // if it looks for the predecessor
        if (root->data == data) {
            cout << "This node has no predecessor" << endl;
            return root;
        }
        if (root == NULL) {
            cout << "Could not find root on the tree!" << endl;
            return root;
        }
        if (root->left->data == data || root->right->data==data) {
            cout << "The node's predecessor is: " << root->data << endl;
            return root;
        }
        if (data < root->data) {   
            return findNeighbour(root->left, data, direction);
        }
        else if (data > root->data) {
            return findNeighbour(root->right, data, direction);
        }
    }
    else { // if it looks for the successor
        if (root->data == data) {
            if (root->left != NULL) {
                cout << "left successor of Node: " << root->left->data << endl;
            }
            if (root->right != NULL) {
                cout << "right successor of Node: " << root->right->data << endl;
            }
            cout << endl;
            return root;
        }
        if (root == NULL) {
            cout << "Could not find root on the tree!" << endl;
            return root;
        }
        if (data < root->data) {
            return findNeighbour(root->left, data, direction);
        }
        else if (data > root->data) {
            return findNeighbour(root->right, data, direction);
        }
    }
    return root;
}

Node* searchNode(Node* root, int data , int N1 , int N2) {  // searches for the node 
    if (root->data == data){
        findrange(root, N1, N2); // if it finds the node it prints the numbers in range
        cout << endl;
        return root;
    }
    if (root == NULL) {
        cout << "Could not find root on the tree!" << endl;
        return root;
    }
    if (data < root->data) {
        return searchNode(root->left, data , N1 ,N2);
    }
    else if (data > root->data) {
        return searchNode(root->right, data , N1 , N2 );
    }
    
}


Node* minValueNode(Node* node) { // searches for the node with the smallest value in the right subtree of deleted node.
    Node* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

Node* deleteNode(Node* root, int data) { //deletes Node 
    if (root == NULL)
        return root;
    if (data < root->data)
        root->left = deleteNode(root->left, data);
    else if (data > root->data)
        root->right = deleteNode(root->right, data);
    else { //finds the Node to delete
        if (root->left == NULL) { // if the Node has only a right Child
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == NULL) { // if the Node has only a left child
            Node* temp = root->left;
            delete root;
            return temp;
        }
        Node* temp = minValueNode(root->right); // if the node has to children. 
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}



int main() {
    Node* root = NULL;
    Node* parent = NULL;
    string input , direction = " ";
    int k , l ,m;

    root = insertNode(root, 80, parent);
    root = insertNode(root, 70, parent);
    root = insertNode(root, 60, parent);
    root = insertNode(root, 50, parent);
    root = insertNode(root, 40, parent);
    root = insertNode(root, 30, parent);
    root = insertNode(root, 20, parent);
    do {
        cout << "Please select the function you want: " << endl << endl <<
            "--- Insert A New Node. (Insert)" << endl <<
            "--- Delete A Node. (Delete)" << endl <<
            "--- Show Tree. (Show)" << endl <<
            "--- Search A Range of Nodes. (Search)" << endl <<
            "--- Find The Neighbour of a Node. (Neighbour)" << endl <<
            "--- Exit Application. (Exit)" << endl << endl;
        cin >> input;
        cout << endl;

        if (input == "Insert") {
            cout << "Please type the number you want to insert: " << endl;
            cin >> k;
            root = insertNode(root, k, parent);
        }
        else if (input == "Delete") {
            cout << "Please type the number you want to insert: " << endl;
            cin >> k;
            root = deleteNode(root, k);
            if (sizetree(root) < a * max_size) {
                reassigntree(root, parent);
                max_size = sizetree(root);
            }
        }
        else if (input == "Show") {
            levelOrderTraversal(root);
        }
        else if (input == "Search") {
            cout << "Please type the root of the tree you want to search: " << endl;
            cin >> k;
            cout << endl << "Please type the Range of Number you want to show: " << endl;
            cin >> l >> m;
            cout << endl;
            searchNode(root, k, l, m);
        }
        else if (input == "Neighbour") {
            cout << "Please type the number you want to find the neighbour for: " << endl;
            cin >> k;
            cout << "PLease type (p) for predecessor  or (s) for successor: " << endl;
            do {
                cin >> direction;
                if (direction == "s" || direction == "p") {
                    findNeighbour(root, k, direction);
                } 
            } while (direction != "s" && direction != "p");
            
        }
        else if (input == "Exit") {
            cout << "Exiting Application..." << endl;
        }
        else {
            cout << "Not a valid input." << endl;
        }
    } while (input != "Exit");

    levelOrderTraversal(root);
}
