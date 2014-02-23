/**
 * N. Cooper-Smith
 * 
 * An example program which calls SMTnTupleAnalysis to run over the 
 * the mini-nTuples for the SMT SL ttbar analysis
 */

#include <iostream>
#include "SMTnTupAnalysis/SMTttbarLplusJetsAnalysis.h"
#include "TChain.h"
#include "TString.h"

using std::cout;
using std::endl;

void process(TString inFileName, TString treeTagName,TString options);

int main(int argc, char *argv[]) {
  
  
	if ( argc != 8){
		cout << "usage: " << argv[0] << " <InputFileName> <TreeTagName> <isMC=true,false> <isWsample=true,false> <isZsample=true,false> <isQCDDD=true,false> <isWRaw=true,false> " << endl;
	}
	else
	{
		TString inputFileName(argv[1]);
		TString treeTagName(argv[2]);
		TString options("");
		if (TString(argv[3]) == "true") options += " MC ";
		if (TString(argv[4]) == "true") options += " Wsample ";
		if (TString(argv[5]) == "true") options += " Zsample ";
		if (TString(argv[6]) == "true") options += " QCDDD ";
		if (TString(argv[7]) == "true") options += " WRaw ";
		process(inputFileName,treeTagName,options);
	}

  return 0;
}

void process(TString inFileName, TString treeTagName, TString options){
  

	TString outFileName(inFileName);
	outFileName.ReplaceAll(".root","");
	outFileName.ReplaceAll("Output","TopD3PD");

	//Do electron channel  
	TChain* chain_el = new TChain("SemiLepSMT_el"+treeTagName,"");
	chain_el->Add(inFileName);
  
	SMTttbarLplusJetsAnalysis* anal_el = new SMTttbarLplusJetsAnalysis(chain_el,treeTagName,outFileName+"_el.root");
	chain_el->Process(anal_el,options);
  
	delete anal_el;

	//Do muon channel  
	TChain* chain_mu = new TChain("SemiLepSMT_mu"+treeTagName,"");
	chain_mu->Add(inFileName);
  
	SMTttbarLplusJetsAnalysis* anal_mu = new SMTttbarLplusJetsAnalysis(chain_mu,treeTagName,outFileName+"_mu.root");
	chain_mu->Process(anal_mu,options);
  
	delete anal_el;

}
