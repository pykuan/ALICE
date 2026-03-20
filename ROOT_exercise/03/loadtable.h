#ifndef LOADTABLE_H
#define LOADTABLE_H

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "TGraphErrors.h"

using namespace std;
inline vector<TGraphErrors*> loadtable(const char* csvfile)
{
    ifstream in(csvfile);
    vector<TGraphErrors*> graphs;
    string line, currentTitle;
    vector<double> vx, vy, ex, ey;

    auto push = [&]() {
        if (vx.empty()) return;
        auto g = new TGraphErrors(vx.size(),
                                  vx.data(), vy.data(),
                                  ex.data(), ey.data());
        g->SetNameTitle(currentTitle.c_str(), currentTitle.c_str());
        graphs.push_back(g);
        vx.clear(); vy.clear(); ex.clear(); ey.clear();
    };

    while (getline(in,line)) {
        if (line.rfind("#:",0)==0) continue;
        if (line.empty())        { push(); continue; }

        if (line.rfind("CENTRALITY",0)==0) {
            stringstream hs(line);
            string dummy;
            getline(hs,dummy,',');
            getline(hs,currentTitle,',');
            continue;
        }

        stringstream ss(line);
        string tok;  double cx,val,err;
        getline(ss,tok,',');  cx   = stod(tok);
        getline(ss,tok,',');  if(tok[0]=='-') continue; else val = stod(tok);
        getline(ss,tok,',');  err  = stod(tok);

        vx.push_back(cx);
        vy.push_back(val);
        ex.push_back(0.0);
        ey.push_back(err);
    }
    push();                       
    return graphs;
}

#endif
