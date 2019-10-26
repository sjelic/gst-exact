// g++ "st_red_gst.cpp" --std=c++11 -o "run.exe"
// ./run.exe "data/small.stp" "reduced.stp"

#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <fstream>
#include <sstream>
using namespace std;

class graph
{
  public:

  void addE(int v1, int v2, float d)
  {
    E[make_pair(min(v1, v2), max(v1, v2))] = d;
    if (V.find(v1) == V.end()) 
      V[v1] = set<int>();
    if (V.find(v2) == V.end()) 
      V[v2] = set<int>();
    V[v1].insert(v2);
    V[v2].insert(v1);
  }

  map<pair<int, int>, float> E; // edges
  map<int, set<int>> V; // vertex neighbourhood
  map<int, set<int>> K; // original terminals
  set<int> T; // terminals
};

class stred // steiner tree reduction
{
  public:

  stred(graph& G) : G(G) { }

  graph& reduce()
  {
    float D[G.V.size()][G.V.size()];
    float S[G.V.size()][G.V.size()];
    
    // all-pair shortest path
    for (int i = 0; i < G.V.size(); ++i)
      for (int j = 0; j < G.V.size(); ++j)
        if (i == j) 
          D[i][i] = 0;
        else if (G.E.find(make_pair(min(i, j), max(i, j))) != G.E.end())
          D[i][j] = G.E[make_pair(min(i, j), max(i, j))];  
        else 
          D[i][j] = 1e31;   
   
    for (int i = 0; i < G.V.size(); ++i)
      for (int j = 0; j < G.V.size(); ++j)
        for (int k = 0; k < G.V.size(); ++k)
          D[i][j] = min(D[i][j], D[i][k] + D[k][j]); 

    // special path
    for (int i = 0; i < G.V.size(); ++i)
    {
      for (int j = 0; j < G.V.size(); ++j)
        S[i][j] = D[i][j];
        
      set<int> L; L.insert(i);
      
      while (1)
      {
        bool b = 0;
        for (int j = 0; j < G.T.size(); ++j)
          if (L.find(j) != L.end()) 
          {
            b = 1; 
            break;
          }
        if (b) break;
        
        int m = -1;
        for (int j = 0; j < G.T.size(); ++j)
          if (L.find(j) == L.end() && (m == -1 || S[i][m] > S[i][j]))
            m = j;
        L.insert(m);
        
        for (int j = 0; j < G.V.size(); ++j)
        {
          if (L.find(j) != L.end()) continue;
          S[i][j] = min(S[i][j], max(S[i][m], D[m][j]));
          if (j > m && S[i][j] <= S[i][m])
            L.insert(j);
        }
        
        if (G.V.size() == L.size())
          break;
      }
    }
    
    // prune
    for (auto& it : G.E)
      if (S[it.first.first][it.first.second] < G.E[it.first])
        G.E[it.first] = -1;  
    return G;
  }

  private:

  graph& G;
};

class parser
{
  public:
  
  parser(graph& G) : G(G) { }
  
  graph& parse(string path)
  {
    fstream in(path);
    for(string s; getline(in, s);)
    {
      if (s[0] == 'E' && s[1] == ' ')
      {
        stringstream ss(s);
        int u, v; float d; char c;
        ss >> c >> u >> v >> d; 
        G.addE(u, v, d);
      }
      else if (s[0] == 'T' && s[1] == ' ')
      {
        int v = G.V.size();
        G.K[v] = set<int>();
        G.T.insert(v);
        stringstream ss(s);
        char c; ss >> c;
        for(int u; ss >> u;)
        {
          G.addE(u, v, 1e20);
          G.K[v].insert(u);
        }  
      }
    }
    return G;
  }
  
  private:
  
  graph& G;
};

int main(int argc, char** argv)
{
  graph G; int j = 0;
  stred(parser(G).parse(argv[1])).reduce();
  ofstream out(argv[2]);
  for (auto& it : G.E)
    if (it.second == -1 || G.T.find(it.first.second) != G.T.end()) 
      ++j;
  out << 0 << endl;
  out << "SECTION Comment" << endl;
  out << "Name x" << endl;
  out << "Remark x" << endl;
  out << "END" << endl;
  out << endl;
  out << "SECTION Graph" << endl;
  out << "Nodes " << G.V.size() - G.T.size() << endl;
  out << "Edges " << G.E.size() - j << endl;
  for (auto& it : G.E)
    if (it.second != -1 && G.T.find(it.first.second) == G.T.end()) 
      out << "E " << it.first.first << ' ' << it.first.second << ' ' << it.second << endl;
  out << "END" << endl;
  out << endl;
  out << "SECTION Terminals" << endl;
  out << "Terminals " << G.T.size() << endl;
  for (auto& it : G.K)
  {
    out << "T ";
    for (auto& v : it.second)
      out << v << " ";
    out << endl; 
  }
  out << "END" << endl;
  out << endl;
  out << "EOF";
  cout << "bridova " << G.E.size() << endl;
  cout << "izbacio " << j << endl;
  return 0;
}