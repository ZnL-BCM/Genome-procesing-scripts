// genome_n2bed.c
// Finds all instances of N (missing base) in the human genome and return BED coordinates for them, 
// Useful when we need to generate a mask for this.
// Last updated: 7/21/2022

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
  printf("Input file: %s\n", argv[1]);
  printf("Output file: %s\n\n", argv[2]);

  // Define variables
  FILE *filePointer, *fileWriter;   // Pointers for filenames
  char ch;   // Object for text processing
  int FlagChrom = 0;   // Boolean flag for whether to write new chromosome name
  int FlagN = 0;   // Boolean flag for whether to start counting the location of Ns
  int Start, End;   // Integers to be written to output
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
      // Write coordinates to files if haven't done so
      if (FlagN == 1) {
	End = i;
	FlagN = 0;
	fprintf(fileWriter, "%s\t%d\t%d\n", chromname, Start, End);
      }
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
    
    // Finding locations of N
    else {

      if (ch == '\n') {continue;}   // Skip newline characters

      else {

	if (ch == 'N' && FlagN == 0) {  // Initializing N counting
	  Start = i;
	  FlagN = 1;
	}

	else if (ch != 'N' && FlagN == 1) {   // Ending N counting
	  End = i;
	  FlagN = 0;
	  fprintf(fileWriter, "%s\t%d\t%d\n", chromname, Start, End);
	}

	i++;   // Counting location

      }
    }
  }
  // Finish up the final region writeup if haven't done so
  if (FlagN == 1) {
    End = i;
    fprintf(fileWriter, "%s\t%d\t%d\n", chromname, Start, End);
  }


  // Close files
  fclose(filePointer);
  fclose(fileWriter);


  return 0;

}

