#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2Poly.h"
#include "TTreeReader.h"

class arich_event : public TObject {
public:
  int version;
  int runID;
  int eventID;
  double eventTime;
  std::vector<int> particleID;
  std::vector<int> hitPixels; // IDs of pixels that have been hit
  std::vector<int> hitTimes; // Time in nanoseconds relative to eventTime
  std::vector<int> charge;

  std::vector<double> dir; // xdir, ydir 
  std::vector<double> pos; // xpos, ypos, cm
  std::vector<double> mom; // GeV

  arich_event();
  arich_event(int vers, int rID, std::vector<int> pID, int evID, double evT, std::vector<int> hP, std::vector<int> hT);
  void PrintEvent();
};

  void EventToFile(arich_event* data);
  std::vector<arich_event*> FileToEvent(const char* filename, const char* treename);
  TH2Poly* EventToHist(arich_event* data, TString PDfile);
  arich_event* HistToEvent(TH2Poly* htemp);
