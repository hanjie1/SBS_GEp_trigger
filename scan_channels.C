void scan_channels(){
   int nframe=3;

    ofstream outfile1("data/frame0.txt");
    for(int ich=0; ich<256; ich++)
       outfile1<<0<<"  "<<0<<endl;
    for(int ich=0; ich<32; ich++)
       outfile1<<0<<"  "<<0<<endl;

    outfile1.close();

    ofstream outfile2("data/frame1.txt");
    for(int ich=0; ich<256; ich++){
       if(ich==80)
          outfile2<<7000<<"  "<<0<<endl;
       else
          outfile2<<0<<"  "<<0<<endl;
    }
    for(int ich=0; ich<32; ich++)
       outfile2<<0<<"  "<<0<<endl;

    outfile2.close();

    ofstream outfile3("data/frame2.txt");
    for(int ich=0; ich<256; ich++)
       outfile3<<0<<"  "<<0<<endl;
    for(int ich=0; ich<32; ich++)
       outfile3<<0<<"  "<<0<<endl;

    outfile3.close();




}
