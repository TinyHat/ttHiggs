/**
 * N. Cooper-Smith
 * 
 * An example program which calls SMTnTupleAnalysis to run over the 
 * the mini-nTuples for the SMT SL ttbar analysis
 */

#include <iostream>
#include <map>
#include "SMTnTupAnalysis/SMTttbarLplusJetsAnalysis.h"
#include "TChain.h"
#include "TString.h"

using std::cout;
using std::endl;

//Pre-declaration of functions
void processAll(TString inFileName, TString treeTagName,TString options);
void processOnce(TString inFileName, TString treeTagName,TString options,TString jetInc, TString WjetsSysType="", int idsys=0);
void setWjetsSysMap(std::map<TString,int> &inputMap);

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
		processAll(inputFileName,treeTagName,options);

	}

  return 0;
}

void processAll(TString inFileName, TString treeTagName, TString options){
  

  if (options.Contains("Wsample") && treeTagName=="STANDARD_NOMINAL"){
    std::map<TString,int> wjetsSysMap;
    setWjetsSysMap(wjetsSysMap);
    std::map<TString,int>::iterator iter;
    for (iter = wjetsSysMap.begin(); iter != wjetsSysMap.end(); iter++) {
      cout << "Running analysis for a Wjets sample with sys = " << iter->first << "(" << iter->second << ")" << endl;
      processOnce(inFileName, treeTagName, options, "3jinc",iter->first,iter->second);
      processOnce(inFileName, treeTagName, options, "4jinc",iter->first,iter->second);
    }
  }
  else {
    processOnce(inFileName, treeTagName, options, "3jinc");
    processOnce(inFileName, treeTagName, options, "4jinc");
  }

}

void processOnce(TString inFileName, TString treeTagName, TString options, TString jetInc, TString WjetsSysType, int idsys){
  
  TString outFileName(inFileName);
	outFileName.ReplaceAll(".root","");
	outFileName.ReplaceAll("Output","TopD3PD");
  
	//------ Do electron channel

	TChain* chain_el = new TChain("SemiLepSMT_el_"+treeTagName,"");
	chain_el->Add(inFileName);
  
	SMTttbarLplusJetsAnalysis* anal_el = new SMTttbarLplusJetsAnalysis(chain_el,treeTagName,outFileName+"_"+treeTagName+WjetsSysType+"_"+jetInc+"_el.root", idsys);
	chain_el->Process(anal_el,"el "+options+" "+jetInc);
  
	delete anal_el;
  delete chain_el;  
  
  // -------------------------
  
  
	//------ Do muon channel

	TChain* chain_mu = new TChain("SemiLepSMT_mu_"+treeTagName,"");
	chain_mu->Add(inFileName);
  
	SMTttbarLplusJetsAnalysis* anal_mu = new SMTttbarLplusJetsAnalysis(chain_mu,treeTagName,outFileName+"_"+treeTagName+WjetsSysType+"_"+jetInc+"_mu.root", idsys);
	chain_mu->Process(anal_mu,"mu "+options+" "+jetInc);
  
	delete anal_mu;
  delete chain_mu;
  
  // -------------------------
  
}



void setWjetsSysMap(std::map<TString,int> &inputMap){
  inputMap[""]=0;
  inputMap["_FHFvsFLF_UP"]=2000;
  inputMap["_FHFvsFLF_DOWN"]=-2000;
  inputMap["_FBBCCvsFC_UP"]=2001;
  inputMap["_FBBCCvsFC_DOWN"]=-2001;
  inputMap["_FBB25_UP"]=2003;
  inputMap["_FBB25_DOWN"]=-2003;
  inputMap["_FC25_UP"]=2004;
  inputMap["_FC25_DOWN"]=-2004;
  inputMap["_CANORM_UP"]=2005;
  inputMap["_CANORM_DOWN"]=-2005;
}
