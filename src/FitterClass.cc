// -------------------------------------------------------------------------------------------
// BasicFitter: A simple C++ interface to Minuit
// -------------------------------------------------------------------------------------------

#include "FitterClass.h"

//BasicFitter::FitterClass* BasicFitter::FitterClass::_glbInstance=0;
BasicFitter::FitterClass* BasicFitter::FitterClass::_gblInstance = NULL;
//Constructor
// -------------------------------------------------------------------------------------------

BasicFitter::FitterClass::FitterClass(TEnv *set){
    _set=set;
    
    _input=new BasicFitter::FitterInputClass(_set);

    for(int iobs=0; iobs<_input->GetObservables().size();iobs++){
        _nbins.push_back(_input->GetInputComponent(iobs).front().size());
    }

    // Set global instance
    setAsGlobalInstance(); 
        
    Printf("\n\n\n Fitter Class -- initializing \n\n");

    // Some constants we later will/might need
    p0 = 0; p1 = 1;

    // Create the fitter objects and bind it to our minuit function
    _tfitter = new TFitter( _input->GetFitPars().size() );    
    _tfitter->ExecuteCommand("SET PRINTOUT",&p1,1);
    _tfitter->SetFCN(minuitFunction);  
   
    // Setup the free parameters of the fit
    int ivar(0);
    for(Str FitPar : _input->GetFitPars() ) {
        // Sort out options in case we have not specified any we assume there are no limits
        VecD Opts; Opts.push_back(_input->GetFitOpt(ivar)[0]); 
        if( _input->GetFitOpt(ivar).size() == 1) {
            Opts.push_back( 0. ); Opts.push_back( 0. ); 
        } else {
            Opts.push_back( _input->GetFitOpt(ivar)[1] ); Opts.push_back( _input->GetFitOpt(ivar)[2] ); 
        }
        _tfitter->SetParameter(ivar++, FitPar,  Opts[0], 0.01, Opts[1],  Opts[2] );       
    }
   
   _tfitter->ExecuteCommand("CALL FCN", &p1 ,1);

}

// Fitter Function
// -------------------------------------------------------------------------------------------

void BasicFitter::FitterClass::Fit() {

      // Fix the maximal number of calls
      _tfitter->SetMaxIterations(10000);

      // Execute a simple minimization scheme
      _tfitter->ExecuteCommand("SIMPLEX",&p0,0);
      _tfitter->ExecuteCommand("MIGRAD",&p0,0); 
      _tfitter->ExecuteCommand("IMPROVE",&p0,0);
      _tfitter->ExecuteCommand("MINOS",&p0,0);    
      _tfitter->ExecuteCommand("HESSE",&p0,0);    
        
      // Show Covariance  
      _tfitter->ExecuteCommand("SHOW COVARIANCE",&p0,0);
      // Execute the function one last time
      _tfitter->ExecuteCommand("CALL FCN", &p1 ,1);

}

 // Evalute the Likelihood
// -------------------------------------------------------------------------------------------

void BasicFitter::FitterClass::Fcn() {

   _tfitter->ExecuteCommand("CALL FCN", &p1 ,1);

}

///NEW Likelihood Function
double BasicFitter::FitterClass::likelihood(double *x) { 

 double value = 0;


 for(int iobs=0; iobs<_input->GetObservables().size();iobs++){
    for(int ibin=0; ibin < _nbins[iobs];ibin++){
        double pred(0);
        for(int icat=0; icat<_input->GetMergedInputComponent(iobs).size();icat++){
            pred+=_input->GetMergedInput(iobs,icat)[ibin]*x[icat];
            value += -2*log(TMath::Poisson( _input->GetData(iobs)[ibin], pred ));
        }
    }
 }
     
    cout << " value = " << value << endl;
                     
 return value;

}

// Minuit Function
// -------------------------------------------------------------------------------------------

void BasicFitter::minuitFunction(int& nDim, double* gout, double& result, double par[], int flg) {

  result = BasicFitter::FitterClass::getGlobalInstance()->likelihood(par);

}
