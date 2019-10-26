#include "Types.h"
#include "graph.h"
#include "Timer.h"

using namespace std;

int main(int   argc, ch **argv)
{
    uii n,m;
    Graph G;
    EdgeList el;
    NodeList cycle;
    ifstream fin(argv[1]);
    load_graph(fin, G, el, n, m);
    uii minCL = min_cycle(cycle,G,n);
    if (minCL == n+1) cout<<"NEMA CIKLUSA"<<endl;
    else
    {
        cout<<minCL<<endl;
        for (auto& n : cycle) cout<<n<<" ";
        cout<<endl;

    }
    
    
    return 0;
}
