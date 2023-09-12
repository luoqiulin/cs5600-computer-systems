#include <stdio.h>
#include <string.h>
#include "polybius.h"

int main(int argc, char *argv[]){
    table t = {{{1,1},{1,2},{1,3},{1,4},{1,5},
  	       {2,1},{2,2},{2,3},{2,4},{2,5},
  	       {3,1},{3,2},{3,3},{3,4},{3,5},
  	       {4,1},{4,2},{4,3},{4,4},{4,5},
  	       {5,1},{5,2},{5,3},{5,4},{5,5}},
  	       {'a','b','c','d','e','f','g','h',
  	       'i','j','k','l','m','n','o','p',
  	       'q','r','s','t','u','v','w','x',
  	       'y','z'}};
  
    if(strcmp(argv[1], "-e") == 0) pbEncode(argv[2], t);
    if(strcmp(argv[1], "-d") == 0) pbDecode(argv[2], t);
    return 0;
}


