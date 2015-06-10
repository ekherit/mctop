{
  gSystem->Load("libMyEvent.so");
  McTopo m;
  for(int i=0;i<10000;i++)
  {
    m.GetEntry(i);
    unsigned long hash = test_hash2(&m);
    if(hash != 808577621) cout << hash  << "    " << info(&m) << endl;
  }
}
