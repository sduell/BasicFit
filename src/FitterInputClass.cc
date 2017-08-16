//-------------------------------------------------------------------------------------------
// FitterInputClass: Input Class for a basic fitter
//-------------------------------------------------------------------------------------------

#include "FitterInputClass.h"

//Constructor

BasicFitter::FitterInputClass::FitterInputClass(TEnv *set){
    _set=set;
    if(_set->GetValue("InputFile","")=="") fatal("Error: No inputfile specified");
    TFile f(_set->GetValue("InputFile",""));

    _ObsNames = Vectorize(_set->GetValue("Observables",""));
    StrV FitPars = Vectorize(_set->GetValue("FitPars",""));
    StrV Components;

    for(auto Observable : _ObsNames){
        VecD _v_cat_tot;
        VecD _v_asimovdata;
        VecD _v_data;
        VecVecD _vv_inputs;
        VecVecD _vv_mergedinputs;
        for(auto FitPar : FitPars){
            StrV Templates=Vectorize(_set->GetValue(Form("%s.Templates",FitPar.Data()),"")," ");
            double total_integral(0);
            VecD _v_mergedinputs;
            _v_FitPars.push_back(FitPar);
            _vv_FitOpts.push_back(VectorizeD(_set->GetValue(Form("%s.Startpar",FitPar.Data()),"1.0")," "));

            for(auto Template : Templates){
                Components.push_back(Template);
                VecD _v_inputs;
                TH1 *h=(TH1D*) f.Get(Observable+Template);
                
                if(_v_asimovdata.size()==0){
                    for(int i=0; i<h->GetXaxis()->GetNbins(); i++){
                        _v_asimovdata.push_back(0);
                    }
                }

                for(int i=0; i<h->GetXaxis()->GetNbins(); i++){
                    _v_inputs.push_back(h->GetBinContent(i+1));
                    _v_asimovdata[i]+=_v_inputs[i];
                }
                
                if(_v_mergedinputs.size() == 0){
                        _v_mergedinputs=_v_inputs;
                }
                else{
                    for(int i=0; i<_v_mergedinputs.size(); i++)
                        _v_mergedinputs[i]+=_v_inputs[i];
                }
                total_integral+=h->Integral();
                _vv_inputs.push_back(_v_inputs);
            }
            _v_cat_tot.push_back(total_integral);
            _vv_mergedinputs.push_back(_v_mergedinputs);
        }
        _vvv_inputs.push_back(_vv_inputs);
        _vvv_mergedinputs.push_back(_vv_mergedinputs);
        _vv_cat_tot.push_back(_v_cat_tot);
        _vv_asimovdata.push_back(_v_asimovdata);
        if(_set->GetValue("UseData","false")=="true"){
            Str datapp=_set->GetValue("DataAppendix","data");
            TH1 *h_data=(TH1D*) f.Get(Observable+datapp);
            for(int i=0; i<h_data->GetXaxis()->GetNbins(); i++){
                _v_data.push_back(h_data->GetBinContent(i+1));
            }
        }
        else _v_data=_v_asimovdata;
        _vv_data.push_back(_v_data);            
    }


}