//This file code have been debugged and until now there is no memory leak
//It took me 23 hours to modify the code, very difficult assignment! Thank you so much!!!
#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

// Node category Node
class Node
{
public:
  Node(const string &_value = "", Node *_par = nullptr, Node *_left = nullptr, Node *_right = nullptr)
      : value(_value), left(_left), right(_right), par(_par) {}

  Node(const Node &other) : value(other.value), left(nullptr), right(nullptr), par(nullptr)
  {
    if (other.left)
    {
      left = new Node(*other.left);
      left->par = this;
    }
    if (other.right)
    {
      right = new Node(*other.right);
      right->par = this;
    }
  }

  Node &operator=(const Node &other)
  {
    value = other.value;
    if (other.left)
    {
      left = new Node(*other.left);
      left->par = this;
    }
    if (other.right)
    {
      right = new Node(*other.right);
      right->par = this;
    }
    return *this;
  }

  // Node(Node &&other) {
  //   par = other.par;
  //   left = other.left;
  //   right = other.right;
  //   value = std::move(other.value);
  //   other.par = nullptr;
  //   other.left = nullptr;
  //   other.right = nullptr;
  // }

  // Node& operator=(Node &&other) {
  //   value = std::move(other.value);
  //   par = other.par;
  //   left = other.left;
  //   right = other.right;
  //   other.par = nullptr;
  //   other.left = nullptr;
  //   other.right = nullptr;
  //   return *this;
  // }

  ~Node()
  {
    if (left)
      delete left;
    if (right)
      delete right;
  }

  Node *left;  // Left subtree pointer
  Node *right; // Right subtree pointer
  Node *par;
  string value;                  // Values stored in the node
  friend class LinkedBinaryTree; // The friendly class LinkedBinaryTree has
                                 // access to the private members of the Node
};

class Position;
typedef list<Position> PositionList; // list of positions

// Binary tree class LinkedBinaryTree
class LinkedBinaryTree
{
public:
  class Position
  { // position in the tree
  private:
    Node *v; // pointer to the node
  public:
    Position(Node *v = nullptr) : v(v) {} // constructor
    string &operator*()                   // get element
    {
      return v->value;
    }
    Position left() const // get left child
    {
      return Position(v->left);
    }
    Position right() const // get right child
    {
      return Position(v->right);
    }
    Position parent() const // get parent
    {
      return Position(v->par);
    }
    bool isRoot() const // root of the tree?
    {
      return v->par == nullptr;
    }

    friend class LinkedBinaryTree; // give tree access
  };

  LinkedBinaryTree()
      : root(nullptr), score(0), n(0)
  {
    // root = new Node;
    // Node *root = new Node; // Create a new node as the root node
    // root->left =
    // nullptr; // The left pointer of the root node points to the empty
    // root->right =
    // nullptr; // The right pointer of the root node points to the empty
    // root->par = nullptr; // The value stored in the root node is empty
  }

  LinkedBinaryTree(const LinkedBinaryTree &other)
      : score(other.score), n(other.n)
  {
    if (other.root)
      root = new Node(*other.root);
  }

  LinkedBinaryTree &operator=(const LinkedBinaryTree &other)
  {
    if (root)
      delete root;
    if (other.root)
      root = new Node(*other.root);
    score = other.score;
    n = other.n;
    return *this;
  }

  LinkedBinaryTree &operator=(LinkedBinaryTree &&other)
  {
    root = other.root;
    score = other.score;
    n = other.n;
    other.root = nullptr;
    return *this;
  }

  LinkedBinaryTree(LinkedBinaryTree &&other)
  {
    score = other.score;
    n = other.n;
    root = other.root;
    other.root = nullptr;
  }

  // class Position {
  ~LinkedBinaryTree()
  {
    if (root)
      delete root;
  };                      // Analytic functions
  void printExpression(); // Output expressions
  double evaluateExpression(double a,
                            double b);             // Calculate the value of the expression
  double getScore();                               // Get Rating
  void setScore(double s);                         // Set rating
  void append(string postfix);                     // Add a node to the expression tree
  int size() const;                                // number of nodes
  bool empty() const;                              // is tree empty?
  Position _root() const;                          // get the root
  PositionList positions() const;                  // list of nodes
  void addRoot();                                  // add root to empty tree
  void expandExternal(const Position &p);          // expand external node
  Position removeAboveExternal(const Position &p); // remove p and parent
  bool operator<(const LinkedBinaryTree &other)
      const
  {                                   // Overloading the less-than operator
    return this->score < other.score; // Determine size by score
  }
  bool isExternal() const // an external node?
  {
    return root->left == nullptr && root->right == nullptr;
  }

private:
  Node *root;   // Root node pointer
  int n;        // Number of nodes
  double score; // Scoring of expressions
};

int LinkedBinaryTree::size() const // number of nodes
{
  return n;
}
bool LinkedBinaryTree::empty() const // is tree empty?
{
  return size() == 0;
}
LinkedBinaryTree::Position LinkedBinaryTree::_root() const // get the root
{
  return Position(root);
}
void LinkedBinaryTree::addRoot() // add root to empty tree
{
  root = new Node;
  n = 1;
}
void LinkedBinaryTree::expandExternal(const Position &p)
{
  Node *v = p.v;       // p’s node
  v->left = new Node;  // add a new left child
  v->left->par = v;    // v is its parent
  v->right = new Node; // and a new right child
  v->right->par = v;   // v is its parent
  n += 2;              // two more nodes
}
LinkedBinaryTree::Position // remove p and parent
LinkedBinaryTree::removeAboveExternal(const Position &p)
{
  Node *w = p.v;
  Node *v = w->par; // get p’s node and parent
  Node *sib = (w == v->left ? v->right : v->left);
  if (v == root)
  {             // child of root?
    root = sib; // . . .make sibling root
    sib->par = nullptr;
  }
  else
  {
    Node *gpar = v->par; // w’s grandparent
    if (v == gpar->left)
      gpar->left = sib; // replace parent by sib
    else
      gpar->right = sib;
    sib->par = gpar;
  }
  delete w;
  delete v; // delete removed nodes
  n -= 2;   // two fewer nodes
  return Position(sib);
}

// Output expressions
void LinkedBinaryTree::printExpression()
{ // Declaring left and right subtree
  // objects
  LinkedBinaryTree left,
      right; // Set the root nodes of the left and right subtrees
  // if (left.root)
  //   delete left.root;
  // if (right.root)
  //   delete right.root;
  left.root = root->left;
  right.root = root->right;
  if (!isExternal())
  { // If the current node is not an external node (leaf
    // node), Different processing according to node values
    if (root->value == "abs")
    {                 //  If the node value is "abs", output "abs("
      cout << "abs("; // Recursive output of the right subtree
      right.printExpression();
    }
    else
    {
      cout << "(";
      left.printExpression(); // Output the value of the current node (operator)
      cout << root->value;
      right.printExpression(); // Recursive output of the right subtree
      cout << ")";
    }
  }
  else
  { // If it is an external node, output the node value (operand)
    // directly
    cout << root->value;
  }
  left.root = nullptr;
  right.root = nullptr;
}

double LinkedBinaryTree::evaluateExpression(double a, double b)
{
  double ans = 0.0;
  const string plusStr = "+"; // define
  const string minusStr = "-";
  const string multiplyStr = "*";
  const string divideStr = "/";
  const string variableAStr = "a";
  const string variableBStr = "b";
  const string greaterThanStr = ">";
  const string absStr = "abs";

  string value = root->value;

  // Create left and right subtrees
  LinkedBinaryTree l, r;
  l.root = root->left;
  r.root = root->right;

  // Use if-else statements for operations
  if (value == variableAStr)
  { // variable a
    ans = a;
  }
  else if (value == variableBStr)
  { // variable b
    ans = b;
  }
  else if (value == plusStr)
  { // add
    ans = l.evaluateExpression(a, b) + r.evaluateExpression(a, b);
  }
  else if (value == minusStr)
  { // minus
    ans = l.evaluateExpression(a, b) - r.evaluateExpression(a, b);
  }
  else if (value == multiplyStr)
  { // minus
    ans = l.evaluateExpression(a, b) * r.evaluateExpression(a, b);
  }
  else if (value == divideStr)
  { // Division
    ans = l.evaluateExpression(a, b) / r.evaluateExpression(a, b);
  }
  else if (value == greaterThanStr)
  { // Comparative Operations
    ans = l.evaluateExpression(a, b) > r.evaluateExpression(a, b) ? 1 : -1;
  }
  else
  { // Other Operators
    if (value == absStr)
    { // Absolute Value Functions
      ans = 1.0 * abs(r.evaluateExpression(a, b));
    }
    else
    { // Return Value
      ans = double(stod(value.c_str()));
    }
  }
  l.root = nullptr;
  r.root = nullptr;
  return ans;
}

// Get the score of the expression tree
double LinkedBinaryTree::getScore() { return score; }

// Set the score of the expression tree
void LinkedBinaryTree::setScore(double s) { score = s; }

// Adding postfix expressions to the expression tree
void LinkedBinaryTree::append(string postfix)
{
  Node *k = new Node; // Create new node
  k->left = nullptr;
  k->right = nullptr;
  int i = postfix.size() -
          1; // Get the index of the last character of the expression

  if (postfix[i] < '0' ||
      postfix[i] > '9' && postfix[i] != 'a' && postfix[i] != 'b' &&
          postfix[i] != 's')
  {                        // Other operators (two subtrees)
    k->value = postfix[i]; // Set the value of the node to the operator
    if (root)
      delete root;
    root = k;        // Set the new node as the root node
    i -= 2;          // Update Index
    int operate = 0; // Calculating the number of operators
    int number = 0;  // Calculate the number of digits
    for (int j = i; j >= 0;
         j--)
    { // Determine the demarcation point of the left and right
      // subtrees based on the relationship between the operator and
      // the number
      if (postfix[j] == 's')
      {
      }
      else if (postfix[j] < '0' ||
               postfix[j] > '9' && postfix[j] != 'a' && postfix[j] != 'b')
      {
        operate++;
      }
      else
      {
        number++;
      }
      while (postfix[j] != ' ')
        j--; // Skip space
      if (number >
          operate)
      {                                       // Need one more number to determine the cut-off point
        LinkedBinaryTree lefttree, righttree; // Create left and right subtrees
        lefttree.append(postfix.substr(
            0, j)); // Recursive add an expression for the left subtree
        righttree.append(postfix.substr(
            j + 1, i - j)); // Recursive add an expression for the right subtree
        root->left =
            lefttree.root; // Set the left subtree to the left child node
        root->right =
            righttree.root; // Set the right subtree to the right child node
        lefttree.root->par = root;
        righttree.root->par = root;
        lefttree.root = nullptr;
        righttree.root = nullptr;

        break;
      }
    }
  }
  else if (postfix[i] == 's')
  {                                      // The abs operator (a subtree)
    k->value = postfix.substr(i - 2, 3); // Set the value of the node to "abs"
    i -= 4;                              // Update Index
    if (root)
      delete root;
    root = k;                                 // Set the new node as the root node
    LinkedBinaryTree newtree;                 // Create a new expression tree
    newtree.append(postfix.substr(0, i + 1)); // Recursive add sub-expressions
    root->right = newtree.root;               // Set the new tree as the right subtree
    newtree.root->par = root;
    newtree.root = nullptr;
  }
  else
  { // number
    k->value =
        postfix.substr(0, i + 1); // Set the value of the node to a number
    if (root)
      delete root;
    root = k; // Set the new node as the root node
    root->left = nullptr;
    root->right = nullptr;
  }
}
// This function accepts a suffix expression, creates an expression tree using
// the LinkedBinaryTree data structure, and returns the root node of the tree.
LinkedBinaryTree createExpressionTree(string postfix)
{
  // Create an empty LinkedBinaryTree object
  LinkedBinaryTree root;

  // Add postfix expressions to the tree and build the expression tree
  root.append(postfix);

  // Returns the root node of the expression tree
  return root;
}

int main()
{
  // Read postfix expressions into vector of expression trees
  vector<LinkedBinaryTree> trees;

  ifstream exp_file("expressions.txt");
  string line;
  while (getline(exp_file, line))
  {
    trees.push_back(createExpressionTree(line));
  }

  // Read input data into 2D vector of doubles
  vector<vector<double>> inputs;
  ifstream input_file("input.txt");
  while (getline(input_file, line))
  {
    vector<double> ab_input;
    stringstream ss(line);
    string str;
    while (getline(ss, str, ' '))
    {
      ab_input.push_back(double(stod(str.c_str())));
    }
    inputs.push_back(ab_input);
  }

  // Evaluate expressions on input data
  for (auto &t : trees)
  {
    double sum = 0;
    for (auto &i : inputs)
    {
      sum += t.evaluateExpression(i[0], i[1]);
    }
    t.setScore(sum / inputs.size());
  }
  // Sort trees by their score
  sort(trees.begin(), trees.end());

  for (auto &t : trees)
  {
    cout << "Exp ";
    t.printExpression();
    cout << " Score " << t.getScore() << endl;
  }
  return 0;
}
