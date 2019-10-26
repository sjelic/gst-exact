#include "gst.h"
#include "gurobi_c++.h"
#include "Timer.h"

using namespace std;
//using namespace lemon;

typedef vector<unordered_map<Node,GRBVar>> EdgeGRBVarCont;

template<class T>
class subtourelim: public GRBCallback
{
  public:
    Graph* G;
    GRBVar* yv;
    T& xv;
    EdgeList* el;
    uii n;
    subtourelim(Graph* G, GRBVar* yv, T& xv, EdgeList* el, uii n) : G(G), yv(yv), xv(xv), el(el), n(n) {};
    protected:
    uii checkVar(GRBVar& x)
    {
        return (getSolution(x) >= 0.99 ? 1 : 0); 
    }
    void callback() {
        try {
            if (where == GRB_CB_MIPSOL) {

                Graph CS(n);
                NodeList Cycle;
                for (auto& e : *el)
                {
                    uii s = u(e), t = v(e);
                    if (checkVar(xv[s][t])/*checkVar((xv -> at(s))[t])*/)
                    {
                        CS[s].push_back(make_pair(t,1));
                        CS[t].push_back(make_pair(s,1));
                    }
                }

                uii minC = min_cycle(Cycle, CS, n);
                if (minC > n) return; // nema ciklusa
                GRBLinExpr expr = 0;
                for(Node v = 0; v < minC; v++)
                {
                    uii v1 = Cycle[v], v2 = Cycle[(v+1) % minC];
                    uii s = min(v1,v2), t = max(v1,v2);
                    /*expr += (yv[v1] - (xv -> at(s))[t]);*/
                    expr += (yv[v1] - xv[s][t]);
                }
                for(Node v = 0; v < minC; v++) addLazy(yv[Cycle[v]] <= expr);

            }

        }catch (GRBException e) {
            cout << "Error number: " << e.getErrorCode() << endl;
            cout << e.getMessage() << endl;

        } catch (...) {
            cout << "Error during callback" << endl;
        }
    }
};



void exactILPSubTourElimSolver(st fN, st logF, st solF, uii nThr, sr iTL, sr& timeS, ostream& resOut = cout)
{
    // INICIJALIZACIJA INSTANCE

    Timer initT; initT.start(); initT.stop();

    uii n,m,k;
    Graph G;
    GroupNodeList gv;
    NodeGroupList vg;
    EdgeList el;

    Cost curr_sol = 0;
    EdgeList elcs;
    Solution S;

    initT.start();
    load_instance(fN,G,gv,vg,el,n,m,k);
    shorth_path_heuristic(curr_sol,S,G,gv,vg,elcs,n,m,k);

    GRBVar* y = new GRBVar[n]; // varijable po vrhovima

    GRBVar** x = new GRBVar*[n];
    for(Node v = 0; v < n; v++) x[v] = new GRBVar[n];
    //EdgeGRBVarCont x(n);

    


    


    GRBEnv env = GRBEnv(logF); // gurobi okruženje
    GRBModel model = GRBModel(env); // gurobi model
    model.set(GRB_IntParam_LogToConsole,0);
    model.set(GRB_DoubleParam_TimeLimit, iTL);
    model.set(GRB_IntParam_Threads,nThr);
    model.set(GRB_IntParam_LazyConstraints, 1); // postavljanje lazy uvjeta


    for(Node v = 0; v < n; v++) y[v] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY); // dodavanje varijabli za svaki vrh
    model.update();

    for(auto& e : el) x[u(e)][v(e)] = model.addVar(0.0, 1.0, e.w, GRB_BINARY); // dodavanje varijable za upravo dodani brid
    model.update();

    for (Node v = 0; v<n; v++) y[v].set(GRB_DoubleAttr_Start,S[v]);
    for(auto& e : el) x[u(e)][v(e)].set(GRB_DoubleAttr_Start,0);
    for(auto& e : elcs) x[u(e)][v(e)].set(GRB_DoubleAttr_Start,1);
	model.update();
    // dodavanje funkcije cilja
    GRBLinExpr objf;
    for(auto& e : el) objf += c(e) * x[u(e)][v(e)];
    model.setObjective(objf,GRB_MINIMIZE);
    model.update();

    /* DODAVANJE UVJETA ZA POKRIVANJE GRUPA */

    for (Group g = 0; g < k; g++)
    {
         GRBLinExpr lhsCons = 0;
         for (auto v : gv[g]) lhsCons += y[v];
         model.addConstr(lhsCons,GRB_GREATER_EQUAL,1);
    }
    model.update();

    /* DODAVANJE UVJETA ZA BROJ BRIDOVA/VRHOVA U STABLU*/
    GRBLinExpr lhsCons = 0;
    for(Node v = 0; v < n; v++) lhsCons += y[v];
    for(auto& e : el) lhsCons -= x[u(e)][v(e)];
    model.addConstr(lhsCons,GRB_EQUAL,1);
    model.update();
    

    /* DODAVANJE SUBTOUR ELIMINATION UVJETA ZA SKUPOVE VELIČINE DVA */

    for(auto& e : el)
    {
        model.addConstr(x[u(e)][v(e)],GRB_LESS_EQUAL,y[u(e)]);
        model.addConstr(x[u(e)][v(e)],GRB_LESS_EQUAL,y[v(e)]);
    }    
    model.update();

    /* DODAVANJE UVJETA KOJI ZABRANJUJU IZOLIRANE VRHOVE */

    for (Node v = 0; v < n; v++)
    {
        GRBLinExpr sumEV = 0;
        for (auto& e : G[v]) 
        {
            uii s = min(v,e.first), t = max(v,e.first);
            sumEV += x[s][t];
        }
         model.addConstr(sumEV - y[v] ,GRB_GREATER_EQUAL,0);

    }
    model.update();

    subtourelim<GRBVar**> cb = subtourelim<GRBVar**>(&G, y, x, &el, n);
    

    model.setCallback(&cb);
    initT.stop();


    Timer sT; sT.start(); sT.stop();

    resOut<<fN<<" "<<n<<" "<<m<<" "<<k<<" "<<initT.secs()<<" "<<model.get(GRB_IntAttr_NumVars)<<" "<<model.get(GRB_IntAttr_NumConstrs)<<" ";
    sT.start();
    model.optimize();
    sT.stop();
    if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL) { 
        resOut<<sT.secs()<<" "<<model.get(GRB_DoubleAttr_ObjVal)<<endl;
        timeS+=sT.secs();
        EdgeList optSol; optSol.reserve(n-1);
        for(auto& e : el) if( x[u(e)][v(e)].get(GRB_DoubleAttr_X) >= 0.999 ) optSol.push_back(e);
        save_graph(solF, optSol, n);

    }
    else if (model.get(GRB_IntAttr_Status) == GRB_TIME_LIMIT)
    {
        resOut<<">="<<iTL<<" sec."<<endl;
        timeS+=iTL;
    }
    else if (model.get(GRB_IntAttr_Status) == GRB_INFEASIBLE)
    {
        resOut<<"The instance is infeasible."<<endl;
        timeS+=sT.secs();
    }
    else
    {
        resOut<<"Unspecified error."<<endl;
        timeS+=sT.secs();
    }
    cout<<endl<<"Feasibility: "<<is_feasible(fN,solF)<<endl;
    
    // prepisivanje rješenja
    

    // DEALOKACIJA MEMORIJE
    for(Node v = 0; v < n; v++) delete[] x[v];
    delete[] x;
    delete[] y;
    return;


}

int main(int   argc, ch **argv)
{
    ifstream testInstFile( argc > 1 ? argv[1] : "example.txt");
    uii nThr = stoi(argc > 2 ? argv[2] : "1");
    sr globalTimeLimitSec = stof(argc > 3 ? argv[3] : "86400.00");
    sr iTL = stof(argc > 4 ? argv[4] : "600.00");
    st fN;
    sr timeS = 0;
    while(testInstFile>>fN) 	
    {
        exactILPSubTourElimSolver(fN,"exact_subtour_" + fN.substr(0,fN.size()-3) + "log", 
            fN.substr(0,fN.size()-3) + "sol", nThr, iTL, timeS);
        if (timeS >= globalTimeLimitSec){ cout<<endl<<"GLOBAL TIME-LIMIT OF "<<globalTimeLimitSec<<" sec. EXCEEDED. EXIT."<<endl; break; }
    }

    return 0;
}
