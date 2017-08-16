#ifndef _FitterInputClass
#define _FitterInputClass

#include "Utils.h"

namespace BasicFitter{
    class FitterInputClass{
        
        public:

            FitterInputClass(TEnv *set);
            ~FitterInputClass();

            //Grab the individual inputs
            VecVecVecD GetInputs() {return _vvv_inputs;};
            VecVecD GetInputComponent(int i){return _vvv_inputs[i];};
            VecD GetInput(int i, int j){return _vvv_inputs[i][j];};

            //Grab the templated inputs
            VecVecVecD GetMergedInputs() {return _vvv_mergedinputs;};
            VecVecD GetMergedInputComponent(int i){return _vvv_mergedinputs[i];};
            VecD GetMergedInput(int i, int j){return _vvv_mergedinputs[i][j];};

            //These functions are for performing an asimov fit
            VecVecD GetAsimovDataInputs(){ return _vv_asimovdata; };
            VecD GetAsimovData(int i){ return _vv_asimovdata[i]; };

            //These functions are for performing a fit to data
            //! Caution, when no data is included, this will be set to asimov data!
            VecVecD GetDataInputs(){ return _vv_data; };
            VecD GetData(int i){ return _vv_data[i]; };

            void SetData(VecVecD vv_data){_vv_data=vv_data;};

            //Get the parameter that is to be used in the fit
            StrV GetFitPars() { return _v_FitPars; };
            Str GetFitPar(int i) { return _v_FitPars[i]; };

            //Get additional constraints for the fit
            VecVecD GetFitOpts() { return _vv_FitOpts; };
            VecD GetFitOpt(int i) { return _vv_FitOpts[i]; };

            VecVecD GetCatTotals(){return _vv_cat_tot;};
            VecD GetCatTotal(int i){return _vv_cat_tot[i];};
            double GetCatTotal(int i, int j){return _vv_cat_tot[i][j];};

            //Grab the asimov data
            VecVecD GetAsimovs(){return _vv_asimovdata;};
            VecD GetAsimov(int i){return _vv_asimovdata[i];};
            double GetTotal(int i, int j){return _vv_asimovdata[i][j];};

            //Grab the Observables
            StrV GetObservables(){return _ObsNames;};

        private:
            // TEnV Pointer
            TEnv *_set;
            
            // Vector of component vector of inputs
            VecVecVecD _vvv_inputs, _vvv_mergedinputs; VecVecD _vv_asimovdata, _vv_cat_tot;
            
            // Vector of data input
            VecVecD _vv_data;
            
            // Vector of free parameters of the fit
            StrV _v_FitPars, _ObsNames; VecVecD _vv_FitOpts;

    };
}

#endif