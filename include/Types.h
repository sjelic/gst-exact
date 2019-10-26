#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <map>
#include <algorithm>
using namespace std;

typedef float  sr;
typedef double dr;
typedef char ch;
typedef string st;
typedef sr Cost;
typedef unsigned int ii;
typedef unsigned int uii;
typedef pair<uii,uii> puii;
typedef pair<uii,sr> puiisr;
typedef pair<sr,uii> psruii;
typedef tuple<sr,uii,uii> tsruii;
typedef vector<tsruii> vtsruii;
typedef vector<ii> vii;
typedef vector<uii> vuii;
typedef vector<bool> vb;
typedef vector<vuii> vvuii;
typedef vector<sr> vsr;
typedef vector<puiisr> vpuiisr;
typedef vector<psruii> vpsruii;
typedef numeric_limits<Cost> srl;
typedef numeric_limits<uii> uiil;
typedef priority_queue<tsruii,vtsruii,greater<tsruii>> MinPQTup;