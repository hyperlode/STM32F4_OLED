#include "IOBoard.h"
#include <stdio.h>

IOBoard::IOBoard(){


	printf("iniiiiiiittt");
	isConstructed = true;
}
void IOBoard::stats(char* outputString){

	if (isConstructed){
		outputString[0]= 'O';
	}else{
		outputString[0]= 'P';
	}
}

