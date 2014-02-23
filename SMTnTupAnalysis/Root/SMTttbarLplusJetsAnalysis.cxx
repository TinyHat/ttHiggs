#define SMTttbarLplusJetsAnalysis_cxx

#include "SMTnTupAnalysis/SMTttbarLplusJetsAnalysis.h"
#include "WjetsCorrections/HFsys.h"
#include "TH1.h"
#include "TLorentzVector.h"
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <algorithm>

using std::endl;
using std::cout;

// Constructor
SMTttbarLplusJetsAnalysis::SMTttbarLplusJetsAnalysis(TChain* chain, TString treeTagName, TString outFileName, int wjetsSysType){
  
  m_outRootFile = NULL;
  m_chain = chain;
  m_treeTagName = treeTagName;
  m_outFileName = outFileName;
  m_channelTag = "";
  m_jetincTag = "3j";
  
  m_isElectron = false;
  m_isWsample = false;
  m_isZsample = false;
  m_isMC = false;
  m_isqcdDD = false;
  m_isqcdAB = false;
  m_isRaw = false;
  m_isMC = false;
  m_is3jin = true;
  m_jetIncN = 3;
             
  // Declare and initialize global counters here
  m_evts = 0.;

  //own global counters
  jet_counter = 0; //counting number of jets
  total_bJets = 0; //counting number of btagged jets
  zero_MV1 = 0; //for counting how many events have 0 - >4 MV1 tagged bjets
  one_MV1 = 0;
  two_MV1 = 0; 
  three_MV1 = 0; 
  four_MV1 = 0; 
  fourPlus_MV1 = 0;
  num_bquarks = 0; //number of truth bquarks
  num_bbarquarks = 0; //number of truth bbar quarks
  closeJets_bTagged = 0; //counts number of jets that are close to b-quark that are b-tagged.
  numCloseJets_b = 0; //number of jets close to b
  numCloseJets_bbar = 0; // number of jets close to bbar
  numFarJets = 0; //number of jets not close to b-quarks
  farJets_bTagged = 0; //number of jets not close to b-quarks that are b-tagged
  num_farbQuarks = 0; //count number of b-quarks far from jets
  num_closeJetsb_untagged = 0; //close untagged jets
  num_closeJetsbbar_untagged = 0; //close untagged jets
  numFarJets_total = 0; //total number of far jets
  bQuarkPT_inRange = 0; 
  bQuarkEta_inRange = 0;
  bQuarkEtaPT_inRange = 0;
  num_higgs = 0; // number of Higgs bosons 


  m_intitalSampleSize = 0.;
  m_sampleScale = 1.;
  
  m_wjetsSysType = wjetsSysType;
  
  //histo container
  m_histograms = new HistogramContainer("");
  
}

SMTttbarLplusJetsAnalysis::~SMTttbarLplusJetsAnalysis(){
  delete m_outRootFile;
  delete m_histograms;
}

// The Begin function is where you book all the histograms
void SMTttbarLplusJetsAnalysis::Begin(TTree * /*tree*/){

  // cleanup pointers in case function has been executed before
  gDirectory->Delete("All");

  // -------- Deal with passed options ----------------
  TString option = GetOption();
  cout << "| * Options set to = " << option << endl;


  if (option.Contains("el ")) {
    cout << "| * This is the electron channel" << endl;
    m_isElectron = true;
    m_channelTag = "el";
    m_leptonPDGID = 11;
    m_neutrinoPDGID = 12;
  }
  else {
    cout << "| * This is the muon channel" << endl;
    m_isElectron = false;
    m_channelTag = "mu";
    m_leptonPDGID = 13;
    m_neutrinoPDGID = 14;
  }
  if (option.Contains("MC")) {
    cout << "| * This is MC" << endl;
    m_isMC = true;
    if (option.Contains("Wsample")) {
      cout << "| * This is a wSample" << endl;
      m_isWsample = true;
    }
    if (option.Contains("WRaw")) {
      cout << "| * Running on raw MC wih no W scales" << endl;
      m_isRaw = true;
    }
    if (option.Contains("Zsample")) {
      cout << "| * Running is a zSample" << endl;
      m_isZsample = true;
    }
  }
  else {
    cout << "| * This is Data" << endl;
    if (option.Contains("QCDDD")) {
      cout << "| * This is a QCD DD sample" << endl;
      m_isqcdDD = true;
    }
  }

  if (m_treeTagName.Contains("ELEQCDAB")){
    cout << "| * This is a QCD AB sample" << endl;
    m_isqcdAB = true;
  }
  
  if (option.Contains("4jin")){
    m_is3jin = false;
    m_jetincTag = "4j";
    cout << "| * Running analysis using 1j,2j,3j,4j(inclusive) bins" << endl;
  }
  else{
    m_is3jin = true;
    m_jetincTag = "3j";
    cout << "| * Running analysis using 1j,2j,3j(inclusive) bins" << endl;
  }

  // ---------------------------------------------------

  //Histograms
  m_histograms->add1DHisto(new TH1D("event_bTags", "How many b-tagged jets per event",6,-0.5,5.5));
  //change bin labels
  m_histograms->find1DHisto("event_bTags")->GetXaxis()->SetBinLabel(1, "0");
  m_histograms->find1DHisto("event_bTags")->GetXaxis()->SetBinLabel(2, "1");
  m_histograms->find1DHisto("event_bTags")->GetXaxis()->SetBinLabel(3, "2");
  m_histograms->find1DHisto("event_bTags")->GetXaxis()->SetBinLabel(4, "3");
  m_histograms->find1DHisto("event_bTags")->GetXaxis()->SetBinLabel(5, "4");
  m_histograms->find1DHisto("event_bTags")->GetXaxis()->SetBinLabel(6, ">4");
  //jet pt and eta for all jets
  m_histograms->add1DHisto(new TH1D("jetPt", "pT of all jets",100,0,500));
  m_histograms->add1DHisto(new TH1D("jetEta", "eta of all jets",40,-4,4));
  //plots for jets that are b-tagged AND near a b-quark 
  m_histograms->add1DHisto(new TH1D("h_closeJets_bTagged_pT", "pT of b-tagged jets that are close to a b-quark", 100, 0, 500));
  m_histograms->add1DHisto(new TH1D("h_closeJets_bTagged_eta", "eta of b-tagged jets that are close to a b-quark", 40, -4, 4));
  m_histograms->add1DHisto(new TH1D("h_closeJets_bTagged_emFrac", "E.M. Fraction of b-tagged jets that are close to a b-quark", 110, 0, 1.1));
  m_histograms->add1DHisto(new TH1D("h_closeJets_bTagged_nTracks", "Number of tracks of b-tagged jets that are close to a b-quark", 35, 0, 35));
  //plots for jets that are b-tagged and NOT near a b-quark
  m_histograms->add1DHisto(new TH1D("h_closeJets_pT", "pT of jets that are close to a b-quark that are not b-tagged", 100, 0, 500));
  m_histograms->add1DHisto(new TH1D("h_closeJets_eta", "eta of jets that are close to a b-quark that are not b-tagged", 40, -4, 4));
  m_histograms->add1DHisto(new TH1D("h_closeJets_emFrac", "E.M. Fraction of jets that are close to a b-quark that are not b-tagged", 110, 0, 1.1));
  m_histograms->add1DHisto(new TH1D("h_closeJets_nTracks", "Number of tracks of jets that are close to a b-quark that are not b-tagged", 35, 0, 35));
  //plots for b-quarks that have no jets close to them
  m_histograms->add1DHisto(new TH1D("h_farQuarks_TruepT", "Truth pT of b-quarks that have no jets close to them", 100, 0, 500));
  m_histograms->add1DHisto(new TH1D("h_farQuarks_TrueEta", "Truth eta of b-quark that have no jets close to them", 40, -4, 4));
  //plots for jets not close to quarks
  m_histograms->add1DHisto(new TH1D("h_farJets_bTagged_pT", "pT of b-tagged jets that are not close to a b-quark", 100, 0, 500));
  m_histograms->add1DHisto(new TH1D("h_farJets_bTagged_eta", "eta of b-tagged jets that are not close to a b-quark", 40, -4, 4));
  m_histograms->add1DHisto(new TH1D("h_farJets_bTagged_emFrac", "E.M. Fraction of b-tagged jets that are not close to a b-quark", 110, 0, 1.1));
  m_histograms->add1DHisto(new TH1D("h_farJets_bTagged_nTracks", "Number of tracks of b-tagged jets that are not close to a b-quark", 35, 0, 35));
  //not b-tagged
  m_histograms->add1DHisto(new TH1D("h_farJets_pT", "pT of jets that are not close to a b-quark and are not b-tagged", 100, 0, 500));
  m_histograms->add1DHisto(new TH1D("h_farJets_eta", "eta of jets that are not close to a b-quark and are not b-tagged", 40, -4, 4));
  m_histograms->add1DHisto(new TH1D("h_farJets_emFrac", "E.M. Fraction of jets that are not close to a b-quark and are not b-tagged", 110, 0, 1.1));
  m_histograms->add1DHisto(new TH1D("h_farJets_nTracks", "Number of tracks of jets that are not close to a b-quark and are not b-tagged", 35, 0, 35));
  //Top histograms
  m_histograms->add1DHisto(new TH1D("h_Top_pT", "pT of Top Quarks", 100, 0, 500));
  //Higgs Histograms
  m_histograms->add1DHisto(new TH1D("h_Higgs_pT", "pT of the Higgs", 100, 0, 500));
  m_histograms->add1DHisto(new TH1D("h_Higgs_eta", "eta of the Higgs", 40, -4, 4));
  //true b-quark histograms - Invariant mass
  m_histograms->add1DHisto(new TH1D("h_bQuark_Minv", "Invariant mass of true b quarks (first pair found)", 100, 0, 500));
  m_histograms->add1DHisto(new TH1D("h_bQuark_Minv2", "Invariant mass of true b quarks (second pair found)", 100, 0, 500));
  //true b-quark histograms - pT and eta
  m_histograms->add1DHisto(new TH1D("h_truebQuarkFromTop_pT", "pT of true b quarks from top quarks", 100, 0, 500));
  m_histograms->add1DHisto(new TH1D("h_truebQuarkFromTop_eta", "eta of true b quarks from top quarks", 40, -2, 2));
  m_histograms->add1DHisto(new TH1D("h_truebQuarkFromHiggs_pT", "pT of true b quarks from Higgs", 100, 0, 500));
  m_histograms->add1DHisto(new TH1D("h_truebQuarkFromHiggs_eta", "eta of true b quarks from Higgs", 40, -2, 2));
  //invariant mass of all b-tagged jet combinations in ttH
  m_histograms->add1DHisto(new TH1D("h_taggedJets12_Minv", "Invariant mass of first and second b-tagged jets", 100, 0, 500));
  m_histograms->add1DHisto(new TH1D("h_taggedJets13_Minv", "Invariant mass of first and third b-tagged jets", 100, 0, 500));
  m_histograms->add1DHisto(new TH1D("h_taggedJets14_Minv", "Invariant mass of first and fourth b-tagged jets", 100, 0, 500));
  m_histograms->add1DHisto(new TH1D("h_taggedJets23_Minv", "Invariant mass of second and third b-tagged jets", 100, 0, 500));
  m_histograms->add1DHisto(new TH1D("h_taggedJets24_Minv", "Invariant mass of second and fourth b-tagged jets", 100, 0, 500));
  m_histograms->add1DHisto(new TH1D("h_taggedJets34_Minv", "Invariant mass of third and fourth b-tagged jets", 100, 0, 500));
  //Delta R between b-quarks from Tops and from Higgs
  m_histograms->add1DHisto(new TH1D("h_bQuarkDR_fromTop", "Delta R between b quarks coming from top quarks", 70, 0, 7));
  m_histograms->add1DHisto(new TH1D("h_bQuarkDR_fromHiggs", "Delta R between b quarks coming from Higgs", 70, 0, 7));
  //Delta R between tagged jets
  m_histograms->add1DHisto(new TH1D("h_taggedJets12_DR", "Delta R between first and second b-tagged jets", 70, 0, 7));
  m_histograms->add1DHisto(new TH1D("h_taggedJets13_DR", "Delta R between first and third b-tagged jets", 70, 0, 7));
  m_histograms->add1DHisto(new TH1D("h_taggedJets14_DR", "Delta R between first and fourth b-tagged jets", 70, 0, 7));
  m_histograms->add1DHisto(new TH1D("h_taggedJets23_DR", "Delta R between second and third b-tagged jets", 70, 0, 7));
  m_histograms->add1DHisto(new TH1D("h_taggedJets24_DR", "Delta R between second and fourth b-tagged jets", 70, 0, 7));
  m_histograms->add1DHisto(new TH1D("h_taggedJets34_DR", "Delta R between third and fourth b-tagged jets", 70, 0, 7));
  //Lepton and Neutrino Plots
  m_histograms->add1DHisto(new TH1D("h_lepton_pT", "pT of lepton in event", 100, 0, 500));
  m_histograms->add1DHisto(new TH1D("h_MET_sumET", "Sum of transverse energy of Neutrino in event", 250, 0, 1500));


  char *rootcoreDir = getenv("ROOTCOREDIR");
  std::string baseDir = "";
  if(rootcoreDir && baseDir == "") {
    baseDir = std::string(rootcoreDir) + "/";
  }
  
  std::string nameMM=baseDir+"/data/FakesMacros/EJetsMM/";
  m_llmmB = new LLMatrixMethodB(nameMM,3); //use choice 2 for Eiso
  m_MatrixUtils = new MatrixUtils();

  std::string nameBK=baseDir+"/data/FakesMacros/MuJetsMMEffs_ICHEP12_mc11c_BK_MV1_70.root";
  std::string nameLM=baseDir+"/data/FakesMacros/MuJetsMMEffs_ICHEP12_mc11c_LM_MV1_70.root";
  std::string nameBfake=baseDir+"/data/FakesMacros/MuJetsMM_B_mc11c_ICHEP12_eff_fake_ALL.root";
  std::string nameBreal=baseDir+"/data/FakesMacros/MuJetsMM_B_mc11c_ICHEP12_eff_real_ALL.root";    
  MJMMA_BK = new MuJetsMatrixMethodA(nameBK, "#||{#eta(#mu)};p_{T}(j1)[GeV]");
  MJMMA_LM = new MuJetsMatrixMethodA(nameLM, "#||{#eta(#mu)};p_{T}(j1)[GeV]");
  MJMMB = new MuJetsMatrixMethodB(nameBfake, nameBreal);

  //log files
  bJetTagslog.open ("log_bJet_tagging_" + m_channelTag + ".txt");
  truthbQuarklog.open ("log_truth_bQuark_info_" + m_channelTag + ".txt");
  jetQuarkDRlog.open ("log_jet_bQuark_DR_" + m_channelTag + ".txt");
  jetsCloseToQuarkslog.open ("log_jetsCloseToQuarks_" + m_channelTag + ".txt");
  bQuarkReconstructionlog.open ("log_bQuark_reconstruction_" + m_channelTag + ".txt");
  higgslog.open ("log_Higgs_info_" + m_channelTag + ".txt");
  closeJetslog.open ("log_information_on_closeJets_" + m_channelTag + ".txt");
  invariantMasslog.open("log_invariantMass_" + m_channelTag + ".txt");
  bQuarkDRlog.open("log_bQuarkDR_" + m_channelTag + ".txt");
}

// The Process function runs every event: this is where most of the code goes
Bool_t SMTttbarLplusJetsAnalysis::Process(Long64_t entry)
{
  fChain->GetTree()->GetEntry(entry);

  //count number of jets in each event
  int event_jetCounter = 0;
  //counter for b-tagged jets (MV1)
  bJet_counter = 0;
  //variable to find index of b and bbar in jets
  b_index = new vector<int>;
  bbar_index = new vector<int>;
  //test variable to see if DR between jet and b/bbar is small
  float bQuark1_jet_DR_test = 0.4;
  float bbar1_jet_DR_test = 0.4;
  float bQuark2_jet_DR_test = 0.4;
  float bbar2_jet_DR_test = 0.4;
  //variable to find index of jet closest to b & bbar
  closestJet_b1 = -999;
  closestJet_bbar1 = -999;
  closestJet_b2 = -999;
  closestJet_bbar2 = -999;
  // condition that quark has a jet close to it
  bool jetisClose = false;
  //variable to find index of Higgs in truth info
  int higgs_index = -999;
  //invariant mass of b quarks
  bQuark_Minv = -999;
  bQuark_Minv = -999;
  //invariant mass between tagged jets
  Minv_jets12 = 0;
  Minv_jets13 = 0;
  Minv_jets14 = 0;
  Minv_jets23 = 0;
  Minv_jets24 = 0;
  Minv_jets34 = 0;
  //DR between b-quarks
  bQuark_DR1 = -999;
  bQuark_DR2 = -999;
  //DR between tagged jets
  DR_jets12 = 0;
  DR_jets13 = 0;
  DR_jets14 = 0;
  DR_jets23 = 0;
  DR_jets24 = 0;
  DR_jets34 = 0;

  //CUTS!!!!
  if (jet_n < 4) return kTRUE; //only runs for events with four or more jets

  std::vector<int> untaggedJetIndexes;
  std::vector<int> taggedJetIndexes;
  for(size_t jetIdx = 0; jetIdx != jet_n; jetIdx++)
  {
    if(jet_isBJet_MV1->at(jetIdx) != 1)
    {
      untaggedJetIndexes.push_back(jetIdx);
    } else
    {
      taggedJetIndexes.push_back(jetIdx);
    }
  }

  if (taggedJetIndexes.size() < 2) return kTRUE; 

  // increment event counter
  m_evts++;


  //loop over truth
  for (unsigned i = 0; i< truth_n ; i++)
  {

    //find b and bbar quarks
    if ((*truth_pdgId)[i] == 5)
    {
      b_index->push_back(i);
      truthbQuarklog << "Position of truth b-quark: " << i << endl;
      truthbQuarklog << "Truth eta: " << (*truth_eta)[i] << ", truth phi: " << (*truth_phi)[i] << endl;
      num_bquarks++;
      //fill pt and eta histograms?
    }
    else if ((*truth_pdgId)[i] == -5)
    {
      bbar_index->push_back(i);
      truthbQuarklog << "Position of truth bbar-quark: " << i << endl;
      truthbQuarklog << "Truth eta: " << (*truth_eta)[i] << ", truth phi: " << (*truth_phi)[i] << endl;
      num_bbarquarks++;
      //fill pt and eta histograms?
    }

    //Find top quarks
    if (fabs((*truth_pdgId)[i]) == 6)
    {
      m_histograms->find1DHisto("h_Top_pT")->Fill((*truth_pt)[i]/1000);
    }

    //Find Higgs
    if ((*truth_pdgId)[i] == 25)
    {
      higgs_index = i;
      higgslog << "Position of true Higgs: " << i << endl;
      higgslog << "Eta: " << (*truth_eta)[i] << ", phi: " << (*truth_phi)[i] << endl;
      num_higgs++;
    }
  } //end of looping over truth

  // Get Invariant mass of b and b-bar
  // Find DR between b and bbar
  //std::cout << " == DEBUG == " << std::endl;
  int nBQuarks = b_index->size();
  int nBBarQuarks = bbar_index->size();

  std::cout << "Size of b_index :: " << nBQuarks << std::endl;
  std::cout << "Size of bbar_index :: " << nBBarQuarks << std::endl;

  invariantMasslog << "Information about b quarks: " << endl;

  if(nBQuarks == 0 || nBBarQuarks == 0)
  {
    std::cout << "No truth b or b-bar quarks in the event" << std::endl;
  }
  else if (nBQuarks > 0 && nBBarQuarks > 0)
  {
    int b_index_element = b_index->at(0);
    int bbar_index_element = bbar_index->at(0);

    std::cout << "Grabbing element " << b_index_element << " from b_index" << std::endl;
    std::cout << "Grabbing element " << bbar_index_element << " from bbar_index" << std::endl;

    std::cout << "There are " << truth_pt->size() << " truth particles." << std::endl;

    double b_pt = truth_pt->at(b_index_element);
    double b_eta = truth_eta->at(b_index_element);
    double b_phi = truth_phi->at(b_index_element);

    double b_px = getPx(b_pt, b_phi);
    double b_py = getPy(b_pt, b_phi);
    double b_pz = getPz(b_pt, b_eta);

    double bbar_pt = truth_pt->at(bbar_index_element);
    double bbar_eta = truth_eta->at(bbar_index_element);
    double bbar_phi = truth_phi->at(bbar_index_element);

    double bbar_px = getPx(bbar_pt, bbar_phi);
    double bbar_py = getPy(bbar_pt, bbar_phi);
    double bbar_pz = getPz(bbar_pt, bbar_eta);

    double bQuarks_px = b_px + bbar_px;
    double bQuarks_py = b_py + bbar_py;
    double bQuarks_pz = b_pz + bbar_pz;
    double bQuarks_E = truth_E->at(b_index_element) + truth_E->at(bbar_index_element);

    bQuark_Minv = getMinv(bQuarks_E, bQuarks_px, bQuarks_py, bQuarks_pz);

    invariantMasslog << "First pair of b-quarks: " << endl;
    invariantMasslog << "Energy = " << bQuarks_E << ", px = " << bQuarks_px << ", py = " << bQuarks_py << ", pz = " << bQuarks_pz << endl;
    invariantMasslog << "Invariant Mass of first-found pair of b-quarks = " << bQuark_Minv << endl << " " << endl; 

    // Calculating DR between first pair of b-quarks
    bQuark_DR1 = getDeltaR(b_phi, bbar_phi, b_eta, bbar_eta);
    bQuarkDRlog << "First pair of b-quarks: " << endl;
    bQuarkDRlog << "b-quark: phi = " << b_phi << ", eta = " << b_eta << endl;
    bQuarkDRlog << "bbar: phi = " << bbar_phi << ", eta = " << bbar_eta << endl;
    bQuarkDRlog << "Delta R = " << bQuark_DR1 << endl << " " << endl;
    

  }
  if (nBQuarks == 2 && nBBarQuarks == 2)
  {
    int b_index_element = b_index->at(1);
    int bbar_index_element = bbar_index->at(1);

    std::cout << "Grabbing element " << b_index_element << " from b_index" << std::endl;
    std::cout << "Grabbing element " << bbar_index_element << " from bbar_index" << std::endl;

    std::cout << "There are " << truth_pt->size() << " truth particles." << std::endl;

    double b_pt = truth_pt->at(b_index_element);
    double b_eta = truth_eta->at(b_index_element);
    double b_phi = truth_phi->at(b_index_element);

    double b_px = getPx(b_pt, b_phi);
    double b_py = getPy(b_pt, b_phi);
    double b_pz = getPz(b_pt, b_eta);

    double bbar_pt = truth_pt->at(bbar_index_element);
    double bbar_eta = truth_eta->at(bbar_index_element);
    double bbar_phi = truth_phi->at(bbar_index_element);

    double bbar_px = getPx(bbar_pt, bbar_phi);
    double bbar_py = getPy(bbar_pt, bbar_phi);
    double bbar_pz = getPz(bbar_pt, bbar_eta);

    double bQuarks_px = b_px + bbar_px;
    double bQuarks_py = b_py + bbar_py;
    double bQuarks_pz = b_pz + bbar_pz;
    double bQuarks_E = truth_E->at(b_index_element) + truth_E->at(bbar_index_element);

    bQuark2_Minv = getMinv(bQuarks_E, bQuarks_px, bQuarks_py, bQuarks_pz);

    invariantMasslog << "Second pair of b-quarks: " << endl;
    invariantMasslog << "Energy = " << bQuarks_E << ", px = " << bQuarks_px << ", py = " << bQuarks_py << ", pz = " << bQuarks_pz << endl;
    invariantMasslog << "Invariant Mass of second-found pair of b-quarks = " << bQuark2_Minv << endl << " " << endl;

    // Calculating DR between first pair of b-quarks
    bQuark_DR2 = getDeltaR(b_phi, bbar_phi, b_eta, bbar_eta);
    bQuarkDRlog << "Second pair of b-quarks: " << endl;
    bQuarkDRlog << "b-quark: phi = " << b_phi << ", eta = " << b_eta << endl;
    bQuarkDRlog << "bbar: phi = " << bbar_phi << ", eta = " << bbar_eta << endl;
    bQuarkDRlog << "Delta R = " << bQuark_DR2 << endl << " " << endl;
    

    // testing: what about mass of 0 + 1 and 1+ 0??
    /*b_index_element = b_index->at(0);
    bbar_index_element = bbar_index->at(1);

    b_pt = truth_pt->at(b_index_element);
    b_eta = truth_eta->at(b_index_element);
    b_phi = truth_phi->at(b_index_element);

    b_px = getPx(b_pt, b_phi);
    b_py = getPy(b_pt, b_phi);
    b_pz = getPz(b_pt, b_eta);

    bbar_pt = truth_pt->at(bbar_index_element);
    bbar_eta = truth_eta->at(bbar_index_element);
    bbar_phi = truth_phi->at(bbar_index_element);

    bbar_px = getPx(bbar_pt, bbar_phi);
    bbar_py = getPy(bbar_pt, bbar_phi);
    bbar_pz = getPz(bbar_pt, bbar_eta);

    bQuarks_px = b_px + bbar_px;
    bQuarks_py = b_py + bbar_py;
    bQuarks_pz = b_pz + bbar_pz;
    bQuarks_E = truth_E->at(b_index_element) + truth_E->at(bbar_index_element);

    bQuark2_Minv = getMinv(bQuarks_E, bQuarks_px, bQuarks_py, bQuarks_pz);

    invariantMasslog << " 0 + 1pair of b-quarks: " << endl;
    invariantMasslog << "Energy = " << bQuarks_E << ", px = " << bQuarks_px << ", py = " << bQuarks_py << ", pz = " << bQuarks_pz << endl;
    invariantMasslog << "Invariant Mass of 0 + 1 pair of b-quarks = " << bQuark2_Minv << endl << " " << endl;*/

    // now: 1+0

    /*b_index_element = b_index->at(1);
    bbar_index_element = bbar_index->at(0); 

    b_pt = truth_pt->at(b_index_element);
    b_eta = truth_eta->at(b_index_element);
    b_phi = truth_phi->at(b_index_element);

    b_px = getPx(b_pt, b_phi);
    b_py = getPy(b_pt, b_phi);
    b_pz = getPz(b_pt, b_eta);

    bbar_pt = truth_pt->at(bbar_index_element);
    bbar_eta = truth_eta->at(bbar_index_element);
    bbar_phi = truth_phi->at(bbar_index_element);

    bbar_px = getPx(bbar_pt, bbar_phi);
    bbar_py = getPy(bbar_pt, bbar_phi);
    bbar_pz = getPz(bbar_pt, bbar_eta);

    bQuarks_px = b_px + bbar_px;
    bQuarks_py = b_py + bbar_py;
    bQuarks_pz = b_pz + bbar_pz;
    bQuarks_E = truth_E->at(b_index_element) + truth_E->at(bbar_index_element);

    bQuark2_Minv = getMinv(bQuarks_E, bQuarks_px, bQuarks_py, bQuarks_pz);

    invariantMasslog << " 1+ 0 pair of b-quarks: " << endl;
    invariantMasslog << "Energy = " << bQuarks_E << ", px = " << bQuarks_px << ", py = " << bQuarks_py << ", pz = " << bQuarks_pz << endl;
    invariantMasslog << "Invariant Mass of 1+ 0 pair of b-quarks = " << bQuark2_Minv << endl << " " << endl; 
    */
  }
  bQuarkDRlog << " " << endl;
  
  //fill Histogram
  m_histograms->find1DHisto("h_bQuark_Minv")->Fill(bQuark_Minv);
  m_histograms->find1DHisto("h_bQuark_Minv2")->Fill(bQuark2_Minv); //need to add different combinations of bQuarks in ttH

  //After inspection of these histograms assume first pair of b-quarks is from the Higgs
  //fill pT and eta histograms
  if ((nBQuarks < 2 && nBQuarks != 0) && (nBBarQuarks < 2 && nBBarQuarks != 0)) //ttbar with condition for having a b in event
  {
    m_histograms->find1DHisto("h_truebQuarkFromTop_pT")->Fill(truth_pt->at(b_index->at(0))/1000);
    m_histograms->find1DHisto("h_truebQuarkFromTop_pT")->Fill(truth_pt->at(bbar_index->at(0))/1000);
    m_histograms->find1DHisto("h_truebQuarkFromTop_eta")->Fill(truth_eta->at(b_index->at(0)));
    m_histograms->find1DHisto("h_truebQuarkFromTop_eta")->Fill(truth_eta->at(bbar_index->at(0)));

    //Fill DR histogram
    m_histograms->find1DHisto("h_bQuarkDR_fromTop")->Fill(bQuark_DR1);
  }
  else if (nBQuarks == 2 && nBBarQuarks == 2) //ttH
  {
    //higgs bquarks
    m_histograms->find1DHisto("h_truebQuarkFromHiggs_pT")->Fill(truth_pt->at(b_index->at(0))/1000);
    m_histograms->find1DHisto("h_truebQuarkFromHiggs_pT")->Fill(truth_pt->at(bbar_index->at(0))/1000);
    m_histograms->find1DHisto("h_truebQuarkFromHiggs_eta")->Fill(truth_eta->at(b_index->at(0)));
    m_histograms->find1DHisto("h_truebQuarkFromHiggs_eta")->Fill(truth_eta->at(bbar_index->at(0)));
    //top bquarks
    m_histograms->find1DHisto("h_truebQuarkFromTop_pT")->Fill(truth_pt->at(b_index->at(1))/1000);
    m_histograms->find1DHisto("h_truebQuarkFromTop_pT")->Fill(truth_pt->at(bbar_index->at(1))/1000);
    m_histograms->find1DHisto("h_truebQuarkFromTop_eta")->Fill(truth_eta->at(b_index->at(1)));
    m_histograms->find1DHisto("h_truebQuarkFromTop_eta")->Fill(truth_eta->at(bbar_index->at(1)));

    //Fill DR histogram
    m_histograms->find1DHisto("h_bQuarkDR_fromHiggs")->Fill(bQuark_DR1);
    m_histograms->find1DHisto("h_bQuarkDR_fromTop")->Fill(bQuark_DR2);
  }

  m_histograms->find1DHisto("h_Higgs_eta")->Fill((*truth_eta)[higgs_index]);
  m_histograms->find1DHisto("h_Higgs_pT")->Fill((*truth_pt)[higgs_index]/1000);


  //loop over JETS
  for (unsigned int i=0; i<jet_n; i++)
  {
    // Reset flags
    jetisClose = false;
    jet_counter++;
    event_jetCounter++;
    
    //plot jet pt and jet eta of all jets
    m_histograms->find1DHisto("jetPt")->Fill((*jet_pt)[i]/1000);
    m_histograms->find1DHisto("jetEta")->Fill((*jet_eta)[i]);

    //set DR values to be above 0.4 - next part of code will only grab DR value if b/bbar is present
    double jet_bquark1_DR = 0.5;
    double jet_bquark2_DR = 0.5;
    double jet_bbarquark1_DR = 0.5;
    double jet_bbarquark2_DR = 0.5;
    //Find DR between jet and b/bbar quark
    if (b_index->size() > 0) 
    { 
      jet_bquark1_DR = getDeltaR((*jet_phi)[i], (*truth_phi)[b_index->at(0)], (*jet_eta)[i], (*truth_eta)[b_index->at(0)]);
      jetQuarkDRlog << "DR between jet and b quark = " << i << " " << jet_bquark1_DR << endl;
    }
    if (bbar_index->size() > 0)
    {
      jet_bbarquark1_DR = getDeltaR((*jet_phi)[i], (*truth_phi)[bbar_index->at(0)], (*jet_eta)[i], (*truth_eta)[bbar_index->at(0)]);
      jetQuarkDRlog << "DR between jet and bbar = " << i << " " << jet_bbarquark1_DR << endl;
    }
    if (b_index->size() > 1) 
    { 
      jet_bquark2_DR = getDeltaR((*jet_phi)[i], (*truth_phi)[b_index->at(1)], (*jet_eta)[i], (*truth_eta)[b_index->at(1)]);
      jetQuarkDRlog << "DR between jet and b quark = " << i << " " << jet_bquark2_DR << endl;
    }
    if (bbar_index->size() > 1)
    {
      jet_bbarquark2_DR = getDeltaR((*jet_phi)[i], (*truth_phi)[bbar_index->at(1)], (*jet_eta)[i], (*truth_eta)[bbar_index->at(1)]);
      jetQuarkDRlog << "DR between jet and bbar = " << i << " " << jet_bbarquark1_DR << endl;
    }
    
    //check if these DR values are below 0.4 and find the closest jet
    if (jet_bquark1_DR < jet_bbarquark1_DR)
    {
      if (jet_bquark1_DR < bQuark1_jet_DR_test) //for first case <0.4
      {
        closestJet_b1 = i;
        bQuark1_jet_DR_test = jet_bquark1_DR;
        jetisClose = true;
      }
    }
    else if (jet_bquark1_DR > jet_bbarquark1_DR)
    {
      if (jet_bbarquark1_DR < bbar1_jet_DR_test) //for first case <0.4 
      {
        closestJet_bbar1 = i;
        bbar1_jet_DR_test = jet_bbarquark1_DR;
        jetisClose = true;
      } 
    }
    if (jet_bquark2_DR < jet_bbarquark2_DR)
    {
      if (jet_bquark2_DR < bQuark2_jet_DR_test) //for first case <0.4
      {
        closestJet_b2 = i;
        bQuark2_jet_DR_test = jet_bquark2_DR;
        jetisClose = true;
      }
    }
    else if (jet_bquark2_DR > jet_bbarquark2_DR)
    {
      if (jet_bbarquark2_DR < bbar2_jet_DR_test) //for first case <0.4 
      {
        closestJet_bbar2 = i;
        bbar2_jet_DR_test = jet_bbarquark2_DR;
        jetisClose = true;
      } 
    }
    else
    {
      jetisClose = false;
    }//end of checking if jets are close to b-quarks
    
    //check if jet is btagged
    if (jet_isBJet_MV1->at(i))
    {
      bJet_counter++;
      total_bJets++;
    }

    jetQuarkDRlog << " " << endl;
  } //end of looping over jets
  //jetQuarkDRlog << " After the loop: closestJet_b: " << closestJet_b << " closestJet_bbar: " << closestJet_bbar << " numFar: " << numFarJets << endl;

  closeJetslog << "Number of jets in event = " << event_jetCounter << endl;
  closeJetslog << "Index of jet closest to: " << endl;
  closeJetslog << "First b quark: " << closestJet_b1 << endl;
  closeJetslog << "Second b quark: " << closestJet_b2 << endl;
  closeJetslog << "First bbar quark: " << closestJet_bbar1 << endl;
  closeJetslog << "Second bbar quark: " << closestJet_bbar2 << endl << " " << endl;
  
  // Start filling histograms here
  if (closestJet_b1 != -999) //jet is close to b1
  {
    numCloseJets_b++;

    //check if MV1 tagged
    if(jet_isBJet_MV1->at(closestJet_b1) != false)
    {
     closeJets_bTagged++;

      //fill histograms with different information about these jets
      m_histograms->find1DHisto("h_closeJets_bTagged_pT")->Fill((*jet_pt)[closestJet_b1]/1000);
      m_histograms->find1DHisto("h_closeJets_bTagged_eta")->Fill((*jet_eta)[closestJet_b1]);
      m_histograms->find1DHisto("h_closeJets_bTagged_emFrac")->Fill((*jet_emFrac)[closestJet_b1]);
      m_histograms->find1DHisto("h_closeJets_bTagged_nTracks")->Fill((*jet_nTracks)[closestJet_b1]);  
    } 
    else
    {
      num_closeJetsb_untagged++;
      //fill histograms with different information about these jets
      m_histograms->find1DHisto("h_closeJets_pT")->Fill((*jet_pt)[closestJet_b1]/1000);
      m_histograms->find1DHisto("h_closeJets_eta")->Fill((*jet_eta)[closestJet_b1]);
      m_histograms->find1DHisto("h_closeJets_emFrac")->Fill((*jet_emFrac)[closestJet_b1]);
      m_histograms->find1DHisto("h_closeJets_nTracks")->Fill((*jet_nTracks)[closestJet_b1]);
    }
  } // End if jet close to b1

  if (closestJet_b2 != -999) //jet is close to b2
  {
    numCloseJets_b++;

    //check if MV1 tagged
    if(jet_isBJet_MV1->at(closestJet_b2) != false)
    {
     closeJets_bTagged++;

      //fill histograms with different information about these jets
      m_histograms->find1DHisto("h_closeJets_bTagged_pT")->Fill((*jet_pt)[closestJet_b2]/1000);
      m_histograms->find1DHisto("h_closeJets_bTagged_eta")->Fill((*jet_eta)[closestJet_b2]);
      m_histograms->find1DHisto("h_closeJets_bTagged_emFrac")->Fill((*jet_emFrac)[closestJet_b2]);
      m_histograms->find1DHisto("h_closeJets_bTagged_nTracks")->Fill((*jet_nTracks)[closestJet_b2]);  
    } 
    else
    {
      num_closeJetsb_untagged++;
      //fill histograms with different information about these jets
      m_histograms->find1DHisto("h_closeJets_pT")->Fill((*jet_pt)[closestJet_b2]/1000);
      m_histograms->find1DHisto("h_closeJets_eta")->Fill((*jet_eta)[closestJet_b2]);
      m_histograms->find1DHisto("h_closeJets_emFrac")->Fill((*jet_emFrac)[closestJet_b2]);
      m_histograms->find1DHisto("h_closeJets_nTracks")->Fill((*jet_nTracks)[closestJet_b2]);
    }
  } // End if jet close to b2


  if (closestJet_bbar1 != -999) //jet is close to bbar1
  {
    numCloseJets_bbar++;

    //check if MV1 tagged
    if(jet_isBJet_MV1->at(closestJet_bbar1) != false)
    {
      closeJets_bTagged++;
    
      //fill histograms with different information about these jets
      m_histograms->find1DHisto("h_closeJets_bTagged_pT")->Fill((*jet_pt)[closestJet_bbar1]/1000);
      m_histograms->find1DHisto("h_closeJets_bTagged_eta")->Fill((*jet_eta)[closestJet_bbar1]);
      m_histograms->find1DHisto("h_closeJets_bTagged_emFrac")->Fill((*jet_emFrac)[closestJet_bbar1]);
      m_histograms->find1DHisto("h_closeJets_bTagged_nTracks")->Fill((*jet_nTracks)[closestJet_bbar1]);
    } 
    else
    {
      num_closeJetsbbar_untagged++;
      //fill histograms with different information about these jets
      m_histograms->find1DHisto("h_closeJets_pT")->Fill((*jet_pt)[closestJet_bbar1]/1000);
      m_histograms->find1DHisto("h_closeJets_eta")->Fill((*jet_eta)[closestJet_bbar1]);
      m_histograms->find1DHisto("h_closeJets_emFrac")->Fill((*jet_emFrac)[closestJet_bbar1]);
      m_histograms->find1DHisto("h_closeJets_nTracks")->Fill((*jet_nTracks)[closestJet_bbar1]);
    }
  } // End if jet is close to b-bar

  if (closestJet_bbar2 != -999) //jet is close to bbar2
  {
    numCloseJets_bbar++;

    //check if MV1 tagged
    if(jet_isBJet_MV1->at(closestJet_bbar2) != false)
    {
      closeJets_bTagged++;
    
      //fill histograms with different information about these jets
      m_histograms->find1DHisto("h_closeJets_bTagged_pT")->Fill((*jet_pt)[closestJet_bbar2]/1000);
      m_histograms->find1DHisto("h_closeJets_bTagged_eta")->Fill((*jet_eta)[closestJet_bbar2]);
      m_histograms->find1DHisto("h_closeJets_bTagged_emFrac")->Fill((*jet_emFrac)[closestJet_bbar2]);
      m_histograms->find1DHisto("h_closeJets_bTagged_nTracks")->Fill((*jet_nTracks)[closestJet_bbar2]);
    } 
    else
    {
      num_closeJetsbbar_untagged++;
      //fill histograms with different information about these jets
      m_histograms->find1DHisto("h_closeJets_pT")->Fill((*jet_pt)[closestJet_bbar2]/1000);
      m_histograms->find1DHisto("h_closeJets_eta")->Fill((*jet_eta)[closestJet_bbar2]);
      m_histograms->find1DHisto("h_closeJets_emFrac")->Fill((*jet_emFrac)[closestJet_bbar2]);
      m_histograms->find1DHisto("h_closeJets_nTracks")->Fill((*jet_nTracks)[closestJet_bbar2]);
    }
  } // End if jet is close to b-bar

  // Loop over jets again to fill histograms for jets which are not close to b-quarks
  for (unsigned int i = 0; i<jet_n; i++)
  {
    //if(i == closestJet_b || i == closestJet_bbar) continue; before adding vectors
    if (closestJet_b1 == i || closestJet_bbar1 == i) continue; //change this?!
    else if (closestJet_b2 == i || closestJet_bbar2 == i) continue; 

    // print here: is this jet really a far jet?
    closeJetslog << "Index of 'Far Jet' " << i << endl;
    closeJetslog << "closestJet_b1 = " << closestJet_b1 << endl;
    closeJetslog << "closestJet_b2 = " << closestJet_b1 << endl;
    closeJetslog << "closestJet_bbar1 = " << closestJet_bbar1 << endl;
    closeJetslog << "closestJet_bbar2 = " << closestJet_bbar2 << endl << " " << endl;

    numFarJets_total++;
    //jetQuarkDRlog << " fill far jet histo: " << i << endl;
   
    //check if MV1 tagged
    if (jet_isBJet_MV1->at(i))//checks for b-tagged jets
    {
      farJets_bTagged++;

      //fill histograms with information about these jets, far and b-tagged
      m_histograms->find1DHisto("h_farJets_bTagged_pT")->Fill((*jet_pt)[i]/1000);
      m_histograms->find1DHisto("h_farJets_bTagged_eta")->Fill((*jet_eta)[i]);
      m_histograms->find1DHisto("h_farJets_bTagged_emFrac")->Fill((*jet_emFrac)[i]);
      m_histograms->find1DHisto("h_farJets_bTagged_nTracks")->Fill((*jet_nTracks)[i]);
    } 
    else
    {
      numFarJets++;
       //fill histograms with different information about these jets, far and not b-tagged
      m_histograms->find1DHisto("h_farJets_pT")->Fill((*jet_pt)[i]/1000);
      m_histograms->find1DHisto("h_farJets_eta")->Fill((*jet_eta)[i]);
      m_histograms->find1DHisto("h_farJets_emFrac")->Fill((*jet_emFrac)[i]);
      m_histograms->find1DHisto("h_farJets_nTracks")->Fill((*jet_nTracks)[i]);
    }
  } //end of second jet loop


  //loop over truth information for finding information about b-quarks that have no jets near them
  if (closestJet_b1 == -999 && closestJet_bbar1 == -999 && closestJet_b2 == -999 && closestJet_bbar2 == -999)
  {
    for (unsigned i = 0; i< truth_n ; i++)
    {
      if (fabs((*truth_pdgId)[i]) == 5)
      {
        num_farbQuarks++;
        m_histograms->find1DHisto("h_farQuarks_TruepT")->Fill((*truth_pt)[i]/1000);
        m_histograms->find1DHisto("h_farQuarks_TrueEta")->Fill((*truth_eta)[i]);

        //find b quarks with pT greater than 25 GeV
        if ((*truth_pt)[i]/1000 > 25)
        {
          bQuarkPT_inRange++;
        }
        //find b quarks with eta between -2.5 to 2.5
        if (fabs((*truth_eta)[i]) < 2.5)
        {
          bQuarkEta_inRange++;
          if ((*truth_pt)[i]/1000 > 25)
          {
            //b quarks that should have been reconstructed
            bQuarkEtaPT_inRange++;
          }
        }
      }
    }
  } //end of second loop over truth


  //find DR and invariant mass between tagged jets
  if (taggedJetIndexes.size() > 1 && taggedJetIndexes.size() < 3) //ttbar
  {
    size_t firstJetIndex = taggedJetIndexes.at(0);
    size_t secondJetIndex = taggedJetIndexes.at(1);

    //for finding DR between tagged jets
    double jet1_eta = jet_eta->at(firstJetIndex);
    double jet1_phi = jet_phi->at(firstJetIndex);

    double jet2_eta = jet_eta->at(secondJetIndex);
    double jet2_phi = jet_phi->at(secondJetIndex);    

    DR_jets12 = getDeltaR(jet1_phi, jet2_phi, jet1_eta, jet2_eta);
    m_histograms->find1DHisto("h_taggedJets12_DR")->Fill(DR_jets12);

    //finding Invariant mass
    double jet1_px = getPx(jet_pt->at(firstJetIndex), jet_phi->at(firstJetIndex));
    double jet1_py = getPy(jet_pt->at(firstJetIndex), jet_phi->at(firstJetIndex));
    double jet1_pz = getPz(jet_pt->at(firstJetIndex), jet_eta->at(firstJetIndex));

    double jet2_px = getPx(jet_pt->at(secondJetIndex), jet_phi->at(secondJetIndex));
    double jet2_py = getPy(jet_pt->at(secondJetIndex), jet_phi->at(secondJetIndex));
    double jet2_pz = getPz(jet_pt->at(secondJetIndex), jet_eta->at(secondJetIndex));

    double jets12_px = jet1_px + jet2_px;
    double jets12_py = jet1_py + jet2_py;
    double jets12_pz = jet1_pz + jet2_pz;
    double jets12_E = (jet_E->at(firstJetIndex) + jet_E->at(secondJetIndex));

    Minv_jets12 = getMinv(jets12_E, jets12_px, jets12_py, jets12_pz);

    invariantMasslog << "First and second tagged jet: " << endl;
    invariantMasslog << "Energy = " << jets12_E << ", px = " << jets12_px << ", py = " << jets12_py << ", pz = " << jets12_pz << endl;
    invariantMasslog << "Invariant Mass of first and second tagged jets = " << Minv_jets12 << endl << " " << endl;

    //fill histogram
    m_histograms->find1DHisto("h_taggedJets12_Minv")->Fill(Minv_jets12);        
  }
  if (taggedJetIndexes.size() > 3) //ttH
  {
    invariantMasslog << " " << endl << "Information about tagged jets: " << endl;

    size_t firstJetIndex = taggedJetIndexes.at(0);
    size_t secondJetIndex = taggedJetIndexes.at(1);
    size_t thirdJetIndex = taggedJetIndexes.at(2);
    size_t fourthJetIndex = taggedJetIndexes.at(3);

    //for finding DR between tagged jets
    double jet1_eta = jet_eta->at(firstJetIndex);
    double jet1_phi = jet_phi->at(firstJetIndex);

    double jet2_eta = jet_eta->at(secondJetIndex);
    double jet2_phi = jet_phi->at(secondJetIndex);

    double jet3_eta = jet_eta->at(thirdJetIndex);
    double jet3_phi = jet_phi->at(thirdJetIndex);

    double jet4_eta = jet_eta->at(fourthJetIndex);
    double jet4_phi = jet_phi->at(fourthJetIndex);

    DR_jets12 = getDeltaR(jet1_phi, jet2_phi, jet1_eta, jet2_eta);
    DR_jets13 = getDeltaR(jet1_phi, jet3_phi, jet1_eta, jet3_eta);
    DR_jets14 = getDeltaR(jet1_phi, jet4_phi, jet1_eta, jet4_eta);
    DR_jets23 = getDeltaR(jet2_phi, jet3_phi, jet2_eta, jet3_eta);
    DR_jets24 = getDeltaR(jet2_phi, jet4_phi, jet2_eta, jet4_eta);
    DR_jets34 = getDeltaR(jet3_phi, jet4_phi, jet3_eta, jet4_eta);

    //fill Histograms

    m_histograms->find1DHisto("h_taggedJets12_DR")->Fill(DR_jets12);
    m_histograms->find1DHisto("h_taggedJets13_DR")->Fill(DR_jets13);
    m_histograms->find1DHisto("h_taggedJets14_DR")->Fill(DR_jets14);
    m_histograms->find1DHisto("h_taggedJets23_DR")->Fill(DR_jets23);
    m_histograms->find1DHisto("h_taggedJets24_DR")->Fill(DR_jets24);
    m_histograms->find1DHisto("h_taggedJets34_DR")->Fill(DR_jets34);


    //finding Invariant mass
    double jet1_px = getPx(jet_pt->at(firstJetIndex), jet_phi->at(firstJetIndex));
    double jet1_py = getPy(jet_pt->at(firstJetIndex), jet_phi->at(firstJetIndex));
    double jet1_pz = getPz(jet_pt->at(firstJetIndex), jet_eta->at(firstJetIndex));

    double jet2_px = getPx(jet_pt->at(secondJetIndex), jet_phi->at(secondJetIndex));
    double jet2_py = getPy(jet_pt->at(secondJetIndex), jet_phi->at(secondJetIndex));
    double jet2_pz = getPz(jet_pt->at(secondJetIndex), jet_eta->at(secondJetIndex));

    double jet3_px = getPx(jet_pt->at(thirdJetIndex), jet_phi->at(thirdJetIndex));
    double jet3_py = getPy(jet_pt->at(thirdJetIndex), jet_phi->at(thirdJetIndex));
    double jet3_pz = getPz(jet_pt->at(thirdJetIndex), jet_eta->at(thirdJetIndex));

    double jet4_px = getPx(jet_pt->at(fourthJetIndex), jet_phi->at(fourthJetIndex));
    double jet4_py = getPy(jet_pt->at(fourthJetIndex), jet_phi->at(fourthJetIndex));
    double jet4_pz = getPz(jet_pt->at(fourthJetIndex), jet_eta->at(fourthJetIndex));

    //first and second jet
    double jets12_px = jet1_px + jet2_px;
    double jets12_py = jet1_py + jet2_py;
    double jets12_pz = jet1_pz + jet2_pz;
    double jets12_E = (jet_E->at(firstJetIndex) + jet_E->at(secondJetIndex));

    Minv_jets12 = getMinv(jets12_E, jets12_px, jets12_py, jets12_pz);

    invariantMasslog << "First and second tagged jet: " << endl;
    invariantMasslog << "Energy = " << jets12_E << ", px = " << jets12_px << ", py = " << jets12_py << ", pz = " << jets12_pz << endl;
    invariantMasslog << "Invariant Mass of first and second tagged jets = " << Minv_jets12 << endl << " " << endl;

    //fill histogram
    m_histograms->find1DHisto("h_taggedJets12_Minv")->Fill(Minv_jets12);

  
    //first and third jet
    double jets13_px = jet1_px + jet3_px;
    double jets13_py = jet1_py + jet3_py;
    double jets13_pz = jet1_pz + jet3_pz;
    double jets13_E = (jet_E->at(firstJetIndex) + jet_E->at(thirdJetIndex));

    Minv_jets13 = getMinv(jets13_E, jets13_px, jets13_py, jets13_pz);

    invariantMasslog << "First and third tagged jet: " << endl;
    invariantMasslog << "Energy = " << jets13_E << ", px = " << jets13_px << ", py = " << jets13_py << ", pz = " << jets13_pz << endl;
    invariantMasslog << "Invariant Mass of first and third tagged jets = " << Minv_jets13 << endl << " " << endl;

    //fill histogram
    m_histograms->find1DHisto("h_taggedJets13_Minv")->Fill(Minv_jets13);


    //first and fourth jet
    double jets14_px = jet1_px + jet4_px;
    double jets14_py = jet1_py + jet4_py;
    double jets14_pz = jet1_pz + jet4_pz;
    double jets14_E = (jet_E->at(firstJetIndex) + jet_E->at(fourthJetIndex));

    Minv_jets14 = getMinv(jets14_E, jets14_px, jets14_py, jets14_pz);

    invariantMasslog << "First and fourth tagged jet: " << endl;
    invariantMasslog << "Energy = " << jets14_E << ", px = " << jets14_px << ", py = " << jets14_py << ", pz = " << jets14_pz << endl;
    invariantMasslog << "Invariant Mass of first and fourth tagged jets = " << Minv_jets14 << endl << " " << endl;

    //fill histogram
    m_histograms->find1DHisto("h_taggedJets14_Minv")->Fill(Minv_jets14);


    //second and third jet
    double jets23_px = jet2_px + jet3_px;
    double jets23_py = jet2_py + jet3_py;
    double jets23_pz = jet2_pz + jet3_pz;
    double jets23_E = (jet_E->at(secondJetIndex) + jet_E->at(thirdJetIndex));

    Minv_jets23 = getMinv(jets23_E, jets23_px, jets23_py, jets23_pz);

    invariantMasslog << "Second and third tagged jet: " << endl;
    invariantMasslog << "Energy = " << jets23_E << ", px = " << jets23_px << ", py = " << jets23_py << ", pz = " << jets23_pz << endl;
    invariantMasslog << "Invariant Mass of second and third tagged jets = " << Minv_jets23 << endl << " " << endl;

    //fill histogram
    m_histograms->find1DHisto("h_taggedJets23_Minv")->Fill(Minv_jets23);


    //second and fourth jet
    double jets24_px = jet2_px + jet4_px;
    double jets24_py = jet2_py + jet4_py;
    double jets24_pz = jet2_pz + jet4_pz;
    double jets24_E = (jet_E->at(secondJetIndex) + jet_E->at(fourthJetIndex));

    Minv_jets24 = getMinv(jets24_E, jets24_px, jets24_py, jets24_pz);

    invariantMasslog << "Second and fourth tagged jet: " << endl;
    invariantMasslog << "Energy = " << jets24_E << ", px = " << jets24_px << ", py = " << jets24_py << ", pz = " << jets24_pz << endl;
    invariantMasslog << "Invariant Mass of second and fourth tagged jets = " << Minv_jets24 << endl << " " << endl;

    //fill histogram
    m_histograms->find1DHisto("h_taggedJets24_Minv")->Fill(Minv_jets24);


    //third and fourth jet
    double jets34_px = jet3_px + jet4_px;
    double jets34_py = jet3_py + jet4_py;
    double jets34_pz = jet3_pz + jet4_pz;
    double jets34_E = (jet_E->at(thirdJetIndex) + jet_E->at(fourthJetIndex));

    Minv_jets34 = getMinv(jets34_E, jets34_px, jets34_py, jets34_pz);

    invariantMasslog << "Third and fourth tagged jet: " << endl;
    invariantMasslog << "Energy = " << jets34_E << ", px = " << jets34_px << ", py = " << jets34_py << ", pz = " << jets34_pz << endl;
    invariantMasslog << "Invariant Mass of third and fourth tagged jets = " << Minv_jets34 << endl << " " << endl;

    //fill histogram
    m_histograms->find1DHisto("h_taggedJets34_Minv")->Fill(Minv_jets34);

  }
  

  // increment counters for how many MV1 tagged jets are in each event
  if (bJet_counter == 0)
  {
    zero_MV1++;
    m_histograms->find1DHisto("event_bTags")->Fill(0);
  }
  else if (bJet_counter == 1)
  {
    one_MV1++;
    m_histograms->find1DHisto("event_bTags")->Fill(1);
  }
  else if (bJet_counter == 2)
  {
    two_MV1++;
    m_histograms->find1DHisto("event_bTags")->Fill(2);
  }
  else if (bJet_counter == 3)
  {
    three_MV1++;
    m_histograms->find1DHisto("event_bTags")->Fill(3);
  }
  else if (bJet_counter == 4)
  {
    four_MV1++;
    m_histograms->find1DHisto("event_bTags")->Fill(4);
  }
  else if (bJet_counter > 4)
  {
    fourPlus_MV1++;
    m_histograms->find1DHisto("event_bTags")->Fill(5);
  }

  truthbQuarklog << " " << endl << " " << endl;

  //fill histograms for lepton and neutrino
  m_histograms->find1DHisto("h_lepton_pT")->Fill((lep_pt)/1000);
  m_histograms->find1DHisto("h_MET_sumET")->Fill((met_sumet)/1000);

  return kTRUE;
}

//calculates DR when given the phis and etas of the two particles
double SMTttbarLplusJetsAnalysis::getDeltaR(double phi_1, double phi_2, double eta_1, double eta_2) {

 const double pi = 4.0*atan(1.0);
 float dPhi = fabs(phi_1 - phi_2);
 float dPhiWrapped = fabs(fabs(dPhi - pi) - pi);

 float dEta = fabs(eta_1 - eta_2);

 return(sqrt(dPhiWrapped * dPhiWrapped + dEta * dEta));

}

//Gets the different momentum components for calculating the invarint mass
double SMTttbarLplusJetsAnalysis::getPx(double pT, double phi)
{
  double px = (pT/1000)*cos(phi);
  return px;
}

double SMTttbarLplusJetsAnalysis::getPy(double pT, double phi)
{
  double py = (pT/1000)*sin(phi);
  return py;
}

double SMTttbarLplusJetsAnalysis::getPz(double pT, double eta)
{
  double pz = (pT/1000)*sinh(eta);
  return pz;
}

//calculated the invariant mass given the individual momentum components found above
double SMTttbarLplusJetsAnalysis::getMinv(double E, double px, double py, double pz)
{
  double Minv = sqrt(pow(E/1000, 2) - (px*px + py*py + pz*pz));
  return Minv;
}


void SMTttbarLplusJetsAnalysis::Terminate()
{


  //print information about b-tagged jets to log
  bJetTagslog << "Total number of events: " << m_evts << endl;
  bJetTagslog << "Total number of jets: " << jet_counter << endl;
  bJetTagslog << "Total number of b-tagged jets (using MV1): " << total_bJets << endl;
  bJetTagslog << "Number of events that have 0 MV1 tags: " << zero_MV1 << endl;
  bJetTagslog << "Number of events that have 1 MV1 tags: " << one_MV1 << endl;
  bJetTagslog << "Number of events that have 2 MV1 tags: " << two_MV1 << endl;
  bJetTagslog << "Number of events that have 3 MV1 tags: " << three_MV1 << endl;
  bJetTagslog << "Number of events that have 4 MV1 tags: " << four_MV1 << endl;
  bJetTagslog << "Number of events that have more than 4 MV1 tags: " << fourPlus_MV1 << endl;
  //fraction of these per event
  double jetFrac = jet_counter/m_evts;
  double jetMV1frac = total_bJets/m_evts;
  double zeroTagFrac = zero_MV1/m_evts;
  double oneTagFrac = one_MV1/m_evts;
  double twoTagFrac = two_MV1/m_evts;
  double threeTagFrac = three_MV1/m_evts;
  double fourTagFrac = four_MV1/m_evts;
  double fourPlusTagFrac = fourPlus_MV1/m_evts;
  bJetTagslog << " " << endl << "Fraction of jets per event: " << jetFrac << endl;
  bJetTagslog << "Fraction of tagged jets per event: " << jetMV1frac << endl;
  bJetTagslog << "Fraction of events that have 0 MV1 tagged jets: " << zeroTagFrac << endl;
  bJetTagslog << "Fraction of events that have 1 MV1 tagged jets: " << oneTagFrac << endl;
  bJetTagslog << "Fraction of events that have 2 MV1 tagged jets: " << twoTagFrac << endl;
  bJetTagslog << "Fraction of events that have 3 MV1 tagged jets: " << threeTagFrac << endl;
  bJetTagslog << "Fraction of events that have 4 MV1 tagged jets: " << fourTagFrac << endl;
  bJetTagslog << "Fraction of events that have >4 MV1 tagged jets: " << fourPlusTagFrac << endl;

  double bQuark_closeToJet = (numCloseJets_b+numCloseJets_bbar);
  double bQuark_closeToJet_untagged = (num_closeJetsb_untagged+num_closeJetsbbar_untagged);
  double reconstruction_efficiency = (bQuark_closeToJet/(num_bbarquarks+num_bquarks));
  double total_bQuarks = (num_bquarks+num_bbarquarks);
  double btagging_efficiency = (closeJets_bTagged/bQuark_closeToJet);
  double mistag_rate = (farJets_bTagged/total_bJets);
  double reconstruction_uncertainty = sqrt((reconstruction_efficiency*(1-reconstruction_efficiency))/(num_bbarquarks+num_bquarks));
  double btagging_uncertainty = sqrt((btagging_efficiency*(1-btagging_efficiency))/(bQuark_closeToJet));
  double mistag_uncertainty = sqrt((mistag_rate*(1-mistag_rate))/(total_bJets));

  jetsCloseToQuarkslog << "Total number of bquarks: " << total_bQuarks << endl;
  jetsCloseToQuarkslog << "Total number of tagged jets: " << total_bJets << endl;
  jetsCloseToQuarkslog << "Number of bquarks far from jets: " << num_farbQuarks << endl << " " << endl;
  jetsCloseToQuarkslog << "Total number of jets that are not close to a bquark (DR > 0.4): " << numFarJets_total << endl;
  jetsCloseToQuarkslog << "Number of jets that are not close to a bquark and are NOT b-tagged: " << numFarJets << endl;
  jetsCloseToQuarkslog << "Number of jets that are not close to a bquark but ARE b-tagged: " << farJets_bTagged << endl;
  jetsCloseToQuarkslog << "Mistag rate = " << mistag_rate << " +/- " << mistag_uncertainty << endl << "" << endl;
  jetsCloseToQuarkslog << "Total number of jets close to a bquark (Min value of DR, DR < 0.4): " << bQuark_closeToJet << endl;
  jetsCloseToQuarkslog << "Number of jets close to bquark that are not b-tagged: " << bQuark_closeToJet_untagged << endl;
  jetsCloseToQuarkslog << "Number of jets close to bquark that are b-tagged: " << closeJets_bTagged << endl;
  jetsCloseToQuarkslog << " " << endl << "Reconstruction efficiency = " << reconstruction_efficiency << " +/- " << reconstruction_uncertainty << endl;
  jetsCloseToQuarkslog << " " << endl << "b-tagging efficieny = " << btagging_efficiency << " +/- " << btagging_uncertainty << endl << " " << endl;


  bQuarkReconstructionlog << "Number of bQuarks with pT greater than 25GeV: " << bQuarkPT_inRange << endl;
  bQuarkReconstructionlog << "Number of bQuarks with Eta within -2.5 to 2.5: " << bQuarkEta_inRange << endl;
  bQuarkReconstructionlog << "Number of bQuarks that are within the allowed Eta range that have pT < 25GeV: " << bQuarkEtaPT_inRange << endl;

  higgslog << "" << endl;

  bJetTagslog.close();
  truthbQuarklog.close();
  jetQuarkDRlog.close();
  jetsCloseToQuarkslog.close();
  bQuarkReconstructionlog.close();
  closeJetslog.close();
  invariantMasslog.close();
  bQuarkDRlog.close();

  // Write all histograms and close root file
  if (m_isElectron) m_outRootFile = new TFile(m_outFileName.Data(),"RECREATE");
  else m_outRootFile = new TFile(m_outFileName.Data(),"RECREATE");
  
  m_histograms->write1DHistos();
  m_histograms->write2DHistos();
  m_outRootFile->Close();

  // This is where you print out at the end of the job all your global counters
  cout << "| ****** Terminating..." << endl;
  cout << "|-> Total Evts Processed = " << m_evts << endl;
}
