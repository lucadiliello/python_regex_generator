// C implementation of search and insert operations
// on Trie
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdbool>
#include <vector>
#include <fstream>

using namespace std;
 
#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])
 
// Alphabet size (# of symbols)
#define ALPHABET_SIZE 127
 
// Converts key current character into index
// use only 'a' through 'z' and lower case

int CHAR_TO_INDEX(char c){
	return (int)c;
}

char INDEX_TO_CHAR(int i){
	return (char)i;
}

string join(string s, vector<string> v){
	if(!v.size()) return "";
	string res = v[0];
	for(int i = 1; i < v.size(); i++) res += s + v[i];
	return res;
}


// trie node
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
 
    // isLeaf is true if the node represents
    // end of a word
    bool isLeaf;
    int keyNumber;
};
 
// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void)
{
    struct TrieNode *pNode = NULL;
 
    pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
 
    if (pNode)
    {
        int i;
 
        pNode->isLeaf = false;
        pNode->keyNumber = 0;
 
        for (i = 0; i < ALPHABET_SIZE; i++)
            pNode->children[i] = NULL;
    }
 
    return pNode;
}
 
// If not present, inserts key into trie
// If the key is prefix of trie node, just marks leaf node
void insert(struct TrieNode *root, string key)
{
    int level;
    int length = key.length();
    int index;
 
    struct TrieNode *pCrawl = root;
 
    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

 		pCrawl->keyNumber++;
        pCrawl = pCrawl->children[index];

    }
 
    // mark last node as leaf
    pCrawl->isLeaf = true;
}
 
// Returns true if key presents in trie, else false
bool search(struct TrieNode *root, string key)
{
    int level;
    int length = key.length();
    int index;
    struct TrieNode *pCrawl = root;
 
    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);
 
        if (!pCrawl->children[index])
            return false;
 
        pCrawl = pCrawl->children[index];
    }
 
    return (pCrawl != NULL && pCrawl->isLeaf);
}


string to_regex(struct TrieNode *root){
	if(root->isLeaf && (root->keyNumber == 0)) return "";
	vector<string> alt;
	vector<string> cc;
	int q = 0;

	if(root->isLeaf){
		q = 1;
	}

	for(int i = 0; i < ALPHABET_SIZE; i++){
		if(root->children[i]){

			string recurse = to_regex(root->children[i]);
			if(recurse != "") alt.push_back(string(1, INDEX_TO_CHAR(i)) + recurse);
			else cc.push_back(string(1, INDEX_TO_CHAR(i)));
		
		}
	}

	bool cconly = !(alt.size() > 0);

	if(cc.size() > 0){
		if(cc.size() == 1){
			alt.push_back(cc[0]);
		} else {
			alt.push_back("[" + join("",cc) + "]");
		}
	}
	
	string result;

	if(alt.size() == 1){
		result = alt[0];
	} else {
		result = "(?:" + join("|",alt) + ")";
	}


	if(q){
		if(cconly){
			result += "?";
		} else {
			result = "(?:" + result + ")?";
		}
	}
	return result;
}


int main(int argc, char * argv[])
{
    if(argc != 3){
        cerr << "Use: ./executable strings_file output_file" << endl;
        exit(1);
    }

    struct TrieNode *root = getNode();
 
    ifstream infile(argv[1]);
    ofstream offile(argv[2]);

    string line;

    while (getline(infile, line))
	{
	    insert(root, line);
	}
    cout << "Names imported successfully" << endl;
    cout << "Generating ..." << endl;
    offile << to_regex(root);
    cout << "Done !" << endl;
    return 0;
}