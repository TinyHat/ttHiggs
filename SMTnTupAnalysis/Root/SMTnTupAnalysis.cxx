#define SMTnTupAnalysis_cxx

#include "SMTnTupAnalysis/SMTnTupAnalysis.h"
#include "TH1.h"
#include <iostream>

using std::endl;
using std::cout;


SMTnTupAnalysis::SMTnTupAnalysis(TChain* chain, TString treeTagName, TString outFileName){
  
  m_outRootFile = NULL;
  m_chain = chain;
  m_treeTagName = treeTagName;
  m_outFileName = outFileName;
  
  m_isElectron = false;
  m_isMC = false;
  
  // Init counters etc
	m_evts = 0.;
  m_evtsPassPretag = 0.;
  m_evtsPassPretagWgt = 0.;
  m_evtsPassTag = 0.;
  m_evtsPassTagWgt = 0.;
  
  m_intitalSampleSize = 0.;
  m_sampleScale = 0.;
  
  //histo container
	m_histograms = new HistogramContainer("");
  
}

SMTnTupAnalysis::~SMTnTupAnalysis(){
  delete m_outRootFile;
  delete m_histograms;
}

void SMTnTupAnalysis::Begin(TTree * /*tree*/)
{

	// cleanup pointers in case function has been executed before
	gDirectory->Delete("All");
  
  // -------- Deal with passed options ----------------
	TString option = GetOption();
	cout << "| * Options set to = " << option << endl;
  
	if (option.Contains(" EL ")) {
		cout << "| * This is the electron channel" << endl;
		m_isElectron = true;
	}
	else {
		cout << "| * This is the muon channel" << endl;
		m_isElectron = false;
	}
	if (option.Contains("MC")) {
		cout << "| * This is MC" << endl;
		m_isMC = true;
  }
  // ---------------------------------------------------
  
  //lepton stuff
	if (m_isElectron){
		m_histograms->add1DHisto(new TH1D("h_el_pretag_pt_wgt", "Electron Pt Pretag Scaled/Weighted",100, 0, 500));
		m_histograms->add1DHisto(new TH1D("h_el_tag_pt_wgt", "Electron Pt Tag Scaled/Weighted",100, 0, 500));
  }
  else{
		m_histograms->add1DHisto(new TH1D("h_mu_pretag_pt_wgt", "Muon Pt Pretag Scaled/Weighted",100, 0, 500));
		m_histograms->add1DHisto(new TH1D("h_mu_tag_pt_wgt", "Muon Pt Tag Scaled/Weighted",100, 0, 500));
  }

}


Bool_t SMTnTupAnalysis::Process(Long64_t entry)
{
	// Gets the next Tree entry i.e. the next event
	fChain->GetTree()->GetEntry(entry);

  // increment event counter
  m_evts++;

  // Retrieve cut flow histograms
	TH1D* h_cutFlow = NULL;
	if (m_evts == 1){
		TFile * currentFile = (TFile*)m_chain->GetFile();
		if (m_isElectron) h_cutFlow = (TH1D*)currentFile->Get("h_cutFlow_el_mcwgt_"+m_treeTagName);
		else h_cutFlow = (TH1D*)currentFile->Get("h_cutFlow_mu_mcwgt_"+m_treeTagName);
    if (h_cutFlow != NULL){
      cout << "| * This file was run over: " << endl;
      if (m_isMC){
        int bin=3;
        m_intitalSampleSize = h_cutFlow->GetBinContent(bin);
        m_evts = h_cutFlow->GetBinContent(bin);
				cout << "|-> Channel Number = " << ChannelNumber << endl;
				cout << "|-> Initial Events = " << m_intitalSampleSize << endl;
				cout << "|-> Sigma*kFac = " << MCXSec << " pb" << endl;
				cout << "|-> Sample Lumi = " << m_intitalSampleSize/MCXSec << " pb-1" << endl;
				cout << "|-> Data Lumi = " << 4713.11 << endl;
				m_sampleScale = (m_intitalSampleSize/MCXSec) / 4713.11;
				cout << "|-> Sample Scale = " << m_sampleScale << endl;
      }
      else {
        m_intitalSampleSize = h_cutFlow->GetBinContent(1);
        cout << "|-> Run Number = " << RunNumber << endl;
        cout << "|-> Initial Events = " << m_intitalSampleSize << endl;
        cout << "|-> After GRL = " << h_cutFlow->GetBinContent(2) << endl;
      }
    }
    else {
      cout << "| * WARNING:: Could not retrieve cut flow histogram from fChain->GetTree()->GetUserInfo()" << endl;
    }
	}
  
  // Set event weights
	double eventWeightPretag = m_sampleScale * TotalSFPretag;
	double eventWeightTag = m_sampleScale * TotalSFTag;
  
  // Simple progress counter
	if ((int)m_evts % 10000 == 0) cout << m_evts << " Events Processed" << endl; 

  // Apply hfor (should already be applied so safety check)
  if (HFOR == 4) return kTRUE;

  //Apply MET & MTW cuts here
  if (m_isElectron){
    if (met_et/1000. <= 30) return kTRUE;
    if (mwt/1000. <= 30) return kTRUE;
  }
  else {
    if (met_et/1000. <= 20) return kTRUE;
    if ((met_et+mwt)/1000. <= 60) return kTRUE;
  }
  
  //Apply jet cuts
  if (jet_n < 3) return kTRUE;
  
  //fill some histograms
  if (m_isElectron) m_histograms->find1DHisto("h_el_pretag_pt_wgt")->Fill(lep_pt/1000.,eventWeightPretag);
  else m_histograms->find1DHisto("h_mu_pretag_pt_wgt")->Fill(lep_pt/1000.,eventWeightPretag);
  
  m_evtsPassPretag++;
  m_evtsPassPretagWgt+=eventWeightPretag;
  
  
  // At least one SMT
  if (!IsEvent_SMT) return kTRUE;
  
  //fill some histograms
  if (m_isElectron) m_histograms->find1DHisto("h_el_tag_pt_wgt")->Fill(lep_pt/1000.,eventWeightPretag);
  else m_histograms->find1DHisto("h_mu_tag_pt_wgt")->Fill(lep_pt/1000.,eventWeightPretag);
  
  
  m_evtsPassTag++;
  m_evtsPassTagWgt+=eventWeightTag;
  
  return kTRUE;
}

void SMTnTupAnalysis::Terminate()
{
  
  cout << "| ****** Terminating..." << endl;
  cout << "|-> Total Evts Processed = " << m_evts << endl;
  cout << "|-> Pass Evts Pretag = " << m_evtsPassPretag << endl;
  cout << "|-> Pass Evts PretagWgt = " << m_evtsPassPretagWgt << endl;
  cout << "|-> Pass Evts Tag = " << m_evtsPassTag << endl;
  cout << "|-> Pass Evts Tag Wgt = " << m_evtsPassTagWgt << endl;

  // Write all histograms and close root file
	if (m_isElectron) m_outRootFile = new TFile(m_outFileName.Data(),"RECREATE");
	else m_outRootFile = new TFile(m_outFileName.Data(),"RECREATE");
  
	m_histograms->write1DHistos();
	m_histograms->write2DHistos();
  m_outRootFile->Close();
	cout << "| ****** Terminating... Done" << endl;


}
