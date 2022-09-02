// getcpgs.c
// Gets all instances of CpG dinucleotides in a given human genome, and return their BED coordinates

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100   // Max length of the name of a chromosome, modify if this is too short



// Main
int main(int argc, char *argv[]) {

  // Make sure command line inputs are correct
  if (argc != 3) {
    printf("Incorrect number of arguments given! Make sure to include \n1) input fasta file name and \n2) output bed file name.\n");
    return 1;
  } 
  printf("Genome input: %s\n", argv[1]);
  printf("Output BED file: %s\n\n", argv[2]);

  // Define variables
  FILE *filePointer, *fileWriter;   // Pointers for filenames
  char ch;   // Object for text processing
  int FlagChrom = 0;   // Boolean flag for whether to write new chromosome name
  int FlagCpG = 0;   // Boolean flag for whether to start counting the location of CpGs
  int Start;   // Integer to be written to output
  int i = 0;   // Iterator
  char chromname[MAX_NAME_LEN];   // Chromosome names


  // Process input file
  filePointer = fopen(argv[1], "r");
  fileWriter = fopen(argv[2], "w");
  // Exit if file is not found
  if (filePointer == NULL || fileWriter == NULL) {
    printf("File is not available! Exiting now.\n");
    return 1;
  }


  // Read contents until end of file (EOF)
  while ((ch = fgetc(filePointer)) != EOF) {

    // Detect chromosome name
    if (ch == '>') {
      FlagChrom = 1;   // Start writing a new chromosome name
      i = 0;
      memset(chromname, 0, MAX_NAME_LEN);   // Clears chromosome name array
    }

    // Making the chromosome name
    else if (FlagChrom == 1) {
      if (ch == '\n') {   // Stops writing name when new line occurs
        FlagChrom = 0;
	i = 0;
      }
      else {
	chromname[i] = ch;
	i++;
      }
    }
    
    // Finding locations of CG adjacent to each other
    else {

      if (ch == '\n') {continue;}   // Skip newline characters

      else {

	if (i == Start+1 && FlagCpG == 1) {  // Check if the next base is G if counting initiated
	  if (ch == 'G') {
	    fprintf(fileWriter, "%s\t%d\t%d\n", chromname, Start, Start+2);
	  }
	  FlagCpG = 0;
	}

	if (ch == 'C' && FlagCpG == 0) {  // Initializing CpG counting
	  Start = i;
	  FlagCpG = 1;
	}

	i++;   // Counting location

      }
    }
  }

  // Close files
  fclose(filePointer);
  fclose(fileWriter);


  return 0;

}


