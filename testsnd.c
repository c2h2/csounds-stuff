#include <stdio.h>
#include <sndfile.h>
#include <stdlib.h>
#include <string.h>


void print_info( SF_INFO sfinfo, char* fn){
  printf("Found Sound File: %s \n", fn);
  printf("Channels     : %d\n", sfinfo.channels);
  printf("Sampling Rate: %d\n", sfinfo.samplerate);
  printf("Samples      : %d\n", sfinfo.frames); 
  printf("Format       : %d\n", sfinfo.format); 
}

void read_file( SNDFILE * file){
  //double *tmp=malloc(sizeof(double)*44100);
  float *tmp=malloc(sizeof(float)*44100*2);  

  int i=0;
  int j=0;
  for(i=0; i< 100 ; i++){
    sf_readf_float(file, tmp, 44100);
    for(j=0; j< 44100; j++){
      printf("%f,",tmp[j]);
    }
    printf("\n");
  }

}



int main(int argc, char ** argv){
  SF_INFO sfinfo;
  SNDFILE * file;
  file = sf_open (argv[1], SFM_READ, &sfinfo);  
  
  print_info(sfinfo, argv[1]);
  read_file(file);
  
  exit(0);
}

