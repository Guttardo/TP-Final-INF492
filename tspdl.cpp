#ifndef CST_TSPDL
#define CST_TSPDL

#define forStep(a,b,c,d) for(int a = b; a != c; a += d)
#define f(a,b,c) forStep(a,b,c,1)

#define PORT_LIMIT 100

#include<bits/stdc++.h>

using namespace std;

class tspdl {
    public:
    int n, nodes[100], dist[100][100], demand[100],draft[100], totalDemand = 0;
    double posX[100], posY[100];

    tspdl(string nomeInst){
        string file = "tspdl-test-data-equal-demand/gr48_" + nomeInst + ".dat";

        string texto;
        char c, cStrAux[1000];

        ifstream data(file.c_str());

        do{
            c = data.peek();

            if(c == '!'){
                data.getline(cStrAux, 1000);
            } else {
                data.getline(cStrAux, 1000, ':');
                texto = cStrAux;

                if( texto == "N"){
                    data >> n;
                } else if(texto == "Nodes"){
                    data >> cStrAux;
                    for(int i = 0; i < n; i++){
                        data >> nodes[i];
                    }
                    data >> cStrAux;
                } else if(texto == "Distance"){
                    data >> cStrAux;
                    for(int i = 0; i < n; i++){
                        for(int j = 0; j < n; j++){
                            data >> dist[i][j];
                        }
                    }
                    data >> cStrAux;
                } else if(texto == "PosX"){
                    data >> cStrAux;
                    for(int i = 0; i < n; i++){
                        data >> posX[i];
                    }
                    data >> cStrAux;
                } else if(texto == "PosY"){
                    data >> cStrAux;
                    for(int i = 0; i < n; i++){
                        data >> posY[i];
                    }
                    data >> cStrAux;
                } else if(texto == "Demand"){
                    data >> cStrAux;
                    for(int i = 0; i < n; i++){
                        data >> demand[i];
                    }
                    data >> cStrAux;
                } else if(texto == "Draft"){
                    data >> cStrAux;
                    for(int i = 0; i < n; i++){
                        data >> draft[i];
                    }
                    data >> cStrAux;
                }

                data.getline(cStrAux, 1000);
            }
        }while(data.gcount());

        data.close();

        f(i,0,n){
            totalDemand += demand[i];
        }
    }

    int solve(const vector<int>& rota) const{
        int posicao = 0, distancia = 0, peso = totalDemand;

        f(i,0,rota.size()){

            if(draft[rota[i]] < peso) return -1;
            distancia += dist[posicao][rota[i]];

            peso -= demand[rota[i]];
            posicao = rota[i];

        }

        distancia += dist[posicao][0];
        return distancia;
    }

    vector<int> generateRandomGreed(){
        vector<int> solucao;
        bool inserido[100] = {1,0};
        int peso = totalDemand;
        int posicao = 0, esc, esc2;
        bool tem;

        f(i,0,n-1){
            int auxMin = 999999, ichos = -1;
            esc = rand() % 2;
            if(esc==0){
                f(j,0,n){ 
                    if(!inserido[j] && draft[j] >= peso && dist[posicao][j] < auxMin){
                        auxMin = dist[posicao][j];
                        ichos = j;
                    }
                }
            }
            else{
                tem = false;
                f(j,0,n){ 
                    if(!inserido[j] && draft[j] >= peso){
                        if(!tem){
                            ichos = j;
                            tem = true;
                        }
                        else{
                            esc2 = rand() % 2;
                            if(esc2==1) ichos = j;
                        }
                    }
                }
            }
            if(ichos < 0){
                cerr << "Erro ao gerar solu��o inicial\n";
                throw exception();
            }
            solucao.push_back(ichos);
            inserido[ichos] = true;
            peso -= demand[ichos];
            posicao = solucao[i];
        }

        return solucao;
    }

    vector<int> generateGreed(){
        vector<int> solucao;
        bool inserido[100] = {1,0};
        int peso = totalDemand;
        int posicao = 0;

        f(i,0,n-1){
            int auxMin = 999999, ichos = -1;
            f(j,0,n){
                if(!inserido[j] && draft[j] >= peso && dist[posicao][j] < auxMin){
                    auxMin = dist[posicao][j];
                    ichos = j;
                }
            }
            if(ichos < 0){
                cerr << "Erro ao gerar solu��o inicial\n";
                throw exception();
            }
            solucao.push_back(ichos);
            inserido[ichos] = true;
            peso -= demand[ichos];
            posicao = solucao[i];
        }

        return solucao;
    }

    vector<int> busca(vector<int> solucao){
        vector<int> melhor(solucao);
        int melhorval;
        bool flag = true;
        cout << "begin search" << endl;
        while(flag){
            flag = false;
            solucao = melhor;
            melhorval = solve(solucao);
            f(i,0,solucao.size()){
                f(j,i+1,solucao.size()){
                    vector<int> candidato(solucao);
                    int aux = candidato[i];
                    candidato[i] = candidato[j];
                    candidato[j] = aux;

                    aux = solve(candidato);
                    if(aux < melhorval){
                        flag = true;
                        melhorval = aux;
                        melhor = candidato;
                    }
                }
            }
        }

        return solucao;
    }
};


#endif // CST_TSPDL
