#include<bits/stdc++.h>

using namespace std;

#include "tspdl.cpp"

#define PORT_LIMIT 100

using namespace std;

int main(){
    vector<int> solucao;
    string inst;

    while(cin >> inst){
        tspdl instance(inst);

        solucao = instance.generateGreed();

        for(int i = 0; i < solucao.size(); i++){
            cout << solucao[i] << " ";
        }
        cout << "\nResultado\n" << instance.solve(solucao) << endl;
    }

    return 0;
}
