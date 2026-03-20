{
    // Physical ROOT file:
    TFile *file = new TFile("test.root","recreate");
    // Output of first class will be saved here:
    TDirectoryFile *dirFile_1 = new TDirectoryFile("dirFile_1","dirFile_1_title");
    // Output of first member function in the first class will be saved here:
    TList *list_11 = new TList();
    list_11->SetName("list_11_name");
    // Output of second member function in the first class will be saved here:
    TList *list_12 = new TList();
    list_12->SetName("list_12_name");
    // ...
    // Output of second class will be saved here:
    TDirectoryFile *dirFile_2 = new TDirectoryFile("dirFile_2","dirFile_2_title");
    // Output of first member function in the second class will be saved here:
    TList *list_21 = new TList();
    list_21->SetName("list_21_name");
    // ...
    // Some example output objects:
    TH1D *hist_111 = new TH1D("hist_111","hist_111_title",10,0.,1.);
    hist_111->SetDirectory(0); // by default, upon creation, each histogram is automatically saved in the currently opened file. You disable that with this line.
    list_11->Add(hist_111); // add histogram to its TList
    TH1D *hist_112 = new TH1D("hist_112","hist_112_title",100,0.,100.);
    hist_112->SetDirectory(0); // by default, upon creation, each histogram is automatically saved in the currently opened file. You disable that with this line.
    list_11->Add(hist_112); // add histogram to its TList
    // Finally, add TList to its TDirectoryFile:
    dirFile_1->Add(list_11);
    // Some more example output objects:
    TProfile *pro_211 = new TProfile("pro_211","pro_211_title",10,-5.,5.);
    pro_211->SetDirectory(0); // by default, upon creation, each profile is automatically saved in the currently opened file. You disable that with this line.
    list_21->Add(pro_211); // add profile to its TList
    // Finally, add TList to its TDirectoryFile:
    dirFile_2->Add(list_21);
   // Finally, dump each TDirectoryFile in the physical ROOT file:
    dirFile_1->Write(dirFile_1->GetName(),TObject::kSingleKey);
    dirFile_2->Write(dirFile_2->GetName(),TObject::kSingleKey);
    file->Close();
    }