#include "call_libraries.h"

Bool_t istestcheck=1;
Bool_t isgenmatching=0;
TString coll_system;
TString trkefffile="/afs/cern.ch/user/v/vpant/pprefanalysis/filelists/Eff/ppref_2024_Pythia_QCDptHat15_NopU_2D_Nominla.root";
Float_t vzcut=15.;
Float_t trkDCAxycut;
Float_t trkDCAzcut;
Float_t ptresocut;
Float_t trketacut=1.;
Float_t trkptcut=0.1;
Bool_t isSystematics=1.;
Bool_t islowptfile=0; //Boolean to decide if the file has a 20 GeV cut or not
void Inputparameters(TString input_file,Bool_t is_MC)
{
if (input_file.Contains("PP"))
  {coll_system="pp";
    if (input_file.Contains("PPRefZeroBiasPlusForward4")) islowptfile=1;
  }
if (input_file.Contains("OO"))
  coll_system="OO";

if (coll_system=="pp")
  { trkDCAxycut=3.0;
    trkDCAzcut=3.0;
    ptresocut=0.1;
  }
 TString data_or_mc;
 if (is_MC==0) data_or_mc="DATA";
 if (is_MC==1) data_or_mc="MC";
 cout<<"Running files for........"<<data_or_mc.Data()<<endl;
 cout<<"For colliding system........"<<coll_system.Data()<<endl;
 cout<<"Trk DCAxy cut........"<<trkDCAxycut<<endl;
 cout<<"Trk DCAz cut........"<<trkDCAzcut<<endl;
 cout<<"Trk pT resolution cut............"<<ptresocut<<endl;
 
}
