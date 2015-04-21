/*
 * intput.cpp
 *
 *  Created on: 27 Mar, 2015
 *      Author: Nok
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <cmath>
#include <map>
#include <cstdlib>
#include "../header/input.h"

void readGeneExpression(const char* filename, GeneExpression* geneExpression,
		char delim, map<string, int>* geneSymbolToId, vector<string>* sampleIdToName){
	ifstream inFile;
	inFile.open(filename, std::ifstream::in);
	vector<int>* geneIds = geneExpression->genes;

	map<string, int>::iterator end = geneSymbolToId->end(); // for a gene not found in the network

	if (inFile.is_open()) {

		//read the first line (sample name)
		string samplesList;
		getline(inFile, samplesList);
		istringstream ss(samplesList);
		string s;
		getline(ss, s, delim);	//discard the first column (GENE header)
		while (ss) {	//for each column (sample)
			if (!getline(ss, s, delim))
				break;
			sampleIdToName->push_back(s);
		}

		while (inFile.good()) { //for each row (gene)
			string s;
			if (!getline(inFile, s))
				break;

			istringstream ss(s);
			vector<double> row;	//to save expression values of each gene

			int i = 0;	// for read gene symbol in the first column
			bool found = false;	// is a current gene found in the network

			while (ss) {	//for each column (sample)
				string s;

				if (!getline(ss, s, delim))
					break;
				if(i == 0){ //read gene symbols
					map<string, int>::iterator it = geneSymbolToId->find(s);
					if(it != end){	//found in the network
						found = true;
						geneIds->push_back(it->second);	//add the gene id to geneEx
					}else{
						found = false;
					}
				}else{		//read expression values
					row.push_back(atof(s.c_str()));
				}

				if(!found)	//not found in the network, so skip this row (gene)
					break;

				i++;	//go to the next column (sample)
			}

			if(found){
				geneExpression->matrix->push_back(row);
			}
		}
		inFile.close();
	} else {
		cerr << "Error opening file\n";
	}
}

void readMutations(const char* filename, Mutations* mutations,
		char delim, map<string, int>* geneSymbolToId){
	ifstream inFile;
	inFile.open(filename, std::ifstream::in);
	vector<int>* geneIds = mutations->genes;

	map<string, int>::iterator end = geneSymbolToId->end(); // gene not found in the network

	if (inFile.is_open()) {

		//read the first line (sample name)
		string samples;
		getline(inFile, samples);

		//read the output from the
		while (inFile.good()) {
			string s;
			if (!getline(inFile, s))
				break;

			istringstream ss(s);
			vector<int> row;	//to save mutation values of each gene

			int i = 0;	// for read gene in the first column;
			bool found = false;	// found in the network

			while (ss) {
				string s;
				if (!getline(ss, s, delim))
					break;
				if(i == 0){ //read gene symbols
					map<string, int>::iterator it = geneSymbolToId->find(s);
					if(it != end){	//found in the network
						found = true;
						geneIds->push_back(it->second);
					}else{
						found = false;
					}
				}else{		//read expression values
					row.push_back(atoi(s.c_str()));
				}
				if(!found)
					break;
				i++;
			}
			if(found){
				mutations->matrix->push_back(row);
			}
		}
		inFile.close();
	} else {
		cerr << "Error opening file\n";
	}
}

void readPointMutations(const char* filename, PointMutations* pointMutations,
		char delim, map<string, int>* geneSymbolToId){
	ifstream inFile;
	inFile.open(filename, std::ifstream::in);
	vector<int>* geneIds = pointMutations->genes;

	map<string, int>::iterator end = geneSymbolToId->end(); // gene not found in the network

	if (inFile.is_open()) {

		//read the first line (sample name)
		string samples;
		getline(inFile, samples);

		//read the output from the
		while (inFile.good()) {
			string s;
			if (!getline(inFile, s))
				break;

			istringstream ss(s);
			vector<int> row;

			int i = 0;	// for read gene in the first column;
			bool found = false;	// found in the network

			while (ss) {
				string s;
				if (!getline(ss, s, delim))
					break;
				if(i == 0){ //read gene symbols
					map<string, int>::iterator it = geneSymbolToId->find(s);
					if(it != end){	//found in the network
						found = true;
						geneIds->push_back(it->second);
					}else{
						found = false;
					}
				}else{		//read mutation values
					row.push_back(atoi(s.c_str()));
				}

				if(!found)
					break;

				i++;
			}

			if(found){
				pointMutations->matrix->push_back(row);
			}

		}
		inFile.close();
	} else {
		cerr << "Error opening file\n";
	}
}

void readCopyNumberVariation(const char* filename, CopyNumberVariation* copyNumberVariation,
		char delim, map<string, int>* geneSymbolToId){
	ifstream inFile;
	inFile.open(filename, std::ifstream::in);
	vector<int>* geneIds = copyNumberVariation->genes;

	map<string, int>::iterator end = geneSymbolToId->end(); // gene not found in the network

	if (inFile.is_open()) {

		//read the first line (sample name)
		string samples;
		getline(inFile, samples);

		//read the output from the
		while (inFile.good()) {
			string s;
			if (!getline(inFile, s))
				break;

			istringstream ss(s);
			vector<int> row;

			int i = 0;	// for read gene in the first column;
			bool found = false;	// found in the network

			while (ss) {
				string s;
				if (!getline(ss, s, delim))
					break;
				if(i == 0){ //read gene symbols
					map<string, int>::iterator it = geneSymbolToId->find(s);
					if(it != end){	//found in the network
						found = true;
						geneIds->push_back(it->second);
					}else{
						found = false;
					}
				}else{		//read mutation values
					row.push_back(atoi(s.c_str()));
				}

				if(!found)
					break;

				i++;
			}

			if(found){
				copyNumberVariation->matrix->push_back(row);
			}

		}
		inFile.close();
	} else {
		cerr << "Error opening file\n";
	}
}

int findIndex(vector<int>* geneIds, int currentGeneId){
	int index = -1;	//not found
	int size = geneIds->size();

	for (int i = 0; i < size; ++i) {
		if(geneIds->at(i) == currentGeneId){
			return i;
		}
	}
	return index;
}

void readGenesList(const char* filename, vector<int>* geneIds, map<string, int>* geneSymbolToId){
	ifstream inFile;
	inFile.open(filename, std::ifstream::in);

	if (inFile.is_open()) {

		//read the output from the
		while (inFile.good()) {
			string s;
			if (!getline(inFile, s))
				break;

			istringstream ss(s);
			map<string, int>::iterator it = geneSymbolToId->find(s);
			geneIds->push_back(it->second);

		}
		inFile.close();
	} else {
		cerr << "Error opening file\n";
	}
}


