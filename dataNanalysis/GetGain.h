void GetGain(TString filename, double& GainSet) {

	string letter="";
	string cf1="";
	string gainSTR="";
	bool found=false;
	int i=0;
	cf1=filename[i];
	cf1+=filename[i+1];
	cf1+=filename[i+2];
	//cout<<cf1<<endl;
	if( cf1=="CF1" ) { GainSet=1; }
	else {
		while(i<filename.Length()-4) { // (-4) because of ".txt"
			letter=filename[i];
			if(letter=="G" && !found) { 
				i++;
				letter=filename[i];
				while( !(letter=="_") )  {
					gainSTR+=letter;
					i++;
					letter=filename[i];
				}
				found=true;
			}
			i++;
		}
		int gain = atof( gainSTR.c_str() );
		GainSet = gain;
	}
}