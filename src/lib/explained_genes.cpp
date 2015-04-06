/*
 * explained_genes.cpp
 *
 *  Created on: 27 Mar, 2015
 *      Author: Nok
 */

#include <cmath>
#include <iostream>
#include <queue>
#include <set>
#include "../header/explained_genes.h"

void getGeneExpressionFromSampleId(TDoubleMatrix* geneExpressionMatrix,
		vector<int>* genesEx, vector<double>* sampleExpression, int sampleId) {
	int totalGenes = geneExpressionMatrix->size();
	for (int i = 0; i < totalGenes; ++i) {
		sampleExpression->at(genesEx->at(i)) =
				geneExpressionMatrix->at(i)[sampleId];
	}
}

void getMutatedGeneIdsFromSampleId(Mutations* mutations,
		vector<int>* mutatedGeneIds, int sampleId, vector<int>* genesMut) {
	// sampleId is column id
	int numGenes = genesMut->size();
	for (int i = 0; i < numGenes; ++i) {
		//if a current gene is mutated
		if (mutations->matrix->at(i)[sampleId] != 0) {
			mutatedGeneIds->push_back(genesMut->at(i));
		}
	}
}

void getExplainedGenes(vector<ExplainedGene>* explainedGenes,
		TIntAdjList* network, vector<double>* sampleGeneExpression,
		vector<int>* mutatedGeneIds, int L, int D, double F) {

	int numMutatedGenes = mutatedGeneIds->size();
	for (int i = 0; i < numMutatedGenes; ++i) {	// for each mutated genes
		//BFS
		//cout << "For mutated gene: " << mutatedGeneIds->at(i) << endl;
		BFSforExplainedGenes(network, mutatedGeneIds->at(i), L, D, F,
				explainedGenes, sampleGeneExpression);
	}
}

void getExplainedGenesIdOnly(vector<int>* explainedGenesFrequency, TIntAdjList* network, vector<double>* sampleGeneExpression,
		vector<int>* mutatedGeneIds, int L, int D, double F){
	int numMutatedGenes = mutatedGeneIds->size();
	for (int i = 0; i < numMutatedGenes; ++i) {	// for each mutated genes
		//BFS
		//cout << "For mutated gene: " << mutatedGeneIds->at(i) << endl;
		BFSforExplainedGenesIdOnly(network, mutatedGeneIds->at(i), L, D, F,
				explainedGenesFrequency, sampleGeneExpression);
	}
}

//output: explained genes frequency
//up regulated gene frequency is in the first half [0,totalGenes - 1]
//down regulated gene frequency is in the second half [totalGenes, totalGenes * 2 - 1]
void getExplainedGenesIdOnlyUpDown(vector<int>* explainedGenesFrequency, TIntAdjList* network, vector<double>* sampleGeneExpression,
		vector<int>* mutatedGeneIds, int L, int D, double F){
	int numMutatedGenes = mutatedGeneIds->size();
	for (int i = 0; i < numMutatedGenes; ++i) {	// for each mutated genes
		//BFS
		//cout << "For mutated gene: " << mutatedGeneIds->at(i) << endl;
		BFSforExplainedGenesIdOnlyUpDown(network, mutatedGeneIds->at(i), L, D, F,
				explainedGenesFrequency, sampleGeneExpression);
	}
}

void getMutatedAndExplainedGenes(vector<MutatedAndExplianedGenes>* mutatedAndExplainedGenes, TIntAdjList* network,
		vector<double>* sampleGeneExpression, vector<int>* mutatedGeneIds, int L, int D, double F){
	int numMutatedGenes = mutatedGeneIds->size();
	int totalGenes = network->size();
	for (int i = 0; i < numMutatedGenes; ++i) {	// for each mutated genes
		int mutatedGeneId = mutatedGeneIds->at(i);
		MutatedAndExplianedGenes* meg = &mutatedAndExplainedGenes->at(mutatedGeneId);
		meg->explainedGenesFreqency = vector<int>(totalGenes);
		//BFS for explained genes of the current mutated gene
		BFSforExplainedGenesIdOnly(network, mutatedGeneId, L, D, F,
				&(meg->explainedGenesFreqency), sampleGeneExpression);
	}

}


void BFSforExplainedGenes(TIntAdjList* network, int geneId, int L, int D,
		double F, vector<ExplainedGene>* explainedGenes, vector<double>* sampleGeneExpression) {
	// Mark all the vertices as not visited
	int numNode = network->size();
	bool *visited = new bool[numNode];
	for (int j = 0; j < numNode; j++) {
		visited[j] = false;
	}

	// Create queue
	queue<int> q;
	vector<int> levels(numNode); // store level of each nodes
	// Mark the current node as visited
	q.push(geneId);
	visited[geneId] = true;
	int currentGene;
	int currentLevel = 0;

	//consider all nodes that are in <= L distant
	while (!q.empty() && currentLevel <= L) {
		//read the root node
		currentGene = q.front();
		q.pop();
		currentLevel = levels[currentGene];

		if (currentLevel > L) {
			break;
		}

		//cout << currentLevel << " : " << currentGene << "(" <<
		//		sampleGeneExpression->at(currentGene) << ")(" <<
		//		getNodeDegree(network, currentGene) << ")" << endl;

		//explore all the connected nodes
		vector<int> adj = (*network)[currentGene];

		int numAdj = adj.size();
		for (int j = 0; j < numAdj; j++) {
			if (!visited[(*network)[currentGene][j]]) {
				// check conditions before push to the queue
				// |fold change| > F
				int geneId = (*network)[currentGene][j];
				if (fabs(sampleGeneExpression->at(geneId)) > F) {
					// is explained gene
					explainedGenes->at(geneId).expression = sampleGeneExpression->at(geneId);
					explainedGenes->at(geneId).frequency++;
					int degree = getNodeDegree(network, geneId);
					explainedGenes->at(geneId).degree = degree;
					// check the degree
					if (degree <= D) {
						// push to queue
						q.push(geneId);
						// save level
						levels[geneId] = currentLevel + 1;
					}
				}
				// mark visited
				visited[geneId] = true;
			}
		}
	}

	delete[] visited;
}

void BFSforExplainedGenesIdOnly(TIntAdjList* network, int geneId, int L, int D,
		double F, vector<int>* explainedGenesFrequency, vector<double>* sampleGeneExpression) {
	// Mark all the vertices as not visited
	int numNode = network->size();
	bool *visited = new bool[numNode];
	for (int j = 0; j < numNode; j++) {
		visited[j] = false;
	}

	// Create queue
	queue<int> q;
	vector<int> levels(numNode); // store level of each nodes
	// Mark the current node as visited
	q.push(geneId);
	visited[geneId] = true;
	int currentGene;
	int currentLevel = 0;

	//consider all nodes that are in <= L distant
	while (!q.empty() && currentLevel <= L) {
		//read the root node
		currentGene = q.front();
		q.pop();
		currentLevel = levels[currentGene];

		if (currentLevel > L) {
			break;
		}

		//cout << currentLevel << " : " << currentGene << "(" <<
		//		sampleGeneExpression->at(currentGene) << ")(" <<
		//		getNodeDegree(network, currentGene) << ")" << endl;

		//explore all the connected nodes
		vector<int> adj = (*network)[currentGene];

		int numAdj = adj.size();
		for (int j = 0; j < numAdj; j++) {
			if (!visited[(*network)[currentGene][j]]) {
				// check conditions before push to the queue
				// |fold change| > F
				int geneId = (*network)[currentGene][j];
				if (fabs(sampleGeneExpression->at(geneId)) > F) {
					// is explained gene
					explainedGenesFrequency->at(geneId) += 1;
//					cout << "gene " << geneId << " is explained" << endl;
					int degree = getNodeDegree(network, geneId);
					// check the degree
					if (degree <= D) {
						// push to queue
						q.push(geneId);
						// save level
						levels[geneId] = currentLevel + 1;
					}
				}
				// mark visited
				visited[geneId] = true;
			}
		}
	}


	delete[] visited;
}

//up regulated gene frequency is in the first half [0,totalGenes - 1]
//down regulated gene frequency is in the second half [totalGenes, totalGenes * 2 - 1]
void BFSforExplainedGenesIdOnlyUpDown(TIntAdjList* network, int geneId, int L, int D,
		double F, vector<int>* explainedGenesFrequency, vector<double>* sampleGeneExpression) {
	int totalGenes = explainedGenesFrequency->size() / 2;

	// Mark all the vertices as not visited
	int numNode = network->size();
	bool *visited = new bool[numNode];
	for (int j = 0; j < numNode; j++) {
		visited[j] = false;
	}

	// Create queue
	queue<int> q;
	vector<int> levels(numNode); // store level of each nodes
	// Mark the current node as visited
	q.push(geneId);
	visited[geneId] = true;
	int currentGene;
	int currentLevel = 0;

	//consider all nodes that are in <= L distant
	while (!q.empty() && currentLevel <= L) {
		//read the root node
		currentGene = q.front();
		q.pop();
		currentLevel = levels[currentGene];

		if (currentLevel > L) {
			break;
		}

		//explore all the connected nodes
		vector<int> adj = (*network)[currentGene];

		int numAdj = adj.size();
		for (int j = 0; j < numAdj; j++) {
			if (!visited[(*network)[currentGene][j]]) {
				// check conditions before push to the queue
				// |fold change| > F
				int geneId = (*network)[currentGene][j];
				if (fabs(sampleGeneExpression->at(geneId)) > F) {
					// is explained gene
					if(sampleGeneExpression->at(geneId) > 0){ 	// up regulated
						explainedGenesFrequency->at(geneId) += 1;
					}else{	// down regulated
						explainedGenesFrequency->at(geneId + totalGenes) += 1;
					}
					int degree = getNodeDegree(network, geneId);
					// check the degree
					if (degree <= D) {
						// push to queue
						q.push(geneId);
						// save level
						levels[geneId] = currentLevel + 1;
					}
				}
				// mark visited
				visited[geneId] = true;
			}
		}
	}


	delete[] visited;
}

//TODO use only one BFS for all L's values