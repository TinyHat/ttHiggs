int SaveHistograms(const std::string& inputFile, const std::string& outputDir, const std::string& label="")
{
	TFile* file = TFile::Open(inputFile.c_str(), "OPEN");
	TList* list = file->GetListOfKeys();

	TCanvas* canvas1 = new TCanvas("test", "Canvas Test");

	std::string baseDir = "/scratch0/aturnbul/major_project/TopRootCoreSMT-00-00-00-58/run/Histograms/basicPlots/";

	for(int objIdx = 0; objIdx != list->GetSize(); objIdx++)
	{
		std::string histoName = list->At(objIdx)->GetName();

		std::cout << "Drawing :: " << histoName << std::endl;
		TH1D* histo = (TH1D*) file->Get(histoName.c_str());
		histo->Draw();

		std::string name = baseDir + outputDir + histoName + label;
		std::string pdfName = name + ".pdf";
		std::string psName = name + ".ps";
		std::cout << name << std::endl;

		canvas1->SaveAs(pdfName.c_str(), "pdf");
		canvas1->SaveAs(psName.c_str(), "ps");
	}

	return (0);
}
