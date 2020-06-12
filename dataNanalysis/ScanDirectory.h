// Code to scan all files in a directory with name "dirname", with a given extend (or not ifwe use ext=""). The output is a matrix of strings with dimention 1x(Nfiles), where Nfiles is the exact number of those files.

//void ScanFilesInDirectory(const char *dirname="/home/gpapad/Desktop/Stage/Xcode/source_codes/data/", const char *ext=".mca" ) {

TString *ScanDirectory(const char *dirname, const char *ext, int& Nfiles) {
 
TString fileList[2000]; // Maximum # of files-> 1000. Can be changed very easily.
int i;
i=0;

TSystemDirectory dir(dirname, dirname); 
TList *files = dir.GetListOfFiles(); 
// If the directory exists, "if(files)" will continue, even if there are no files in the folder with the given extend or no files at all. "if(files)" will not continue only if the "dirname" does not exist.
if(files) { 
	TSystemFile *file;
	TString fname; 
	TIter next(files); 
	while ((file=(TSystemFile*)next())) { 
		fname = file->GetName(); 
		if (!file->IsDirectory() && fname.EndsWith(ext)) { 

			fileList[i] = fname.Data();
			//cout << fname.Data() << endl;
			i++;	
		} 
	} 
}
Nfiles=i;
//cout<<"Number of files: "<< Nfiles << endl;
TString *filenamesList = new TString[Nfiles];
for(int j=0; j<Nfiles; j++) { 
	filenamesList[j]=fileList[j];
	//cout<<filenames[j]<<endl;
	}

return filenamesList;

}
