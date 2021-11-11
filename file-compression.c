#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define bufferSize 255

int main( int argc, char* argv[] ){

	char* inputFile;
	char* outputFile;
	int cmpLen;
	int mode;

	inputFile = argv[1];
	outputFile = argv[2];
	cmpLen = atoi(argv[3]);
	mode = atoi(argv[4]);

	if (argc != 5) {
		perror("Not enough or too many command line arguments");
		exit(-1);
	}

	if (cmpLen < 1){
		if (errno == -1){
			perror("Comparison Length conversion error occurred");
			exit(-1);
		}
		perror("Compression length less than 1");
		exit(-1);
	}

	if (mode != 0 && mode != 1){
		if (errno == -1){
			perror("Mode conversion error occurred");
			exit(-1);
		}
		perror("Mode value other than 0 or 1");
		exit(-1);
	}

	int fileDescriptor1 = open( inputFile, O_RDONLY );
	if( fileDescriptor1 == -1 ){
		perror("Could not open input file");
		exit(-1);
	}

	int fileDescriptor2 = open( outputFile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR );
	if( fileDescriptor2 == -1 ){
		perror("Could not open output file");
		exit(-1);
	}

	if (mode == 0){
		char basis[cmpLen];
		char cmpBasis[cmpLen];
		int count = 1;
		ssize_t retVal = read(fileDescriptor1, basis, cmpLen);
		
		while(1){
			retVal = read(fileDescriptor1, cmpBasis, cmpLen);

			if (memcmp(basis, cmpBasis, cmpLen) == 0 && count != 255){
				count++;
			} else {
				write(fileDescriptor2, &count, 1);
				write(fileDescriptor2, basis, cmpLen);
				count = 1;
				memcpy(basis, cmpBasis, cmpLen);
			}

			if (retVal == 0){
				break;
			}
		}
		
	}
	else if (mode == 1){
		char reps[1];
		char basis[cmpLen];
		ssize_t retVal = read(fileDescriptor1, reps, 1);
		
		while(1){
			
			retVal = read(fileDescriptor1, basis, cmpLen);

			for (int i = 0; i <= reps[0]; i++){
				write(fileDescriptor2, basis, cmpLen);
			}

			retVal = read(fileDescriptor1, reps, 1);

			if(retVal == 0){
				break;
			}
		}
	}
	else{
		perror("Invalid mode value");
		exit(-1);
	}
	

	return 0;
}
