#include<bits/stdc++.h>
#include "tspdl.cpp"

#define POP_SIZE 300
#define ITR 50

typedef struct{
    vector<int> codigo;
    int f_objetivo, selection;
}individuo;

vector<individuo> gera_populacao(tspdl instance){
    bool escolhido[instance.n+1] = {0};
    int sorteado, cont=0, cont_deucerto=0, cont_resta;
    vector<individuo> populacao;
    int n = instance.n, peso_total = instance.totalDemand, peso;
    individuo guloso;
    guloso.codigo = instance.generateGreed();
    guloso.f_objetivo = instance.solve(guloso.codigo);
    populacao.push_back(guloso);
    cont_deucerto++;
    for(int i=0; i<POP_SIZE/4; i++){
        guloso.codigo = instance.generateRandomGreed();
        guloso.f_objetivo = instance.solve(guloso.codigo);
        populacao.push_back(guloso);
        cont_deucerto++;
    } 
    while(cont_deucerto<POP_SIZE){
        vector<int> solucao;
        peso = peso_total;
        while(cont<n-1){
            sorteado = rand() % (n-1)+1;
            if(!escolhido[sorteado]){
                cont_resta = n-1;
                while(cont_resta>0){
                    if(!escolhido[sorteado] && instance.draft[sorteado]>=peso){
                        cont++;;
                        solucao.push_back(sorteado);
                        escolhido[sorteado] = true;
                        peso -= instance.demand[sorteado];
                        break;
                    }
                    cont_resta--;
                    sorteado = rand() % (n-1)+1;
                }
                if(cont_resta==0) goto x;
            }
        }
        if(instance.solve(solucao)!=-1){
            individuo novo;
            novo.codigo = solucao;
            novo.f_objetivo = instance.solve(solucao);
            populacao.push_back(novo);
            cont_deucerto++;
        }
        x:
        solucao.clear();
        for(int i=0; i<POP_SIZE; i++) escolhido[i]=false;
        cont = 0;
    }
    
    return populacao;
}

individuo gera_individuo(tspdl instance){
    int peso, peso_total = instance.totalDemand, cont=0, cont_resta, sorteado, n = instance.n;
    bool escolhido[instance.n+1] = {0};
    while(true){
        vector<int> solucao;
        peso = peso_total;
        while(cont<n-1){
            sorteado = rand() % (n-1)+1;
            if(!escolhido[sorteado]){
                cont_resta = n-1;
                while(cont_resta>0){
                    if(!escolhido[sorteado] && instance.draft[sorteado]>=peso){
                        cont++;;
                        solucao.push_back(sorteado);
                        escolhido[sorteado] = true;
                        peso -= instance.demand[sorteado];
                        break;
                    }
                    cont_resta--;
                    sorteado = rand() % (n-1)+1;
                }
                if(cont_resta==0) goto x;
            }
        }
        if(instance.solve(solucao)!=-1){
            individuo novo;
            novo.codigo = solucao;
            novo.f_objetivo = instance.solve(solucao);
            return novo;
        }
        x:
        solucao.clear();
        for(int i=0; i<POP_SIZE; i++) escolhido[i]=false;
        cont = 0;
    }
}

vector<individuo> evaluate(vector<individuo> p, tspdl instance){
    
    double f_total=0, selection=0.0;

    for(int i=0; i<p.size(); i++){
        p[i].f_objetivo = instance.solve(p[i].codigo);
    }

    for(int i=0; i<p.size(); i++)
        f_total+= 10000.0/p[i].f_objetivo;
    
    //cout << f_total << endl;
    
    for(int i=0; i<p.size(); i++){
        p[i].selection = selection+floor(((10000.0/p[i].f_objetivo)/f_total)*100000);
        selection+=floor(((10000.0/p[i].f_objetivo)/f_total)*100000);    
    }
    p[p.size()-1].selection = 1000000;
    return p;
}

individuo crossover_erx(individuo p1, individuo p2, tspdl instance){

    int tam = p1.codigo.size();
    set<int> vizinhos[tam+1];

    for(int i=0; i<tam; i++){
        //cout << i << " - " << p1.codigo[i] << " - " << p2.codigo[i] << endl;
        if(i!=0 && i!=tam-1){
            vizinhos[p1.codigo[i]].insert(p1.codigo[i-1]);
            vizinhos[p1.codigo[i]].insert(p1.codigo[i+1]);
            vizinhos[p2.codigo[i]].insert(p2.codigo[i-1]);
            vizinhos[p2.codigo[i]].insert(p2.codigo[i+1]);
        }
        else if(i==0){
            vizinhos[p1.codigo[i]].insert(p1.codigo[tam-1]);
            vizinhos[p1.codigo[i]].insert(p1.codigo[i+1]);
            vizinhos[p2.codigo[i]].insert(p2.codigo[tam-1]);
            vizinhos[p2.codigo[i]].insert(p2.codigo[i+1]);
        }
        else{
            vizinhos[p1.codigo[i]].insert(p1.codigo[i-1]);
            vizinhos[p1.codigo[i]].insert(p1.codigo[0]);
            vizinhos[p2.codigo[i]].insert(p2.codigo[i-1]);
            vizinhos[p2.codigo[i]].insert(p2.codigo[0]);
        }
    }

    individuo child;
    int peso = instance.totalDemand;
    int esc = rand() % 2, x, z, menor;
    bool jatem;
    if(esc==0) x = p1.codigo[0];
    else x = p2.codigo[0];
    bool escolhido[tam+1] = {0};
    while(child.codigo.size()!=p1.codigo.size()-1){
        child.codigo.push_back(x);
        peso-=instance.demand[x];
        escolhido[x] = true;
        for(int i=1; i<=tam; i++)
            vizinhos[i].erase(x);
        
        if(vizinhos[x].size()==0){
            z=0;
            jatem = false;
            for(int i=1; i<=tam; i++){
                if(!escolhido[i] && instance.draft[i]>=peso){
                    if(!jatem){
                        z = i;
                        jatem = true;
                    }
                    else{
                        esc = rand() % 2;
                        if(esc==1) z = i;
                    }
                }
            }
            if(z==0){
                child.f_objetivo=-2;
                return child;
            }
        }
        else{
            menor = 10000;
            for(set<int>::iterator it=vizinhos[x].begin(); it!=vizinhos[x].end(); ++it){
                if(vizinhos[*it].size()<menor && instance.draft[*it]>=peso){
                    menor = vizinhos[*it].size();
                    z = *it;
                }
                else if(vizinhos[*it].size()==menor && instance.draft[*it]>=peso){
                    esc = rand() % 2;
                    if(esc==1) z = *it;
                }
            }
            if(menor==10000){
                z=0;
                jatem = false;
                for(int i=1; i<=tam; i++){
                    if(!escolhido[i] && instance.draft[i]>=peso){
                        if(!jatem){
                            z = i;
                            jatem = true;
                        }
                        else{
                            esc = rand() % 2;
                            if(esc==1) z = i;
                        }
                    }
                }
                if(z==0){
                    child.f_objetivo=-2;
                    return child;
                }
            }
        }

        x = z;
    }
    child.codigo.push_back(x);
    
    return child;
}

bool ordena(individuo a, individuo b){return a.f_objetivo<b.f_objetivo;}

double f_soma(vector<individuo> p){
    double f_total=0;
    for(int i=0; i<p.size(); i++)
        f_total+= 10000.0/p[i].f_objetivo;
    
   return f_total;
}

individuo ga(tspdl instance){

    int roulette, cut, n=instance.n, esc;

    vector<individuo> p = gera_populacao(instance);
    vector<individuo> melhorp = p;
    melhorp = evaluate(melhorp, instance);
    for(int t=0; t<ITR; t++){
        //cout << t << endl;
        p = evaluate(p, instance);
        if(f_soma(p)>f_soma(melhorp)){
            melhorp = p;
        }
        individuo p1, p2;
        vector<individuo> plinha, pnova;
        for(int i=0; i<POP_SIZE; i++){
            roulette = rand() % 100000;
            for(int j=0; j<POP_SIZE; j++)
                if(p[j].selection>=roulette){
                    p1 = p[j];
                    break;
                }
            roulette = rand() % 100000;
            for(int j=0; j<POP_SIZE; j++)
                if(p[j].selection>=roulette){
                    p2 = p[j];
                    break;
                }
            
            individuo child = crossover_erx(p1,p2, instance);
            if(child.f_objetivo==-2) i--;
            else plinha.push_back(child);
        }
        vector<int> mutacao;
        for(int i=0; i<POP_SIZE; i++){
            esc = rand() % 100;
            if(esc<(100/n)){
                for(int j=0; j<plinha[i].codigo.size(); j++){
                    mutacao.clear();
                    for(int k=j+1; k<plinha[i].codigo.size(); k++){
                        for(int l=0; l<j; l++)
                            mutacao.push_back(plinha[i].codigo[l]);
                        for(int l=k; l>=j; l--)
                            mutacao.push_back(plinha[i].codigo[l]);
                        for(int l=k+1; l<plinha[i].codigo.size(); l++)
                            mutacao.push_back(plinha[i].codigo[l]);
                        if(instance.solve(mutacao)>=instance.solve(plinha[i].codigo)){
                            plinha[i].codigo = mutacao;
                            goto y;
                        }
                    }
                }
            }
            y:;
        }
        plinha = evaluate(plinha,instance);
        sort(p.begin(),p.end(),ordena);
        sort(plinha.begin(),plinha.end(),ordena);
        for(int i=0; i<POP_SIZE-POP_SIZE/10; i++)
            pnova.push_back(p[i]);
        for(int i=0; i<POP_SIZE/10; i++)
            pnova.push_back(plinha[i]);
        if(pnova.size()<POP_SIZE)
            pnova.push_back(plinha[POP_SIZE/4]);    

        p = pnova;
    }
    p = evaluate(p, instance);
    if(f_soma(p)>f_soma(melhorp))
        melhorp = p;
    individuo melhor;
    int menor = 10000000;
    for(int i=0; i<melhorp.size(); i++){
        if(melhorp[i].f_objetivo<menor){
            melhor = melhorp[i];
            menor = melhorp[i].f_objetivo;
        }
    }
    return melhor;
}

int main(){
    srand (time(NULL));
    string nomeInst, prob;
    individuo melhor;
    for(char i='1';i<='9';i++){
        prob = i;
        nomeInst = "10_"+prob;
        tspdl instance1(nomeInst);
        
        melhor = ga(instance1);
        cout << "gr48_10_"<< i << " : " << melhor.f_objetivo << endl;
    }
    nomeInst = "10_10";
    tspdl instance1(nomeInst);
        
    melhor = ga(instance1);
    cout << "gr48_10_10" << " : " << melhor.f_objetivo << endl;
    
    for(char i='1';i<='9';i++){
        prob=i;
        nomeInst = "25_"+prob;
        tspdl instance2(nomeInst);
        
        melhor = ga(instance2);
        cout << "gr48_25_"<< i << " : " << melhor.f_objetivo << endl;
    }
    nomeInst = "25_10";
    tspdl instance2(nomeInst);
        
    melhor = ga(instance2);
    cout << "gr48_25_10" << " : " << melhor.f_objetivo << endl;
    
    for(char i='1';i<='9';i++){
        prob=i;
        nomeInst = "50_"+prob;
        tspdl instance3(nomeInst);
        
        melhor = ga(instance3);
        cout << "gr48_50_"<< i << " : " << melhor.f_objetivo << endl;
    }
    nomeInst = "50_10";
    tspdl instance3(nomeInst);
        
    melhor = ga(instance3);
    cout << "gr48_50_10" << " : " << melhor.f_objetivo << endl;
    return 0;
}