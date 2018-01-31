#include<iostream>
#include"pass_1.hpp"
#include"pass_2.hpp"
#include<unistd.h>

int main(int argc,char *argv[])
{
    int option;

    try{

      while((option = getopt(argc,argv,":io") != -1)) {}

      std::string inputfile(argv[optind]);
      std::string outputfile(argv[optind+1]);

      do_pass_1(inputfile,"intermediate.txt");
      gen_lst_file("intermediate.txt",outputfile);

      if(remove("intermediate.txt") != 0){}else{}
      
    }catch(std::logic_error){
      std::cout<<"Command line argument error..."<<std::endl<<"parameter expected"
          <<std::endl<<"./a.out -i <sourcefile(.asm)> -o <outputfile(.lst)>"<<std::endl;
    }

    return 0;
}
