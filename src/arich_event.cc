#include "arich_event.hh"
#include <iostream>
using namespace std;

arich_event::arich_event(){
  this->version = 202000;
  this->runID = 1000;
  this->eventID = 100000;
}

arich_event::arich_event(int vers, int rID, std::vector<int> pID, int evID, double evT, std::vector<int> hP, std::vector<int> hT) {
  this->version = vers;
  this->runID = rID;
  this->particleID = pID;
  this->eventID = evID;
  this->eventTime = evT;
  this->hitPixels = hP;
  this->hitTimes = hT;
}

void arich_event::PrintEvent() {
  std::cout << "Event version: " << this->version << ", run ID:" << this->runID << ", event ID: " << this->eventID << '\n';
  std::cout << "Number of hit pixels: " << this->hitPixels.size() << '\n';
}

void EventToFile(arich_event* data){
  TFile f(Form("./output/arichevent_vers%d_run%d.root", data->version, data->runID),"RECREATE");
  TTree* tree = new TTree("arich_events", "arich_events");

  tree->Branch("version",  &(data->version),  "version/I");
  tree->Branch("runID",  &(data->runID),  "runID/I");
  tree->Branch("eventID",  &(data->eventID),  "eventID/I");
  tree->Branch("eventTime",  &(data->eventTime),  "eventTime/D");
  tree->Branch("hitPixels",  "std::vector<int>",  &(data->hitPixels));
  tree->Branch("hitTimes",  "std::vector<int>",  &(data->hitTimes));
  tree->Branch("charge",  "std::vector<int>",  &(data->charge));

  tree->Fill();

  f.Write();
  f.Close();
}

std::vector<arich_event*> FileToEvent(const char* filename, const char* treename){
  // TFile file(Form("./output/arichevent_vers%d_run%d.root", vers, rID), "read");
  TFile file(filename, "read");
  TTreeReader reader(treename, &file);
  std::vector<arich_event*> events;

  TTreeReaderValue<int> vrs(reader, "version");
  TTreeReaderValue<int> runI(reader, "runID");
  TTreeReaderValue<int> eventI(reader, "eventID");
  TTreeReaderValue<double> evTim(reader, "eventTime");
  TTreeReaderValue<std::vector<int>> pID(reader, "truth_pid");
  TTreeReaderValue<std::vector<int>> hitPix(reader, "hitPixels");
  TTreeReaderValue<std::vector<int>> hitTim(reader, "hitTimes");
  TTreeReaderValue<std::vector<int>> chrg(reader, "charge");
  TTreeReaderValue<std::vector<double>> sx(reader, "truth_sx");
  TTreeReaderValue<std::vector<double>> sy(reader, "truth_sy");
  TTreeReaderValue<std::vector<double>> dx(reader, "truth_dx");
  TTreeReaderValue<std::vector<double>> dy(reader, "truth_dy");
  TTreeReaderValue<std::vector<double>> mom(reader, "truth_mom");

  std::vector<double> tmpx;
    
  
  while (reader.Next()){
    arich_event* event = new arich_event();

    event->version = (int) *vrs;
    event->runID = (int) *runI;
    event->eventID = (int) *eventI;
    event->eventTime = (double) *evTim;
    event->particleID = (std::vector<int>) *pID;
    event->hitPixels = (std::vector<int>) *hitPix;
    event->hitTimes = (std::vector<int>) *hitTim;
    event->charge = (std::vector<int>) *chrg;
    tmpx = (std::vector<double>) *sx;
      
	 for(int ii=0;ii<tmpx.size();ii++){
		 event->dir.push_back(dx->at(ii));
		 event->dir.push_back(dy->at(ii));
		 event->pos.push_back(sx->at(ii)*0.1);
		 event->pos.push_back(sy->at(ii)*0.1);
		 event->mom.push_back(mom->at(ii));
	 }

    events.push_back(event);
  }
  file.Close();
  return events;
}

 TH2Poly* EventToHist(arich_event* data, TString PDfile){
   TFile* fdet = new TFile(PDfile, "read");
   TH2Poly* hDet = (TH2Poly*) fdet->Get("hDet");
   TH2Poly* htemp = (TH2Poly*) hDet->Clone();

	for(int i=0;i<data->hitPixels.size();i++){
		if(data->hitTimes.at(i)<-0.001)continue;
		if(data->hitTimes.at(i)>5)continue;//trigger window
		htemp->SetBinContent(data->hitPixels.at(i),data->charge.at(i));
	}
     

   htemp->SetName(Form("event_%d", data->eventID));
   htemp->SetTitle(Form("event_%d", data->eventID));

   htemp->SetDirectory(0);
   delete fdet;

   return htemp;
 }

 arich_event* HistToEvent(TH2Poly* htemp){
   arich_event* event = new arich_event();
   std::vector<int> hits;
   int nbins = htemp->GetNumberOfBins();

   for (int ibin = 1; ibin < nbins; ibin++){
     if (htemp->GetBinContent(ibin) > 0){
       hits.push_back(ibin);
     }
   }
   event->hitPixels = hits;
   return event;
 }
