
#include "TMath.h"
// #include "TF1.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TH2Poly.h"
#include "THStack.h"
#include <chrono>
#include "THStack.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TSystem.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include "TVector3.h"
// #include "TMatrixD.h"
// #include "TMatrixT.h"
#include <vector>
#include "TLegend.h"
#include <glob.h>
#include <fstream>
#include <numeric>
#include "arich_event.hh"

using namespace std;
using namespace std::chrono;

const int NUMPARTICLES = 3;
const char* PNAMES[NUMPARTICLES] = {"Pion", "Kaon", "Proton"};
const char* PNAMES_lower[NUMPARTICLES] = {"pip", "kaon", "proton"};
const double MASSES[NUMPARTICLES] = {0.1395701, 0.493677, 0.938272};
const int PDG[NUMPARTICLES] = {211, 321, 2212};


void get_map(TH2Poly* hist2D,const char *particle, int c){
    
            TList* listBins = hist2D->GetBins();
            ofstream myfile;
            myfile.open(Form("./out_text/event_%s_%i.txt",particle,c));
            myfile << "x_min" <<","<< "x_max" <<","<< "y_min" <<","<< "y_max" <<","<< "deposit" << "," << endl;
            for(int n =1; n <= hist2D->GetNumberOfBins();n++){
                TH2PolyBin* th2Bins = (TH2PolyBin*)listBins->At(n-1);
                double xmin = th2Bins->GetXMin();
                double xmax = th2Bins->GetXMax();
                double ymin = th2Bins->GetYMin();
                double ymax = th2Bins->GetYMax();
                double binc = hist2D->GetBinContent(n);
                myfile << "," << xmin <<","<< xmax <<","<< ymin <<","<< ymax <<","<< binc <<"," << endl;
              }
               myfile.close();
            delete hist2D;
            }

void get_distributions(string name, const char *particle){
   
    //TString filename=infile;
    
    //auto hs_hits = new THStack("hs_hits","");
    //auto hs_intC = new THStack("hs_intC","");
    
    
    TString PDfile = "../../ARICH_Reco/geometry/root/tight.root";
    
    const char *filename = name.c_str();
    const char *treename = "SimTruth";
    
    TFile* fdet = new TFile(PDfile,"read");
    TH2Poly* hDet = (TH2Poly*) fdet->Get("hDet");
   
    
    TFile file(filename, "read");
    
    TTreeReader reader(treename, &file);
    
    TTreeReaderValue<std::vector<int>> hitPix(reader, "hitPixels");
    TTreeReaderValue<std::vector<int>> chrg(reader, "charge");
    TTreeReaderValue<std::vector<int>> hitTim(reader, "hitTimes");
    
    int c = 0;
    
    auto histHit = new TH1I(Form("histHist_%s",particle),"Distribution Hits",200,0,1000);
    auto histSignal = new TH1I(Form("histSignal_%s", particle),"Distribution charge depo",100,100,350);
    
    while (reader.Next()){
        
        TH2Poly* htemp = (TH2Poly*) hDet->Clone();
        
        arich_event* event = new arich_event();
        
        event->hitPixels = (std::vector<int>) *hitPix;
        event->charge = (std::vector<int>) *chrg;
        event->hitTimes = (std::vector<int>) *hitTim;
       
       
        
        int sum_charge =0;
        int sum_hits = 0;
        
    
        for(int i=0;i<event->hitPixels.size();i++){
            if(event->hitTimes.at(i)<-0.001)continue;
            if(event->hitTimes.at(i)>5)continue;//trigger window
            
            sum_charge += event->charge.at(i);
            
            htemp->SetBinContent(event->hitPixels.at(i),event->charge.at(i));
           
            sum_hits += 1;
        }
        histHit->Fill(sum_hits);
        histSignal->Fill(sum_charge);
        get_map(htemp,particle,c);
        c +=1;
    }
     
     histSignal->GetXaxis()->SetTitle("Integrated charge deposited");
     histHit->GetXaxis()->SetTitle("Total hits");
    
    //get_map(filename ,PNAMES_lower[j]);
    histSignal->SaveAs(Form("hist_intC_%s.root",particle));
    histHit->SaveAs(Form("hist_hits_%s.root",particle));
    //auto cs = new TCanvas("cs","cs",10,10,700,900);
    
}

int main(int argc,const char* argv[]){
/*  1: Energy;
    2: Folder of root files;
    3: Output file name
*/
    
    cout << "=====================================================" << endl;
    cout << "                      ANALISYS" << endl;
    cout << "=====================================================" << endl;
    
    const char *root_file = argv[1];
    const char *particle = argv[2];
    
    high_resolution_clock::time_point t1 = high_resolution_clock::now(); 
    
    get_distributions(root_file,particle);
    
    auto duration = duration_cast<microseconds>( high_resolution_clock::now() - t1 ).count();
    cout << fixed << "Finished! Time taken: " << duration / 1000000. << " seconds" << endl;

    return 0;
}

