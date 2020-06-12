// Offset70 folder.
void plot() {
	double gain[12];
	double outAmp[12];
	double outAmpErr[12];

gain[0] = 10;
gain[1] = 15;
gain[2] = 20;
gain[3] = 25;
gain[4] = 30;
gain[5] = 35;
gain[6] = 40;
gain[7] = 45;
gain[8] = 50;
gain[9] = 55;
gain[10] = 60;
gain[11] = 63;

outAmp[0] = 376.7;
outAmp[1] = 935.9;
outAmp[2] = 2126;
outAmp[3] = 4063;
outAmp[4] = 6430;
outAmp[5] = 8904;
outAmp[6] = 11739.7;
outAmp[7] = 14822.2;
outAmp[8] = 17893.2;
outAmp[9] = 21215.7;
outAmp[10] = 24403.8;
outAmp[11] = 26355.6;

outAmpErr[0] = 113.5;
outAmpErr[1] = 241.2;
outAmpErr[2] = 405.9;
outAmpErr[3] = 514.2;
outAmpErr[4] = 575.2;
outAmpErr[5] = 681.6;
outAmpErr[6] = 775.5;
outAmpErr[7] = 906.3;
outAmpErr[8] = 1021.9;
outAmpErr[9] = 1125.8;
outAmpErr[10] = 1171.4;
outAmpErr[11] = 1192.2;

TGraphErrors* graph = new TGraphErrors(12, gain, outAmp, NULL, outAmpErr);
TCanvas* c1 = new TCanvas("c1","c1",700,500);
c1->SetGrid();
c1->SetLeftMargin(0.14);
c1->SetRightMargin(0.07);
graph->SetMarkerStyle(20);
graph->SetMarkerColor(2);
graph->SetTitle("Output CROC amplitude, offset=70");
graph->GetXaxis()->SetTitle("gain (6-bit)");
graph->GetYaxis()->SetTitle("output amplitude (ADU)");
graph->GetHistogram()->SetMinimum(0); // SetMinimum() SetMaximum() in y-axis.
TAxis* xaxis = graph->GetXaxis(); 	// Set range in
xaxis->SetLimits(0.,67.); 			// x-asis.
graph->Draw("AP");
c1->SaveAs("CROC_gain_offset70.png","");
c1->SaveAs("CROC_gain_offset70.pdf","");


}