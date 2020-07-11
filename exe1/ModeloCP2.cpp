#include "ModeloCP2.h"



ModeloCP2::ModeloCP2(string s,int p) : Model(s,p) {

    IloNumArray D = this->dados.calcula_D(this->env);
    IloInt Z_size = D.getSize();

   

    vector<vector<int>> S = this->coleta_Si(D, Z_size);

    //Defini��o das VARIAVEIS de DECISAO=======================================

   //Matriz de variaveis k[j]
    IloNumVarArray Z(this->env, Z_size, 0, 1, ILOINT);


    //Vetor de facilidade y[j], se o centro ser� alocado em j
    IloNumVarArray y(this->env, this->dados.n, 0, 1, ILOINT);

    // Fun��o Objetivo===============================================================


    IloExpr obj(this->env);
    obj += D[0];
    for (int i = 1; i < Z_size; i++) {
        obj += (D[i] - D[i - 1]) * Z[i];
    }

    this->model.add(IloMinimize(this->env, obj));

    // Restri��es===================================================================


    IloExpr sum(this->env); // numero maximo de facilidade
    sum = IloSum(y);
    this->constraints.add(sum <= this->dados.p);
    sum.end();
   
   

    for (IloInt i = 0; i < this->dados.n; i++) {
        for (int k = 0; k < S[i].size(); k++) {
            IloExpr sum(this->env);
          
            sum += Z[S[i][k]];
            for (IloInt j = 0; j < this->dados.n; j++) {
                if (this->dados.D[i][j] < D[S[i][k]]) {
                    sum += y[j];
                }
            }
            this->constraints.add(sum >= 1);
            sum.end();
        }

    }

    for (int i = 0; i < Z_size - 1; i++) {// restri��o mais importante
        this->constraints.add(Z[i] - Z[i+1] >= 0);
     
    }
    this->model.add(this->constraints);

  


   
}


vector<vector<int>> ModeloCP2::coleta_Si(IloNumArray D, int k) {

    vector<vector<int>> S;
    
    for (int i = 0; i < this->dados.n; i++) {
        vector<int> Si;
        for (int t = 0; t < k-1; t++) {
            int contK = 0;
            int contK1 = 0;
            for (int j = 0; j < this->dados.n; j++){
                if (this->dados.D[i][j] == D[t]) contK++;
               
             }
            if (!contK == 0 )Si.push_back(t);
        }
        Si.push_back(k - 1);
       
        S.push_back(Si);
       
        
    }
   


    return S;

}
