
void plotWF() {
gStyle->SetOptFit(1111);

ifstream inpWF;

bool save=false;
// Filenames
int nfiles=23;
TString WFfilenameList[nfiles];
WFfilenameList[0] = "INA1F.CSV";
WFfilenameList[1] = "INA1F01.CSV";

WFfilenameList[2] = "INA3F.CSV";
WFfilenameList[3] = "INA3F01.CSV";
WFfilenameList[4] = "INA3F02.CSV";

WFfilenameList[5] = "INA5F.CSV";
WFfilenameList[6] = "INA5F01.CSV";
WFfilenameList[7] = "INA5F02.CSV";

WFfilenameList[8] = "INA7F.CSV";
WFfilenameList[9] = "INA7F01.CSV";
WFfilenameList[10] = "INA7F02.CSV";

WFfilenameList[11] = "INA9F.CSV";
WFfilenameList[12] = "INA9F01.CSV";
WFfilenameList[13] = "INA9F02.CSV";

WFfilenameList[14] = "INA10F.CSV";
WFfilenameList[15] = "INA10F01.CSV";
WFfilenameList[16] = "INA10F02.CSV";

WFfilenameList[17] = "INA11F.CSV";
WFfilenameList[18] = "INA11F01.CSV";
WFfilenameList[19] = "INA11F02.CSV";


WFfilenameList[20] = "INA15F.CSV";
WFfilenameList[21] = "INA15F01.CSV";
WFfilenameList[22] = "INA15F02.CSV";



//************************************************
// Start the loop.
for(int ifile=0; ifile<nfiles; ifile++) {
//for(int ifile=2; ifile<3; ifile++) {

	TString WFfilename;
	WFfilename=WFfilenameList[ifile];

	string inAmpliSetSTR = "";
	string character;
	bool found=false;
	int ivar=3;
	//cout<<WFfilename<<endl;
	while(!(found) && ivar<20) {
		character=WFfilename[ivar];
		if( !(character.compare("F")) ) { break; }
		inAmpliSetSTR+=WFfilename[ivar]; 
		ivar++;
	}
	//cout<<inAmpliSetSTR<<endl;
	TString plotTitle = inAmpliSetSTR+"_amplitude";

double inAmpliSet = atof(inAmpliSetSTR.c_str()); // in mV
double histLow = -5.0005; 	// in mV
double histHigh = 19.9995;	// in mV
double histResol = 0.001; 	// in mV
double histRange = histHigh - histLow;
double histDivisions = histRange/histResol;

TLegend* legend = new TLegend(0.8,0.89,0.99,0.99);

inpWF.open(WFfilename);

//================================================
// Find the number of points.
int nlines=0;
TString line;
//Skip the text and go to numbers.
inpWF>>line;
inpWF>>line;
inpWF>>line;
inpWF>>line;
while(inpWF>>line) {
	nlines++;
}
inpWF.close();
//cout<<"Number of inputs: "<<nlines<<endl;

//================================================

inpWF.open(WFfilename);

TH1F* wfhist = new TH1F("wfhist","Histogram of raw data", histDivisions, histLow , histHigh);
TGraph* waveform = new TGraph(nlines); 
waveform->SetName("waveform");

double volt, time;
int iline=0;

//Skip the text and go to numbers.
inpWF>>line;
inpWF>>line;
inpWF>>line;
inpWF>>line;
string letter;
while(inpWF>>line && iline<nlines) {
	//cout<<line<<endl;
	string timeSTR="";
	string voltSTR="";
	bool beforeComma=true;  // time value. !beforeComma : voltage value
	for(int ilet=0; ilet<line.Length(); ilet++) {
		letter=line[ilet];
		//cout<<letter<<endl;
		if(!(letter.compare(","))) { beforeComma=false; } // If {letter is not "_"}. It will continue if letter is NOT "_"
		else if(beforeComma) { timeSTR+=letter; }
		else { voltSTR+=letter; }
	}
	//cout<< "#"<<iline<< " Time= "<<timeSTR<<"|| voltage="<<voltSTR<<endl;
	time = atof(timeSTR.c_str());
	volt = atof(voltSTR.c_str());
	//cout<< "#"<<iline<< " Time= "<<time<<", voltage="<<volt<<endl;
	

	waveform->SetPoint(iline, time*1e6, volt*1000);
	wfhist->Fill(volt*1000);
	iline++;
}
if(!(iline==nlines)){ cout<<"Something wrong with number of lines that were read!!!"<<endl; }

TCanvas* cWFRaw = new TCanvas("cWFRaw","Waveform",800,600);
cWFRaw->SetGrid();
cWFRaw->SetRightMargin(0.07);
cWFRaw->SetLeftMargin(0.12);
waveform->SetMarkerStyle(6);
waveform->SetMarkerColor(4);
waveform->SetLineColor(4);
legend->AddEntry(waveform,WFfilenameList[ifile],"l");

waveform->SetTitle(plotTitle);
waveform->GetXaxis()->SetTitle("time (us)");
waveform->GetYaxis()->SetTitle("Amplitude (mV)");
//waveform->GetYaxis()->SetRangeUser(4,10);
//TAxis* axis = waveform->GetXaxis();
//axis->SetNoExponent(kTRUE);
waveform->Draw("APL");
waveform->GetXaxis()->SetRangeUser(-10,110);
legend->Draw();
if(save) { cWFRaw->SaveAs(WFfilename+"_RAW.png",""); }
inpWF.close();

TCanvas* cWFHist = new TCanvas("cWFHist","Histogram of raw data",800,600);
cWFHist->SetGrid();
wfhist->GetXaxis()->SetTitle("voltage (mV)");

wfhist->Fit("gaus","Q0","", 0 - 0.2, 0 + 0.2);
wfhist->Fit("gaus","Q0","", wfhist->GetFunction("gaus")->GetParameter(1) - 3*wfhist->GetFunction("gaus")->GetParameter(2), wfhist->GetFunction("gaus")->GetParameter(1) + 3*wfhist->GetFunction("gaus")->GetParameter(2));
double meanBase=wfhist->GetFunction("gaus")->GetParameter(1);
double sigmaBase=wfhist->GetFunction("gaus")->GetParameter(2);

wfhist->Fit("gaus","Q0","",inAmpliSet - 0.2,inAmpliSet + 0.2);
wfhist->Fit("gaus","Q0","", wfhist->GetFunction("gaus")->GetParameter(1) - 3*wfhist->GetFunction("gaus")->GetParameter(2), wfhist->GetFunction("gaus")->GetParameter(1) + 3*wfhist->GetFunction("gaus")->GetParameter(2));
double meanSignal=wfhist->GetFunction("gaus")->GetParameter(1);
double sigmaSignal=wfhist->GetFunction("gaus")->GetParameter(2);

double ampli = meanSignal - meanBase;
double ampliErr = sqrt( sigmaBase*sigmaBase + sigmaSignal*sigmaSignal );
int precision=2;
if(ampli/1 > 1) { precision=3; }
if(ampli/10 > 1) { precision=4; }
//if(inAmpliSet==3 || inAmpliSet==5 || inAmpliSet==7 || inAmpliSet==9) {
	cout<<WFfilename<<endl;
	cout<<"Amplitude = "<<setprecision(precision)<< ampli <<" +/- "<<setprecision(1) <<ampliErr<<" mV "<< "(" << setprecision(2)<<100*ampliErr/ampli<<"%)"<<endl;
//}

wfhist->Draw();
if(save) { cWFHist->SaveAs(WFfilename+"_HIST.png",""); }


//======================================================================

int ipoint;
int startpoint=145;
double x, xprev, dum;
bool lowbool=false;
double base=0;
double signal=0;
int nbase=0;
int nsignal=0;
double amplitude;
int iampl=0;
TH1F* histAmpl = new TH1F("histAmpl","Histogram of amplitude", 1000, meanSignal - 1.0005, meanSignal + 0.9995);

waveform->GetPoint(startpoint-1, dum, xprev);	
//for(ipoint=startpoint; ipoint<nlines; ipoint++) {
for(ipoint=startpoint; ipoint<nlines; ipoint++) {

	waveform->GetPoint(ipoint, dum, x);	
	//cout<<"#"<<ipoint<<" ("<<dum<<", "<< x<<")"<<endl;
	if( (x>meanBase-2*sigmaBase && x<meanBase+2*sigmaBase) || (x>meanSignal-2*sigmaSignal && x<meanSignal+2*sigmaSignal) ) {
		if(abs(x-xprev)<0.5) {
			if(!lowbool){ base+=x; nbase++; }
			else { signal+= x; nsignal++; }
		}
		else {
			//cout<<"#"<<ipoint<<" condition1"<<endl;
			if(!lowbool) { // if lowbool is false then continue
				lowbool = true;
				signal+= x; nsignal++;
			}
			else {
				//cout<<base/nbase<<", "<<signal<<endl;
				amplitude = signal/nsignal - base/nbase;
				histAmpl->Fill(amplitude);
				//cout<<"#"<<iampl<<" amplitude = "<< amplitude<<endl;
				iampl++;
				lowbool=false;
				base=0;
				signal=0;
				nbase=0;
				nsignal=0;

			}
		}
		xprev=x;
	}


}

TCanvas* cAmpl = new TCanvas("cAmpl","Histogram of amplitude",800,600);
cAmpl->SetGrid();
histAmpl->GetXaxis()->SetTitle("voltage (mV)");
histAmpl->Draw();
histAmpl->Fit("gaus","Q","");
double meanAmplitude = histAmpl->GetFunction("gaus")->GetParameter(1);
double sigmaAmplitude = histAmpl->GetFunction("gaus")->GetParameter(2);
histAmpl->GetXaxis()->SetRangeUser(meanAmplitude-5*sigmaAmplitude, meanAmplitude+5*sigmaAmplitude);
if(save) { cAmpl->SaveAs(WFfilename+"_Ampl.png",""); }

precision=3;
if(meanAmplitude/1 > 1) { precision=4; }
if(meanAmplitude/10 > 1) { precision=5; }
// Recon Ampli = Reaconstructed amplitude
//if(inAmpliSet==3 || inAmpliSet==5 || inAmpliSet==7 || inAmpliSet==9) {
	cout<<"Recon Ampli = "<<setprecision(precision)<< meanAmplitude <<" +/- "<<setprecision(2) <<sigmaAmplitude<<" mV "<< "(" << setprecision(2)<<100*sigmaAmplitude/meanAmplitude<<"%)"<<endl;
//}

delete cWFRaw;
delete wfhist;
delete cWFHist;
delete histAmpl;
delete cAmpl;


}
// End loop over all files
//************************************************

cout<<"----- END -----"<<endl;
}
