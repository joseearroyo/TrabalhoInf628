#include "Model.h"



Model::Model(string s,int p) {
	
		this->model = IloModel(this->env);
		this->constraints = IloRangeArray(this->env);
		this->dados.Ler(this->env, s,p);
		
}

void Model::resolve() {
	
	this->cplex1=IloCplex(this->env); //Declara-se objeto da classe IloCplex
	this->cplex1.extract(this->model);//Extrai o modelo a ser resolvido. Copia os dados do modelo em
						  //estruturas de dados eficientes a serem utilizados pelo CPLEX.
	this->cplex1.setOut(env.getNullStream());
	this->cplex1.solve(); //resolve o modelo
}


double Model::imprime_solucao() {

	//cout << "REsultado: " << this->cplex1.getObjValue() << endl;
	return this->cplex1.getObjValue();


}