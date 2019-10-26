#include "Types.h"
#include "graph.h"


typedef vb Solution;

inline void update_after_cover(Node v, GroupNodeList& gv, NodeGroupList& vg, vb& cg, vuii& tm)
{
    for (auto g : vg[v])
    {
        cg[g] = true;
        for (auto v : gv[g]) if (tm[v]) tm[v]--;
    }

}

void shorth_path_heuristic(Cost& c, Solution& S, Graph& G, GroupNodeList& gv, NodeGroupList& vg, EdgeList& el, uii n, uii m, uii k)
{
    c = srl::max();
    Dists D;
    ParentTrees P;
    all_pairs_shorthest_paths(D,P,G,n,m);
    uii minG = n+1, im = 0;
    for(uii g = 0; g < k; g++) if (gv[g].size() < minG){im = g; minG = gv[g].size();}
    for (auto v  : gv[im])
    {
        Solution cs(n,false); // trenutno rjesenje
        EdgeList elc; // bridovi trenutnog rjesenja
        vb cg(k,false); // trenutno pokrivene grupe
        uii nC = vg[v].size(); // broj pokrivenih grupa
        vuii tm(n,0); // terminal ako je >0, inace Steinerova tocka
        for (Node t = 0; t<n; t++) tm[t] = vg[t].size();
        cs[v] = true;
        tm[v] = false;
        update_after_cover(v,gv,vg,cg,tm);
        while (nC < k)
        {
            Cost shP = srl::max(); // trazim najkraci put od trenutnog stabla do nekog terminala i dodajem ga
            Node ss = 0; Node tt = 0;
            for (Node s = 0; s < n; s++)
            {
                if (!cs[s]) continue;
                for(Node t = 0; t < n; t++)
                {
                    if (cs[t] || !tm[t]) continue;
                    if (D[s][t] < shP){ shP = D[s][t]; ss = s; tt = t; }
                }
            }


            /*for(Node s = 0, t = 0; s < n && t < n && cs[s] && !cs[t] && tm[t]; s++, t++)
            {
                if (D[s][t] < shP){ shP = D[s][t]; ss = s; tt = t; }
            }*/
            while(tt != ss)
            {
                cs[tt] = true;
                nC += tm[tt];
                update_after_cover(tt,gv,vg,cg,tm);
                tt = P[ss][tt];
            }
        }
        Cost ccS = mst_prim(G,elc,cs,n);
        if (ccS < c){ c = ccS; el = elc; S = cs; }

    }

}

bool is_feasible(st fN, st solF)
{
    uii n,m,k,nT,mT;
    Graph G,T;
    GroupNodeList gv;
    NodeGroupList vg;
    EdgeList el,elT;

    load_instance(fN,G,gv,vg,el,n,m,k);
    ifstream fin(solF);
    if(!fin.is_open()){ cout<<" ... OPENING FILE FAILED ... EXIT."<<endl<<endl; return false;}
    load_graph(fin,T,elT,nT,mT);
    fin.close();

    
    
    mT = elT.size(); nT = 0;
    // Graf je stablo ako i samo ako |E| = |V| - 1 i nema ciklusa
    for(Node v = 0; v < n; v++) if (T[v].size()) nT++;
    if (mT + 1 != nT) return false;
    Node start = u(elT[0]); // prvi vrh na popisu bridova
    NodeQueue q;
    vb gVis(k,false);
    vb vVis(n,false);
    q.push(start);
    vVis[start] = true;
    uii numVis = 1;
    uii numCov = 0;
    //vuii par(n);
    //par[start] = start;

    while(! q.empty())
    {
        Node curr = q.front();
        q.pop();
        for (auto& e : T[curr])
        {
            if (vVis[e.first]) continue;
            q.push(e.first);
            vVis[e.first] = true;
            numVis++;
        }
        for (auto g : vg[curr]) if (!gVis[g]){ gVis[g] = true; numCov++;}
    }
    if (numVis < nT || numCov < k) return false;
    return true;
}