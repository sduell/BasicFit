#ifndef _UTILS
#define _UTILS

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <bitset>
#include <time.h>

#include "TROOT.h"
#include "TSystem.h"
#include "TChain.h"
#include "TFile.h"
#include "TEnv.h"
#include "TTree.h"
#include "TChain.h"
#include "TLeaf.h"
#include "TChain.h"
#include "TFile.h"
#include "TLine.h"
#include "TString.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TH2.h"
#include "TStyle.h"
#include "TH2F.h"
#include "TMatrixD.h"
#include "TH3F.h"
#include "TProfile2D.h"
#include "TLorentzVector.h"
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TGraph2DErrors.h>
#include <TLatex.h>
#include <TGraphQQ.h>
#include "TArrow.h"
#include "TPad.h"
#include <TVectorD.h>
#include "TF1.h"
#include "TMarker.h"

//#include "RooHist.h"
//#include "RooCurve.h"

#include <vector>

#include <Math/PdfFuncMathCore.h>
#include <Math/ProbFuncMathCore.h>

#include "Math/Functor.h"
#include "Math/BrentMinimizer1D.h"

using namespace std;

typedef TString Str;
typedef TMatrixD TM;
typedef unsigned int uint;
typedef vector<TString> StrV;
typedef vector< vector <TString> > StrVV;
typedef vector< vector < vector <TString> > > StrVVV;
typedef vector<float> VecF;
typedef vector<int> VecI;
typedef vector<double> VecD;
typedef vector<TF1*> VTF;
typedef vector <vector <double> > VecVecD;
typedef vector <vector <double> > V2D;
typedef vector <vector<vector <double> > > VecVecVecD;
typedef vector <vector<vector <double> > > V3D;
typedef vector<TH1*> VecTH1;
typedef vector<TH1D> VTH1;
typedef vector<vector <TH1*> > VecVecTH1;
typedef vector<TGraph*> VecTG;
typedef vector<TGraphErrors> VTG;
typedef vector <vector <TGraphErrors> > V2TG;
typedef vector<TGraphAsymmErrors*> VecTGA;
typedef TGraphAsymmErrors TGA;
typedef TGraphErrors TG;


///////////////////////////////////////////////////////////////////////////////////////
namespace BasicFitter{
void fatal(Str msg);
TStyle* FitterStyle();
void setFitterStyle();

template <class T> void add(vector<T> &vec, T a) { vec.push_back(a); };
template <class T> void add(vector<T> &v, T a, T b) { add(v,a); add(v,b); };
template <class T> void add(vector<T> &v, T a, T b, T c) { add(v,a,b); add(v,c); };
template <class T> void add(vector<T> &v, T a, T b, T c, T d) { add(v,a,b); add(v,c,d); };
template <class T> void add(vector<T> &vec, T a[]) {
    uint n=sizeof(a)/sizeof(a[0]);
    for (uint i=0;i<n;++i) vec.push_back(a[i]);
}
template <class T> vector<T> vec(T a) { vector<T> v; add(v,a); return v; };
template <class T> vector<T> vec(T a, T b) { vector<T> v; add(v,a,b); return v; };
template <class T> vector<T> vec(T a, T b, T c) { vector<T> v; add(v,a,b,c); return v; };

StrV Vectorize(Str str, Str sep=" ");
VecD VectorizeD(Str str, Str sep);
VecI VectorizeI(Str str, Str sep);
TEnv *OpenSettingsFile(Str fileName);
TFile *OpenFile(Str fn);
void printTime();
void error(Str msg);
}


#endif
