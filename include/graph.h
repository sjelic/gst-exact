#include "Types.h"


typedef vector<vpuiisr> Graph;
typedef vvuii DenseGraph;
typedef uii Node;
typedef queue<Node> NodeQueue;
typedef vector<Node> NodeList;
typedef uii Group;
//typedef vector<GRBVar> NodeVars;
//typedef vector<NodeVars> EdgeVars;
typedef vvuii GroupNodeList;
typedef vvuii NodeGroupList;
typedef vuii ParentTree;
typedef vector<ParentTree> ParentTrees;
typedef vector<Cost> Costs;
typedef vector<Costs> Dists;
typedef vuii BFSDist;
typedef vvuii BFSDists;
typedef vector<unordered_map<Node,Cost>> EdgeContainer;




class Edge
{
public:
    Node u;
    Node v;
    Cost w;
    Edge(){};
    Edge(uii u, uii v, Cost w = 1) : u(min(u,v)), v(max(u,v)), w(w){};
    bool operator==(const Edge& e) const { return w == e.w; }
    bool operator<(const Edge& e) const { return w < e.w; }
    bool operator<=(const Edge& e) const { return w <= e.w; }
    bool operator>(const Edge& e) const { return w > e.w; }
    //Edge(const Edge& e) : u(e.u), v(e.v), w(e.w){};u <
    ~Edge(){};
};

inline Node u(Edge& e) {return e.u;}
inline Node v(Edge& e) {return e.v;}
inline Cost c(Edge& e) {return e.w;}


//Edge make_edge(uii u, uii v, Cost w = 1) return Edge(u,v,w);

typedef vector<Edge> EdgeList;

void load_graph(ifstream& fin, Graph& G, EdgeList& el, uii& n, uii& m)
{
    st str;
    ch mark;
    uii s,t;
    Cost w;
    getline(fin,str); // Section GRAPH
    fin>>str>>n>>str>>m; // uèitavanje dimenzija grafa
    G.resize(n);
    el.resize(m);
    for(uii i = 0; i < m; i++)
    {
        if( (! (fin>>mark)) || mark != 'E'){cout<<" ... INVALID INSTANCE ... EXIT."<<endl<<endl; return;}
        fin>>s>>t>>w;
        G[s].push_back(make_pair(t,w));
        G[t].push_back(make_pair(s,w));
        //cout<<s<<" "<<t<<" "<<w<<endl;
        el[i] = Edge(s,t,w);
    }
    getline(fin,str); getline(fin,str);
    return;

}

void save_graph(st fn, EdgeList& el, uii n)
{
    ofstream solF(fn);
    solF<<"SECTION Graph"<<endl;
    solF<<"Nodes "<<n<<endl<<"Edges "<<el.size()<<endl;
    for(auto& e : el)  solF<<"E"<<" "<<u(e)<<" "<<v(e)<<" "<<c(e)<<endl;    
    solF<<"END"<<endl;
    solF.close();
    return;

}

void load_instance(st fN, Graph& G, GroupNodeList& gv, NodeGroupList& vg, EdgeList& el, uii& n, uii& m, uii& k)
{
    ifstream fin(fN);
    if(!fin.is_open()){ cout<<" ... OPENING FILE FAILED ... EXIT."<<endl<<endl; return;};
    uii s;
    //sr w;
    st str;
    ch mark;
    for(uii i=0; i<6; i++) getline(fin,str);

    load_graph(fin,G,el,n,m);

    getline(fin,str); getline(fin,str);
    if( (! (fin>>str>>k)) || str != "Terminals"){cout<<" ... INVALID INSTANCE ... EXIT."<<endl<<endl; return;}

      /*READING GROUPS*/
    getline(fin,str);
    if(k == 0) {cout<<" ... INVALID INSTANCE ... EXIT."<<endl<<endl; return;}

    gv.resize(k);
    vg.resize(n);

    // UÈITAVANJE GRUPA
    for(Group g = 0; g < k; g++)
    {
        getline(fin, str, '\n');
        istringstream iss(str);
        if( (! (iss>>mark)) || mark != 'T'){cout<<" ... INVALID INSTANCE ... EXIT."<<endl<<endl; return;}
        while (iss >> s)
        {
            gv[g].push_back(s); // u grupu g dodajem vrh s
            vg[s].push_back(g); // vrh s pripada grupi g
        }
    }
    fin.close();
}

bool operator> (const puiisr &p, const puiisr &q)
{
    return (p.second > q.second);
}

bool operator== (const puiisr &p, const puiisr &q)
{
    return (p.second == q.second);
}
void dijkstra(Costs& D, ParentTree& P, Graph& G, Node s, uii n, uii m)
{
	priority_queue<psruii,vpsruii,greater<psruii>> pq;
	pq.push(psruii(D[s],s));
	while(! pq.empty())
    {
        puiisr curr = pq.top();
        pq.pop();
        if (curr.first > D[curr.second]) continue;
        for (auto& e : G[curr.second])
        {
            if (D[e.first] > D[curr.second] + e.second)
            {
                D[e.first] = D[curr.second] + e.second;
                pq.push(psruii(D[e.first],e.first));
                P[e.first] = curr.second;
            }
        }
    }
}

void all_pairs_shorthest_paths(Dists& D, ParentTrees& P, Graph& G, uii n, uii m)
{
	D.resize(n); P.resize(n);
	for (Node v = 0; v < n; v++){ D[v].resize(n,numeric_limits<Cost>::max()); D[v][v] = 0;}
	for (Node v = 0; v < n; v++){ P[v].resize(n,v);}
	for (Node v = 0; v < n; v++)
    {
        dijkstra(D[v],P[v],G,v,n,m);
    }
}
/*void floyd_warsall(Dists& D, ParentTrees& P, DenseGraph& G, uii n, uii m)
{

}*/

inline void process(Graph& G, vb& S, vb& vs, MinPQTup& pq,  Node c)
{
    vs[c] = true;
    for (auto& n : G[c]) if (S[n.first] && !vs[n.first]) pq.push(tsruii(n.second,n.first,c));
}

sr mst_prim(Graph& G, EdgeList& T, vb& S, uii n)
{
    sr cT = 0;
    Node s;
    for (s = 0; s < n && !S[s]; s++);
    T.reserve(n-1);
    MinPQTup pq;
    vb vs(n,false); vs[s]=true;
    process(G,S,vs,pq,s);
    while(!pq.empty())
    {
        tsruii e = pq.top();
        pq.pop();
        if (!vs[get<1>(e)])
        {
            cT += get<0>(e);
            T.push_back(Edge(get<2>(e),get<1>(e),get<0>(e)));
            process(G,S,vs,pq,get<1>(e));
        }
    }
    return cT;
}

void bfs(BFSDist& D, ParentTree& P, Graph& G, Node s, uii n)
{
    D.resize(n,0);
    P.resize(n,s);
    NodeQueue q; q.push(s);
    while( !q.empty() )
    {
        Node c = q.front();
        q.pop();
        for(auto& e : G[c])
        {
            if (D[e.first] || e.first == s) continue;
            D[e.first] = D[c] + 1;
            q.push(e.first);
            P[e.first] = c;
        }
    }
}

void traverse_bfs_tree(NodeList& P, ParentTrees& T, Node s, Node p)
{
    // ova funkcija ne stavlja p u put
    //NodeList P;
    while( s != p)
    {
        P.push_back(s);
        s = T[p][s];
    }
}

uii min_cycle(NodeList& C, Graph& G, uii n)
{
    BFSDists D(n);
    ParentTrees P(n);
    uii lenC = n+1;
    uii sm, tm, um;
    for(Node s = 0; s < n; s++) bfs(D[s],P[s],G,s,n);
    for(Node v = 0; v<n; v++) for(auto& e : G[v])
    {
        Node s = v , t = e.first;
        if (s > t) continue;
        uii lenCUe = uiil::max();
        Node uc;
        for (Node u = 0; u < n; u++)
        {
            if (!D[u][s] || !D[u][t] || P[u][s] == t || P[u][t] == s) continue;
            uii cand = D[u][s] + D[u][t] + 1;
            if (cand < lenCUe){ lenCUe = cand; uc = u; }
        }
        if (lenCUe < lenC){ lenC = lenCUe; sm = s; tm = t; um = uc; }
    }
    if (lenC < n+1)
    {
        C.reserve(lenC);
        traverse_bfs_tree(C,P,sm,um);
        C.push_back(um);
        uii size = C.size();
        traverse_bfs_tree(C,P,tm,um);
        reverse(C.begin() + size, C.end());
        
    }
    return lenC;
    
}
