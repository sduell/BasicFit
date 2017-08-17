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

//Likelihood Function
double BasicFitter::FitterClass::likelihood(double *x) { 

 double value = 0;


cout << " fitpars: = " << x[0] <<" "<<x[1]<< endl;

 for(int iobs=0; iobs<_input->GetObservables().size();iobs++){
    for(int ibin=0; ibin < _nbins[iobs];ibin++){
        double pred(0);
        for(int icat=0; icat<_input->GetMergedInputComponent(iobs).size();icat++){
            pred+=_input->GetMergedInput(iobs,icat)[ibin]*x[icat];
            //cout << " inputvalues = " << _input->GetMergedInput(iobs,icat)[ibin] << endl;
        }
            //cout << " pred = " << pred << endl;
            //cout << " datavalues = " << _input->GetData(iobs)[ibin] << endl;
        if(TMath::Poisson( _input->GetData(iobs)[ibin], pred )!=0)
            value += -2*log(TMath::Poisson( _input->GetData(iobs)[ibin], pred ));
    }
 }
     
    //cout << " value = " << value << endl;
                     //Fatal("here","");
 return value;

}

//Likelihood Function
double BasicFitter::FitterClass::chisq(double *x) { 
 // chisq = ( f(x) - data )^T * Cov^-1 * ( f(x) - data )
 double value = 0;

 for(int iobs=0; iobs<_input->GetObservables().size();iobs++){
    //In this example Cov is NOT with correlations, only statistical errors!
    TMatrixD Cov(_nbins[iobs],_nbins[iobs]);
    TMatrixD Nmc(_nbins[iobs],1);
    TMatrixD Ndat(_nbins[iobs],1);
    for(int ibin=0; ibin < _nbins[iobs];ibin++){
        double pred(0);
        Cov(ibin, ibin)=sqrt(_input->GetData(iobs)[ibin]);
        Ndat(ibin,0)=_input->GetData(iobs)[ibin];
        Nmc(ibin,0)=0;
        for(int icat=0; icat<_input->GetMergedInputComponent(iobs).size();icat++){
            Nmc(ibin,0)+=_input->GetMergedInput(iobs,icat)[ibin]*x[icat];
        }
    }
    TMatrixD mDiff = TMatrixD(Nmc, TMatrixD::kMinus, Ndat);              // f(x) - data 
    TMatrixD C1(Cov);
    C1.Invert(); //Determine C^-1
    TMatrixD p1=TMatrixD(mDiff, TMatrixD::kTransposeMult, C1);
    TMatrixD p2=TMatrixD(p1, TMatrixD::kMult, mDiff);
    value+=p2(0,0);
 }

 return value;

}

// Minuit Function
// -------------------------------------------------------------------------------------------

void BasicFitter::minuitFunction(int& nDim, double* gout, double& result, double par[], int flg) {

  result = BasicFitter::FitterClass::getGlobalInstance()->likelihood(par);

}
