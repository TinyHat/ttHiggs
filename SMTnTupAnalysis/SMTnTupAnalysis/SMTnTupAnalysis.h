//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri May 18 21:29:35 2012 by ROOT version 5.30/01
// from TTree SemiLepSMT_el_STANDARD_NOMINAL/4-vectors + variables required for scaling factors
// found on file: ../run/OutputHistos.root
//////////////////////////////////////////////////////////

#ifndef SMTnTupAnalysis_h
#define SMTnTupAnalysis_h

#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TSelector.h"
#include <vector>
#include "TString.h"
#include "SMTnTupAnalysis/HistogramContainer.h"

using std::vector;

class SMTnTupAnalysis : public TSelector {
  
private:
  TFile*         m_outRootFile;
  TChain*        m_chain;
  TString        m_treeTagName;
  TString        m_outFileName;

  bool           m_isElectron;
  bool           m_isMC;
  
  double         m_evts;
  double         m_evtsPassPretag;
  double         m_evtsPassPretagWgt;
  double         m_evtsPassTag;
  double         m_evtsPassTagWgt;
  double         m_intitalSampleSize;
  double         m_sampleScale;
  
  // Histograms held in a container so no need to have loads of TH1.. definitions
  HistogramContainer *m_histograms;
  
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   // Declaration of leaf types
   Int_t           RunNumber;
   Int_t           EventNumber;
   Int_t           ChannelNumber;
   Int_t           DataPeriod;
   Float_t         Mu;
   Int_t           nVtx;
   Float_t         MCXSec;
   Float_t         MCWgt;
   Float_t         PUWgt;
   Float_t         LeptonSF;
   Float_t         WSF;
   Float_t         JVFSF;
   Float_t         SMTSF;
   Float_t         QCDMMWgt;
   Float_t         BRWgt_NOM;
   Float_t         BRWgt_B_MU_UP;
   Float_t         BRWgt_B_MU_DOWN;
   Float_t         BRWgt_B_TAU_MU_UP;
   Float_t         BRWgt_B_TAU_MU_DOWN;
   Float_t         BRWgt_B_D_MUPLUS_UP;
   Float_t         BRWgt_B_D_MUPLUS_DOWN;
   Float_t         BRWgt_B_D_MUMINUS_UP;
   Float_t         BRWgt_B_D_MUMINUS_DOWN;
   Float_t         BRWgt_C_MU_UP;
   Float_t         BRWgt_C_MU_DOWN;
   Float_t         StdCorrsSF;
   Float_t         TotalSFPretag;
   Float_t         TotalWgtPretag;
   Float_t         TotalSFTag;
   Float_t         TotalWgtTag;
   Int_t           HFOR;
   UInt_t          IsEvent_MV1;
   UInt_t          IsEvent_SMT;
   Float_t         lep_pt;
   Float_t         lep_eta;
   Float_t         lep_phi;
   Float_t         lep_E;
   Float_t         lep_charge;
   Int_t           lep_isTight;
   Float_t         el_cl_eta;
   Float_t         lep_etcone20;
   Float_t         lep_ptcone20;
   UInt_t          jet_n;
   vector<float>   *jet_pt;
   vector<float>   *jet_eta;
   vector<float>   *jet_phi;
   vector<float>   *jet_E;
   vector<float>   *jet_jvf;
   vector<float>   *jet_emFrac;
   vector<float>   *jet_nTracks;
   vector<float>   *jet_SV0;
   vector<float>   *jet_COMBNN;
   vector<float>   *jet_MV1;
   vector<unsigned int> *jet_isBJet_COMBNN;
   vector<unsigned int> *jet_isBJet_MV1;
   vector<unsigned int> *jet_isBJet_SMT;
   vector<int>     *jet_flavor_truth_label;
   vector<float>   *jet_SMTMuon_pt;
   vector<float>   *jet_SMTMuon_eta;
   vector<float>   *jet_SMTMuon_phi;
   vector<float>   *jet_SMTMuon_E;
   vector<float>   *jet_SMTMuon_charge;
   vector<float>   *jet_SMTMuon_chi2;
   vector<float>   *jet_SMTMuon_dRJet;
   vector<float>   *jet_SMTMuon_d0;
   vector<float>   *jet_SMTMuon_z0;
   vector<float>   *jet_SMTMuon_sinTheta;
   vector<float>   *jet_SMTMuon_relChargeWmuSMTMu;
   vector<float>   *jet_SMTMuon_invMassWmuSMTMu;
   vector<float>   *jet_SMTMuon_ptRel;
   Float_t         met_sumet;
   Float_t         met_et;
   Float_t         met_phi;
   Float_t         mwt;

   // List of branches
   TBranch        *b_RunNumber;   //!
   TBranch        *b_EventNumber;   //!
   TBranch        *b_ChannelNumber;   //!
   TBranch        *b_DataPeriod;   //!
   TBranch        *b_Mu;   //!
   TBranch        *b_nVtx;   //!
   TBranch        *b_MCXSec;   //!
   TBranch        *b_MCWgt;   //!
   TBranch        *b_PUWgt;   //!
   TBranch        *b_LeptonSF;   //!
   TBranch        *b_WSF;   //!
   TBranch        *b_JVFSF;   //!
   TBranch        *b_SMTSF;   //!
   TBranch        *b_QCDMMWgt;   //!
   TBranch        *b_BRWgt_NOM;   //!
   TBranch        *b_BRWgt_B_MU_UP;   //!
   TBranch        *b_BRWgt_B_MU_DOWN;   //!
   TBranch        *b_BRWgt_B_TAU_MU_UP;   //!
   TBranch        *b_BRWgt_B_TAU_MU_DOWN;   //!
   TBranch        *b_BRWgt_B_D_MUPLUS_UP;   //!
   TBranch        *b_BRWgt_B_D_MUPLUS_DOWN;   //!
   TBranch        *b_BRWgt_B_D_MUMINUS_UP;   //!
   TBranch        *b_BRWgt_B_D_MUMINUS_DOWN;   //!
   TBranch        *b_BRWgt_C_MU_UP;   //!
   TBranch        *b_BRWgt_C_MU_DOWN;   //!
   TBranch        *b_StdCorrsSF;   //!
   TBranch        *b_TotalSFPretag;   //!
   TBranch        *b_TotalWgtPretag;   //!
   TBranch        *b_TotalSFTag;   //!
   TBranch        *b_TotalWgtTag;   //!
   TBranch        *b_HFOR;   //!
   TBranch        *b_IsEvent_MV1;   //!
   TBranch        *b_IsEvent_SMT;   //!
   TBranch        *b_lep_pt;   //!
   TBranch        *b_lep_eta;   //!
   TBranch        *b_lep_phi;   //!
   TBranch        *b_lep_E;   //!
   TBranch        *b_lep_charge;   //!
   TBranch        *b_lep_isTight;   //!
   TBranch        *b_el_cl_eta;   //!
   TBranch        *b_lep_etcone20;   //!
   TBranch        *b_lep_ptcone20;   //!
   TBranch        *b_jet_n;   //!
   TBranch        *b_jet_pt;   //!
   TBranch        *b_jet_eta;   //!
   TBranch        *b_jet_phi;   //!
   TBranch        *b_jet_E;   //!
   TBranch        *b_jet_jvf;   //!
   TBranch        *b_jet_emFrac;   //!
   TBranch        *b_jet_nTracks;   //!
   TBranch        *b_jet_SV0;   //!
   TBranch        *b_jet_COMBNN;   //!
   TBranch        *b_jet_MV1;   //!
   TBranch        *b_jet_isBJet_COMBNN;   //!
   TBranch        *b_jet_isBJet_MV1;   //!
   TBranch        *b_jet_isBJet_SMT;   //!
   TBranch        *b_jet_flavor_truth_label;   //!
   TBranch        *b_jet_SMTMuon_pt;   //!
   TBranch        *b_jet_SMTMuon_eta;   //!
   TBranch        *b_jet_SMTMuon_phi;   //!
   TBranch        *b_jet_SMTMuon_E;   //!
   TBranch        *b_jet_SMTMuon_charge;   //!
   TBranch        *b_jet_SMTMuon_chi2;   //!
   TBranch        *b_jet_SMTMuon_dRJet;   //!
   TBranch        *b_jet_SMTMuon_d0;   //!
   TBranch        *b_jet_SMTMuon_z0;   //!
   TBranch        *b_jet_SMTMuon_sinTheta;   //!
   TBranch        *b_jet_SMTMuon_relChargeWmuSMTMu;   //!
   TBranch        *b_jet_SMTMuon_invMassWmuSMTMu;   //!
   TBranch        *b_jet_SMTMuon_ptRel;   //!
   TBranch        *b_met_sumet;   //!
   TBranch        *b_met_et;   //!
   TBranch        *b_met_phi;   //!
   TBranch        *b_mwt;   //!

   SMTnTupAnalysis(TTree * /*tree*/ =0) { }
   SMTnTupAnalysis(TChain* chain, TString treeTagName, TString outFileName);
   virtual ~SMTnTupAnalysis();
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree) { }
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate() { }
   virtual void    Terminate();

   ClassDef(SMTnTupAnalysis,0);
};

#endif

#ifdef SMTnTupAnalysis_cxx
void SMTnTupAnalysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   jet_pt = 0;
   jet_eta = 0;
   jet_phi = 0;
   jet_E = 0;
   jet_jvf = 0;
   jet_emFrac = 0;
   jet_nTracks = 0;
   jet_SV0 = 0;
   jet_COMBNN = 0;
   jet_MV1 = 0;
   jet_isBJet_COMBNN = 0;
   jet_isBJet_MV1 = 0;
   jet_isBJet_SMT = 0;
   jet_flavor_truth_label = 0;
   jet_SMTMuon_pt = 0;
   jet_SMTMuon_eta = 0;
   jet_SMTMuon_phi = 0;
   jet_SMTMuon_E = 0;
   jet_SMTMuon_charge = 0;
   jet_SMTMuon_chi2 = 0;
   jet_SMTMuon_dRJet = 0;
   jet_SMTMuon_d0 = 0;
   jet_SMTMuon_z0 = 0;
   jet_SMTMuon_sinTheta = 0;
   jet_SMTMuon_relChargeWmuSMTMu = 0;
   jet_SMTMuon_invMassWmuSMTMu = 0;
   jet_SMTMuon_ptRel = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
   fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
   fChain->SetBranchAddress("ChannelNumber", &ChannelNumber, &b_ChannelNumber);
   fChain->SetBranchAddress("DataPeriod", &DataPeriod, &b_DataPeriod);
   fChain->SetBranchAddress("Mu", &Mu, &b_Mu);
   fChain->SetBranchAddress("nVtx", &nVtx, &b_nVtx);
   fChain->SetBranchAddress("MCXSec", &MCXSec, &b_MCXSec);
   fChain->SetBranchAddress("MCWgt", &MCWgt, &b_MCWgt);
   fChain->SetBranchAddress("PUWgt", &PUWgt, &b_PUWgt);
   fChain->SetBranchAddress("LeptonSF", &LeptonSF, &b_LeptonSF);
   fChain->SetBranchAddress("WSF", &WSF, &b_WSF);
   fChain->SetBranchAddress("JVFSF", &JVFSF, &b_JVFSF);
   fChain->SetBranchAddress("SMTSF", &SMTSF, &b_SMTSF);
   fChain->SetBranchAddress("QCDMMWgt", &QCDMMWgt, &b_QCDMMWgt);
   fChain->SetBranchAddress("BRWgt_NOM", &BRWgt_NOM, &b_BRWgt_NOM);
   fChain->SetBranchAddress("BRWgt_B_MU_UP", &BRWgt_B_MU_UP, &b_BRWgt_B_MU_UP);
   fChain->SetBranchAddress("BRWgt_B_MU_DOWN", &BRWgt_B_MU_DOWN, &b_BRWgt_B_MU_DOWN);
   fChain->SetBranchAddress("BRWgt_B_TAU_MU_UP", &BRWgt_B_TAU_MU_UP, &b_BRWgt_B_TAU_MU_UP);
   fChain->SetBranchAddress("BRWgt_B_TAU_MU_DOWN", &BRWgt_B_TAU_MU_DOWN, &b_BRWgt_B_TAU_MU_DOWN);
   fChain->SetBranchAddress("BRWgt_B_D_MUPLUS_UP", &BRWgt_B_D_MUPLUS_UP, &b_BRWgt_B_D_MUPLUS_UP);
   fChain->SetBranchAddress("BRWgt_B_D_MUPLUS_DOWN", &BRWgt_B_D_MUPLUS_DOWN, &b_BRWgt_B_D_MUPLUS_DOWN);
   fChain->SetBranchAddress("BRWgt_B_D_MUMINUS_UP", &BRWgt_B_D_MUMINUS_UP, &b_BRWgt_B_D_MUMINUS_UP);
   fChain->SetBranchAddress("BRWgt_B_D_MUMINUS_DOWN", &BRWgt_B_D_MUMINUS_DOWN, &b_BRWgt_B_D_MUMINUS_DOWN);
   fChain->SetBranchAddress("BRWgt_C_MU_UP", &BRWgt_C_MU_UP, &b_BRWgt_C_MU_UP);
   fChain->SetBranchAddress("BRWgt_C_MU_DOWN", &BRWgt_C_MU_DOWN, &b_BRWgt_C_MU_DOWN);
   fChain->SetBranchAddress("StdCorrsSF", &StdCorrsSF, &b_StdCorrsSF);
   fChain->SetBranchAddress("TotalSFPretag", &TotalSFPretag, &b_TotalSFPretag);
   fChain->SetBranchAddress("TotalWgtPretag", &TotalWgtPretag, &b_TotalWgtPretag);
   fChain->SetBranchAddress("TotalSFTag", &TotalSFTag, &b_TotalSFTag);
   fChain->SetBranchAddress("TotalWgtTag", &TotalWgtTag, &b_TotalWgtTag);
   fChain->SetBranchAddress("HFOR", &HFOR, &b_HFOR);
   fChain->SetBranchAddress("IsEvent_MV1", &IsEvent_MV1, &b_IsEvent_MV1);
   fChain->SetBranchAddress("IsEvent_SMT", &IsEvent_SMT, &b_IsEvent_SMT);
   fChain->SetBranchAddress("lep_pt", &lep_pt, &b_lep_pt);
   fChain->SetBranchAddress("lep_eta", &lep_eta, &b_lep_eta);
   fChain->SetBranchAddress("lep_phi", &lep_phi, &b_lep_phi);
   fChain->SetBranchAddress("lep_E", &lep_E, &b_lep_E);
   fChain->SetBranchAddress("lep_charge", &lep_charge, &b_lep_charge);
   fChain->SetBranchAddress("lep_isTight", &lep_isTight, &b_lep_isTight);
   fChain->SetBranchAddress("el_cl_eta", &el_cl_eta, &b_el_cl_eta);
   fChain->SetBranchAddress("lep_etcone20", &lep_etcone20, &b_lep_etcone20);
   fChain->SetBranchAddress("lep_ptcone20", &lep_ptcone20, &b_lep_ptcone20);
   fChain->SetBranchAddress("jet_n", &jet_n, &b_jet_n);
   fChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
   fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
   fChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
   fChain->SetBranchAddress("jet_E", &jet_E, &b_jet_E);
   fChain->SetBranchAddress("jet_jvf", &jet_jvf, &b_jet_jvf);
   fChain->SetBranchAddress("jet_emFrac", &jet_emFrac, &b_jet_emFrac);
   fChain->SetBranchAddress("jet_nTracks", &jet_nTracks, &b_jet_nTracks);
   fChain->SetBranchAddress("jet_SV0", &jet_SV0, &b_jet_SV0);
   fChain->SetBranchAddress("jet_COMBNN", &jet_COMBNN, &b_jet_COMBNN);
   fChain->SetBranchAddress("jet_MV1", &jet_MV1, &b_jet_MV1);
   fChain->SetBranchAddress("jet_isBJet_COMBNN", &jet_isBJet_COMBNN, &b_jet_isBJet_COMBNN);
   fChain->SetBranchAddress("jet_isBJet_MV1", &jet_isBJet_MV1, &b_jet_isBJet_MV1);
   fChain->SetBranchAddress("jet_isBJet_SMT", &jet_isBJet_SMT, &b_jet_isBJet_SMT);
   fChain->SetBranchAddress("jet_flavor_truth_label", &jet_flavor_truth_label, &b_jet_flavor_truth_label);
   fChain->SetBranchAddress("jet_SMTMuon_pt", &jet_SMTMuon_pt, &b_jet_SMTMuon_pt);
   fChain->SetBranchAddress("jet_SMTMuon_eta", &jet_SMTMuon_eta, &b_jet_SMTMuon_eta);
   fChain->SetBranchAddress("jet_SMTMuon_phi", &jet_SMTMuon_phi, &b_jet_SMTMuon_phi);
   fChain->SetBranchAddress("jet_SMTMuon_E", &jet_SMTMuon_E, &b_jet_SMTMuon_E);
   fChain->SetBranchAddress("jet_SMTMuon_charge", &jet_SMTMuon_charge, &b_jet_SMTMuon_charge);
   fChain->SetBranchAddress("jet_SMTMuon_chi2", &jet_SMTMuon_chi2, &b_jet_SMTMuon_chi2);
   fChain->SetBranchAddress("jet_SMTMuon_dRJet", &jet_SMTMuon_dRJet, &b_jet_SMTMuon_dRJet);
   fChain->SetBranchAddress("jet_SMTMuon_d0", &jet_SMTMuon_d0, &b_jet_SMTMuon_d0);
   fChain->SetBranchAddress("jet_SMTMuon_z0", &jet_SMTMuon_z0, &b_jet_SMTMuon_z0);
   fChain->SetBranchAddress("jet_SMTMuon_sinTheta", &jet_SMTMuon_sinTheta, &b_jet_SMTMuon_sinTheta);
   fChain->SetBranchAddress("jet_SMTMuon_relChargeWmuSMTMu", &jet_SMTMuon_relChargeWmuSMTMu, &b_jet_SMTMuon_relChargeWmuSMTMu);
   fChain->SetBranchAddress("jet_SMTMuon_invMassWmuSMTMu", &jet_SMTMuon_invMassWmuSMTMu, &b_jet_SMTMuon_invMassWmuSMTMu);
   fChain->SetBranchAddress("jet_SMTMuon_ptRel", &jet_SMTMuon_ptRel, &b_jet_SMTMuon_ptRel);
   fChain->SetBranchAddress("met_sumet", &met_sumet, &b_met_sumet);
   fChain->SetBranchAddress("met_et", &met_et, &b_met_et);
   fChain->SetBranchAddress("met_phi", &met_phi, &b_met_phi);
   fChain->SetBranchAddress("mwt", &mwt, &b_mwt);
}

Bool_t SMTnTupAnalysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef SMTnTupAnalysis_cxx
