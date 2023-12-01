#include "generate_clusters.h"

int LoadFrame(TString filename, int all_e[288], int all_t[288]){
    ifstream infile;
    infile.open(filename);
    if(!infile.is_open()){
       cout<<"Couldn't find file: "<<filename<<endl;
       return 0;
    }
 
    Ssiz_t from=0;
    TString content,tmp;
    int nn=0;

    while(tmp.ReadLine(infile)){
          tmp.Tokenize(content,from,"  ");
          int ee=atoi(content.Data());
          tmp.Tokenize(content,from,"  ");
          int tt=atoi(content.Data());
       
          all_e[nn] = ee;
          all_t[nn] = tt;

          from=0;
          nn++;
     }
    infile.close();
    return 1;
   
}


void plot_cluster(){

     int nframe=6;

     TH2F *hE[nframe];
     TH2F *hT[nframe];

     TH2F *hE_tot = new TH2F("hE_tot","hE_tot",12,0,12,24,0,24);

     for(int ii=0; ii<nframe; ii++){
         TString filename = Form("data/frame%d.txt",ii);
         int all_t[288]={0};
         int all_e[288]={0};
         int valid = LoadFrame(filename, all_e, all_t);

         hE[ii] = new TH2F(Form("hE%d",ii),Form("hE%d",ii),12,0,12,24,0,24);
         hT[ii] = new TH2F(Form("hT%d",ii),Form("hT%d",ii),12,0,12,24,0,24);
         for(int jj=0; jj<288; jj++){
             int yy = jj/12;
             int xx = jj%12;
             if(all_e[jj]>0){
                hE[ii]->Fill(xx, yy, all_e[jj]);
                hT[ii]->Fill(xx, yy, all_t[jj]*4+ii*32);
                hE_tot->Fill(xx, yy, all_e[jj]);
             }
         } 
         
     }

     TCanvas *c1 = new TCanvas("c1");
     c1->Divide(3,2);
     for(int ii=0; ii<nframe; ii++){
        c1->cd(ii+1);
        hE[ii]->Draw("COLZ TEXT");
        gPad->SetGrid();
     }

     TCanvas *c2 = new TCanvas("c2");
     c2->Divide(3,2);
     for(int ii=0; ii<nframe; ii++){
        c2->cd(ii+1);
        hT[ii]->Draw("COLZ TEXT");
        gPad->SetGrid();
     }

     TCanvas *c3 = new TCanvas("c3");
     c3->SetGrid();
     hE_tot->Draw("COLZ TEXT");

}
