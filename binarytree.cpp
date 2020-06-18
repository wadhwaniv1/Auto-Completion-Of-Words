#include <iostream>
#include <algorithm>
#include <fstream>
#include <cctype>
#include <vector>
#include <time.h>
#include <string>
using namespace std;
class Node{
char data;
bool mMarker;
Node *right, *left;
public:
Node(){
data = ' ';
mMarker = false;
right = NULL;
left = NULL;
}
~Node();
char returnData(){
return data;
}
void setData(char c){
data = c;
}
bool wordMarker(){
return mMarker;
}
void setWordMarker(){
mMarker = true;
}
void appendRight(Node *node){
right = node;
}
void appendLeft(Node *node){
left = node;
}
Node *getRight(){
return right;
}
Node *getLeft(){
return left;
}
};
Node *getSameLevel(Node *node, char c){
if (node == NULL)
return NULL;
else if (node->returnData() == c)
return node;
else
getSameLevel(node->getRight(), c);
}
class BinaryTree{
Node *root;
public:
BinaryTree();
void addWord(string);
Node *searchWord(string, bool);
bool autoComplete(string, vector<string> &);
void parseTree(Node *, string &, vector<string> &, bool &);
};
BinaryTree::BinaryTree()
{
root = new Node();
}
void BinaryTree::addWord(string s)
{
Node *current = root;
int len = s.length();
if (s.length() == 0)
return;
for (int i = 0; i < len; ++i)
{
Node *next = getSameLevel(current, s[i]);
if (next != NULL)
{
if (i < len - 1)
{
if (next->getLeft() == NULL)
{
Node *nNode = new Node();
nNode->setData(s[i + 1]);
if (i == len - 1)
nNode->setWordMarker();
next->appendLeft(nNode);
//i++;
}
current = next->getLeft();
}
}
else
{
Node *tmp = current;
Node *nNode = new Node();
nNode->setData(s[i]);
while (tmp->getRight() != NULL)
tmp = tmp->getRight();
tmp->appendRight(nNode);
current = tmp->getRight();
i--;
}
if (i == len - 1)
next->setWordMarker();
}
}
Node *BinaryTree::searchWord(string s, bool word = true)
{
Node *current = root->getRight(), *prev = NULL;
int len = s.length();
for (int i = 0; i < len; ++i)
{
Node *next = getSameLevel(current, s[i]);
if (next == NULL)
return NULL;
prev = next;
current = next->getLeft();
}
if (word)
{
if (current->wordMarker() == true)
return prev;
else
return NULL;
}
else
{
//if(prev)
return prev;
//else
// return NULL;
}
}
bool BinaryTree::autoComplete(string s, vector<string> &res)
{
Node *current = root;
Node *tmp = searchWord(s, false);
if (tmp == NULL || tmp->getLeft() == NULL)
return false;
current = tmp->getLeft();
s += current->returnData();
bool loop = true;
parseTree(current, s, res, loop);
return true;
}
void BinaryTree::parseTree(Node *current, string &s, vector<string> &res,
bool &loop)
{
if (loop)
{
if (current->wordMarker())
{
res.push_back(s);
if (res.size() > 15)
loop = false;
}
if (current->getLeft())
{
Node *left = current->getLeft();
s += left->returnData();
//cout << "*" << left->content();
parseTree(left, s, res, loop);
}
else
{
s.erase(s.size() - 1);
//return;
}
if (current->getRight())
{
Node *right = current->getRight();
s += right->returnData();
//cout << "#" << right->content();
parseTree(right, s, res, loop);
}
else
{
s.erase(s.size() - 1);
return;
}
}
}
bool loadDictionary(BinaryTree* bt, string filename)
{
ifstream words;
words.open("words.txt"); //filename.c_str()
if (!words.is_open()) {
cout << "Dictionary file Not Open" << endl;
return false;
}
while (!words.eof()) {
string s;
words >> s;
bt->addWord(s);
}
return true;
}
bool isValid(string s)
{
if(!isalpha(s[0]))
return false;
return true;
}
int main()
{
BinaryTree *bt = new BinaryTree();
int mode;
cout << "Autocomplete Feature" << endl;
loadDictionary(bt, "words.txt");
while (1) {
cout << endl
<< endl;
cout << "Enter choice: " << endl;
cout << "1: Enter a prefix string" << endl;
cout << "2: Quit" << endl
<< endl;
cin >> mode;
switch (mode) {
case 1: //Auto complete
{
clock_t start, finish;
string s;
char addNew;
cin >> s;
start = clock();
if(!isValid(s))
{
cout<<"Enter valid word."<<endl;
break;
}
transform(s.begin(), s.end(), s.begin(), ::tolower);
vector<string> autoCompleteList;
bt->autoComplete(s, autoCompleteList);
if (autoCompleteList.size() == false) {
cout << "No suggestions" << endl;
cout<<"Want to add this to the tree?(y/n): ";
cin>>addNew;
if(addNew == 'y'|| addNew == 'Y')
{
bt->addWord(s);
cout<<"Word "<<s<<" added to the tree"<<endl;
}
else
cout<<"Word "<<s<<" was not added to the tree"<<endl;
}
else {
cout << "list of words: " << endl;
for (int i = 0; i < autoCompleteList.size(); i++) {
cout << "\t \t " << autoCompleteList[i] << endl;
}
}
finish = clock();
cout<< "Time for algorithm to run in binary tree (in seconds) is"<<((double)(finish-start))/CLOCKS_PER_SEC;
}
continue;
case 2:
delete bt;
return 0;
default:
continue;
}
}
}
