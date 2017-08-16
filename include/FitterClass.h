#ifndef _FitterClass
#define _FitterClass

// -------------------------------------------------------------------------------------------
// FitterClass: a simple c++ interface to minuit
// -------------------------------------------------------------------------------------------

//include c++ stuff
#include <vector>

//include stuff from ROOT
#include "TMinuit.h"
#include "TFitter.h"
#include "TVirtualFitter.h"
#include "TRandom3.h"

//include stuff from the local framework
#include "FitterInputClass.h"

//include utility class
#include "Utils.h"

namespace BasicFitter{
    class FitterClass{
        public:
            FitterClass(TEnv *set);
            ~FitterClass();
            
            // Global instances
            static FitterClass* getGlobalInstance() { return _gblInstance; };
            void setAsGlobalInstance() { _gblInstance = this; };
       
            VecVecD getPars(){return _pars;};
            VecVecD getParErrs(){return _pars_err;};

            // likelihood function
            double likelihood(double *x);
            // chisq function
            double chisq(double *x);

            // Execute the fit
            void Fit();

            // Evalute the Likelihood
            void Fcn();

            // Return the private fitter object if requested
            TFitter* getFitter() { return _tfitter; };

            //Grab the input from the inputclass
            FitterInputClass* GetInput(){return _input;};

        private:

            // TEnv object to read out settings file
            TEnv *_set;
            // Input class pointer
            FitterInputClass *_input;
            // TFitter object to interface with minuit
            TFitter *_tfitter;
            // values to store current likelihood and minuit bits that are needed to set things up
            double _loglikelihood, p0,p1;
            //fit parameter values
            VecVecD _pars, _pars_err;
            // Global instance pointer for class
            static FitterClass* _gblInstance; 
            // Number of bins of the binned likelihood
            VecI _nbins;
    };
    void minuitFunction(int& nDim, double* gout, double& result, double par[], int flg);
}
#endif
