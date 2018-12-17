#include<bits/stdc++.h>

using namespace std;

#include "tspdl.cpp"


using namespace std;

int main(){
    vector<int> solucao;
    string inst;

    while(cin >> inst){
        tspdl instance(inst);

        solucao = instance.busca(instance.generateGreed());

        for(int i = 0; i < solucao.size(); i++){
            cout << solucao[i] << " ";
        }
        cout << "\nResultado\n" << instance.solve(solucao) << endl;
    }

    return 0;
}
