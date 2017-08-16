//
// Utils Class
//

#include <iostream>
#include "Utils.h"
#include "TROOT.h"

///////////////////////////////////////////////////////////////////////////////////////
// ---- Some utility functions from Florian - Thanks Florian! -----/
///////////////////////////////////////////////////////////////////////////////////////

static int hi=0;

// abort program with error message
void BasicFitter::fatal(Str msg) {
    // print the error message
    std::cerr <<
    "\n*********************\n" <<
    "* BasicFitter, Fatal Error:  \n\n" <<
    "  " << msg <<
    "\n\n*********************\n";
    
    // print the time elapsed
    std::cout << "\n* Run aborted due to error.\n";
    printTime(); std::cout << std::endl;
    
    // exit the session
    exit(1);
}

// Create the Fitter plotting style
TStyle* BasicFitter::FitterStyle() {
    
    // make sure histogram bin-errors are properly treated
    TH1::SetDefaultSumw2();
    
    TStyle *style = new TStyle("BasicFitter","BasicFitter style");
    
    style->SetPalette(1);
    // white background
    style->SetFrameBorderMode(0); style->SetFrameFillColor(kWhite);
    style->SetCanvasBorderMode(0); style->SetCanvasColor(kWhite);
    style->SetPadBorderMode(0); style->SetPadColor(kWhite);
    style->SetStatColor(kWhite); style->SetLegendFillColor(kWhite);
    
    double tsize=0.045; int font=42; // regular Helvetcia
    style->SetTextSize(tsize); style->SetTextFont(font);
    style->SetLegendFont(font);
    for ( Str a : {"x","y","z"}) {
        style->SetLabelSize(tsize,a); style->SetLabelSize(tsize,a);
        style->SetLabelFont(font,a); style->SetLabelFont(font,a);
    }
    style->SetPadLeftMargin(0.14); style->SetPadRightMargin(0.04);
    style->SetPadBottomMargin(0.14); style->SetPadTopMargin(0.04);
    style->SetOptTitle(0); style->SetOptStat(0); style->SetOptFit(0);
    style->SetHistMinimumZero();
    
    // set the paper & margin sizes
    style->SetPaperSize(20,26);
    // set title offsets (for axis label)
    style->SetTitleXOffset(1.4); style->SetTitleYOffset(1.5);
    
    // use bold lines and markers
    style->SetMarkerStyle(20); style->SetMarkerSize(1.2);
    style->SetHistLineWidth(2.0);
    //   style->SetLineStyleString(2,"[12 12]"); // postscript dashes
    // get rid of error bar caps
    style->SetEndErrorSize(0.);
    // do not display any of the standard histogram decorations
    style->SetOptTitle(0); style->SetOptStat(0); style->SetOptFit(0);
    // put tick marks on top and RHS of plots
    style->SetPadTickX(1); style->SetPadTickY(1);
    return style;
}

void BasicFitter::setFitterStyle() {
    static TStyle* FitterStyle = 0;
    if ( FitterStyle==0 ) FitterStyle = BasicFitter::FitterStyle();
    gROOT->SetStyle("BasicFitter");
    gROOT->ForceStyle();
}


StrV BasicFitter::Vectorize(Str str, Str sep) {
    StrV result; TObjArray *strings = str.Tokenize(sep.Data());
    if (strings->GetEntries()==0) { delete strings; return result; }
    TIter istr(strings);
    while (TObjString* os=(TObjString*)istr()) {
        if (os->GetString()[0]=='#') break;
        add(result,os->GetString());
    }
    delete strings; return result;
}

VecD BasicFitter::VectorizeD(Str str, Str sep) {
    VecD result; StrV vecS = Vectorize(str,sep);
    for (uint i=0;i<vecS.size();++i)
        result.push_back(atof(vecS[i]));
    return result;
}

VecI BasicFitter::VectorizeI(Str str, Str sep) {
    VecI result; StrV vecS = Vectorize(str,sep);
    for (uint i=0;i<vecS.size();++i) {
        result.push_back(atoi(vecS[i]));
    }
    return result;
}

TEnv *BasicFitter::OpenSettingsFile(Str fileName) {
    if (fileName=="") error("No config file name specified. Cannot open file!");
    TEnv *settings = new TEnv();
    int status=settings->ReadFile(fileName.Data(),EEnvLevel(0));
    if (status!=0) error(Form("Cannot read file %s",fileName.Data()));
    return settings;
}

TFile *BasicFitter::OpenFile(Str fn) {
    TFile *f = TFile::Open(fn); if (f==NULL) error("Cannot open "+fn); return f;
}

// Prints current time/date and time elapsed
void BasicFitter::printTime()
{
    static bool first=true;
    static time_t start;
    if (first) { first=false; ::time(&start); }
    time_t aclock; ::time( &aclock );
    std::cout << "* Current time: " << asctime( localtime( &aclock ) )
    << "*   ( "<< ::difftime( aclock, start) <<" s elapsed )" << std::endl;
}

void BasicFitter::error(Str msg) {
    printf("ERROR:\n\n  %s\n\n",msg.Data());
    abort();
}
////////////////////////////////////////////////////////////////////////////////k///////
