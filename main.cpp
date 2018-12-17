#include<bits/stdc++.h>

#define ANT_COUNT 200
#define PORT_LIMIT 100
#define PHEROMONE_EVAPORATION 0.90
#define PHEROMONE_INCREMENT 50.0
#define ELITE_THRESHOLD 0.15
#define GENERATIONS_NUMBER 500
#define PHEROMONE_START 1000

#define forStep(a,b,c,d) for(int a = b; a != c; a += d)
#define f(a,b,c) forStep(a,b,c,1)

using namespace std;

default_random_engine re;

struct pivi {
    int first;
    vector<int> second;

    pivi(int a, vector<int> b){
        first = a;
        second = b;
    }
    bool operator<(const pivi& a) const{
        return (first == a.first? second < a.second:first < a.first);
    }
};

double pheromone[100][100] = {{0}};

vector<int> generateAnt(){
    vector<int> solucao;
    bool inserido[100] = {1,0};
    int peso = totalDemand;
    int posicao = 0;
    f(i,0,n-1){

        vector<int> bifurcacao;
        double bif = 0;
        f(j,0,n){
            if(!inserido[j] && draft[j] >= peso){
                bifurcacao.push_back(j);
                bif += pheromone[posicao][j];
            }
        }
        if(bifurcacao.size() == 0){
            cerr << "Erro ao gerar solução inicial\n";
            throw exception();
        }
        int ichos = -1;
        double decide = fRand(0,bif);

        while(decide > 0){
            ichos++;
            decide -= pheromone[posicao][bifurcacao[ichos]];
        }
        if(ichos<0) ichos = 0;

        ichos = bifurcacao[ichos];

        solucao.push_back(ichos);
        inserido[ichos] = true;
        peso -= demand[ichos];
        posicao = solucao[i];

    }

    return solucao;
}

void evaporatePheromone(){
    f(i,0,n){
        f(j,0,n){
            pheromone[i][j] *= PHEROMONE_EVAPORATION;
        }
    }
}

void updatePheromone(vector<int> route, int place){
    int posicao = 0;

    f(i,0,route.size()){
        pheromone[posicao][route[i]] += PHEROMONE_INCREMENT;

        posicao = route[i];
    }

}

int main(){
    srand (time(NULL));

    cout << "Guloso: \n";
    {
        vector<int> solucao = generateGreed();

        for(int i = 0; i < solucao.size(); i++){
            cout << solucao[i] << " ";
        }
        cout << "\nResultado\n" << solve(solucao) << endl;
    }

    cout << "Ant Colony: \n";
    {

        int lifetimeBest = 999999;
        vector<int> solucao;

        f(i,0,n){
            f(j,0,n){
                pheromone[i][j] = PHEROMONE_START;
            }
        }
        f(gen,0,GENERATIONS_NUMBER){
            //vector<int> ants[ANT_COUNT];
            set<pivi > ranks;
            f(i,0,ANT_COUNT){
                try{
                    vector<int> ant = generateAnt();
                     ranks.insert(pivi(solve(ant), ant));
                } catch(exception e){
                    cerr << "Erro ao gerar formiga\n";
                    i--;
                }
            }

            evaporatePheromone();
            set<pivi >::iterator it = ranks.begin();

            //Insere no log o melhor da geração
            log << "Gen " << gen << "\t" << it->first;
            for(int i = 0; i < it->second.size(); i++){
                log << "\t" << it->second[i];
            }
            log << "\n";

            if(it->first < lifetimeBest){
                lifetimeBest = it->first;
                solucao = it->second;
            }
            f(i,0,ceil(ANT_COUNT*ELITE_THRESHOLD)){
                updatePheromone(it->second,i+1);
                it++;
            }
        }

        for(int i = 0; i < solucao.size(); i++){
            cout << solucao[i] << " ";
        }
        cout << "\nResultado\n" << solve(solucao) << endl;
    }
    log.close();

    return 0;
}
