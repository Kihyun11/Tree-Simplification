#include <iostream>
#include <vector>
#include <string>

// you can include other headers 
// you need for your code

// you can use only headers from the C++ standard library

// you can use any headers from the C++ standard library
// except for the algorithm header

// do not use "using namespace std;"

// do not alter this structured data type definition
struct BNode {
    std::string val;
    // this will contain
    // "xn" (e.g. x3 or x1 etc) in non-leaf nodes
    // "0" or "1" in leaf nodes

    BNode* left;
    // this will conventionally represent the 0 branch
    BNode* right;
    // this will conventionally represent the 1 branch

    // (as usual leaf nodes will have both left and right pointing to NULL)
};

// you can define here other functions to use in the code below

// you can also define here other 
// structured data types or classes 
// (including member data and member functions) 
// to use in the code below

// do not alter the following line that begins the function build_bt

BNode* cons_tree(BNode* t, const int tree_size, int currentsize) {
    if (currentsize == tree_size) {
        t->val = "0";
        return t;
    }

    t->val = 'x' + std::to_string(currentsize + 1);

    BNode* tmp;

    tmp = new BNode;
    t->left = cons_tree(tmp, tree_size, currentsize + 1);

    tmp = new BNode;
    t->right = cons_tree(tmp, tree_size, currentsize + 1);

    return t;
}

BNode* ordered_insertion_tree(BNode* t, int currentsize, std::vector<std::string> fvalues, int i, int j) {

    if (currentsize == fvalues[0].size()) {
        t->val = "1";

        return t;
    }

    if (fvalues[i][j] == '0') {
        t->left = ordered_insertion_tree(t->left, currentsize + 1, fvalues, i, j + 1);

        return t;
    }
    else if (fvalues[i][j] == '1') {
        t->right = ordered_insertion_tree(t->right, currentsize + 1, fvalues, i, j + 1);

        return t;
    }

    return t;

}

BNode* simplification1(BNode* t) {
    if ((t->left->val == "0") && (t->right->val == "0")) {
        t->val = "0";
        t->left = NULL;
        t->right = NULL;
        return t;
    }

    else if ((t->left->val == "1") && (t->right->val == "1")) {
        t->val = "1";
        t->left = NULL;
        t->right = NULL;
        return t;
    }

    else if ((t->left->val != t->right->val)) {
        return t;
    }


    else {
        simplification1(t->left);
        simplification1(t->right);

    }

    return t;


}



BNode* simplification2(BNode* t) {
    if ((t->left->left == NULL) || (t->right->right == NULL)) {
        return t;
    }
    else if ((t->left->left->val == t->right->left->val) && (t->left->right->val == t->right->right->val) && (t->left->left->val[0] != 'x')) {
        t->val = t->left->val;
        t->left->val = t->left->left->val;
        t->right->val = t->right->right->val;
        t->left->left = NULL;
        t->left->right = NULL;
        t->right->left = NULL;
        t->right->right = NULL;
        return t;
    }
    else if ((t->left->left->val[0] == 'x') && (t->right->right->val[0] == 'x')) {
        simplification2(t->left);
        simplification2(t->right);
    }
    else if ((t->left->left->val[0] == 'x') && (t->right->right->val[0] != 'x')) {
        simplification2(t->left);
    }
    else if ((t->left->left->val[0] != 'x') && (t->right->right->val[0] == 'x')) {
        simplification2(t->right);
    }
    return t;

}

void deallocate_tree(BNode* t) {
    if (t != NULL) {
        deallocate_tree(t->left);
        deallocate_tree(t->right);
        delete t;
    }
}

BNode* build_bt(const std::vector<std::string>& fvalues) {
    // complete this function
    BNode* t;
    t = new BNode;
    std::string x;

    t = cons_tree(t, fvalues[0].size(), 0);

    for (int i = 0; i < fvalues.size(); i++) {
        t = ordered_insertion_tree(t, 0, fvalues, i, 0);
    }

    for (int i = 0; i < fvalues[0].size(); i++) {
        simplification1(t);
    }

    for (int i = 0; i < fvalues[0].size(); i++) {
        simplification2(t);
    }

    return t;
}

// do not alter the following function
// this function converts e.g. std::string "x3" to int 2

int label_to_idx(const std::string& label) {

    std::string out;

    for (int i = 1; i < label.size(); i++) {
        out.push_back(label[i]);
    }

    return std::stoi(out) - 1;
}

// do not alter the following function

std::string eval_bt(BNode* bt, const std::string& input) {

    if ((bt->left == NULL) && (bt->right == NULL)) {
        return bt->val;
    }
    else {
        int idx = label_to_idx(bt->val);
        std::string input_idx;
        input_idx.push_back(input[idx]);

        if (input_idx == "0") {
            return eval_bt(bt->left, input);
        }
        else {
            return eval_bt(bt->right, input);
        }
    }
}

// do not alter the following function
int n_nodes_bt(BNode* t) {
    if (t == NULL) {
        return 0;
    }
    else {
        return 1 + n_nodes_bt(t->left) + n_nodes_bt(t->right);
    }
}

class BoolTree {
public:

    BoolTree(const std::vector<std::string>& fvalues) {
        t = build_bt(fvalues);
    }

    std::string eval(const std::string& s) {
        return eval_bt(t, s);
    }

    int n_nodes() {
        return n_nodes_bt(t);
    }

    ~BoolTree() {
        // complete this function 
        // (do not alter in any other way class BoolTree)
        deallocate_tree(t);

        // keep in mind that you can include in this function 
        // calls to functions defined outside class BoolTree
        // (as done in the member functions above)
        // and that you can define other functions
        // above and outside this class



    }

private:
    BNode* t;
};

// the main provided below must work correctly
// with your implementation for the code above
// however you can change it as you wish for your own testing 
// and your changes won't be considered for the assessment
// (in your submission you can also have an empty main or no main at all)

int main() {

    std::vector<std::string> fv;
    std::string row;

    row = "11";
    fv.push_back(row);

    BoolTree ft1(fv);
    // as in the second assignment we give as input only the rows
    // of the truth table whose value is 1
    // (this is an example with the boolean "and" function)

    fv.clear();

    row = "001";
    fv.push_back(row);
    row = "011";
    fv.push_back(row);
    row = "101";
    fv.push_back(row);
    row = "111";
    fv.push_back(row);
    //row = "101";
    //fv.push_back(row);
    //row = "110";
    //fv.push_back(row);
    //row = "111";
    //fv.push_back(row);
    //row = "110";
    //fv.push_back(row);

    BoolTree ft2(fv);
    // this corresponds to the f(x1, x2, x3) example shown above

    //std::cout << ft1.n_nodes() << std::endl;
    // we expect this to print 5

    std::cout << ft2.n_nodes() << std::endl;
    // if the algorithm is such that it builds the smallest possible corresponding tree
    // for the boolean function we are considering
    // then this will print 3 (see tree diagram in the example above)

    //std::cout << ft1.eval("01") << std::endl;
    // this should print "0" 

    //std::cout << ft1.eval("11") << std::endl;
    // this should print "1"

    std::cout << ft2.eval("010") << std::endl;
    // this should print "0"

    std::cout << ft2.eval("110") << std::endl;
    // this should print "1"

}
