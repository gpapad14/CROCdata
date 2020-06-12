void GetInputAmplitude(TString filename, double& inAmpl, double& inAmplErr) {

	string letter="";
	string letter2="";
	string inputSTR="";
	bool found=false;
	int i=0;
	
		while(i<filename.Length()-4) { // (-4) because of ".txt"
			letter=filename[i];
			letter2=filename[i+1];
			if(letter=="I" && letter2=="n" && !found) { 
				i=i+2;
				letter=filename[i];
				while( !(letter=="_") )  {
					inputSTR+=letter;
					i++;
					letter=filename[i];
				}
				found=true;
			}
			i++;
		}
		double Xinput = atof( inputSTR.c_str() );
		inAmpl = Xinput*0.989722 - 0.0087515; // slope and offset
		// slopeError = 0.00107635
		// offsetError = 0.00940651
		inAmplErr = sqrt( pow(Xinput*0.00107635,2) + pow(0.00940651,2) );

	
}