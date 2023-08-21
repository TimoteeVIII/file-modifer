#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// NOTE: two additional operations are 'rename file' and 'replace line'

// checks if the user wants to overwrite a file, returns 1 if yes, 0 if no
int overwriteFile(char *fileName)
{
  int choice = 0;
  // make sure file exists first
  if(access(fileName, F_OK) == 0)
  {
    printf("File with name '%s' already exists, are you sure you want to continue (enter '0' for no, '1' for yes (if nothing is entered, or an erroneous response is entered, it is assumed you don't want the file to be created))?\n", fileName);
    scanf("%d", &choice);
  }
  if(choice == 1)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
// checks if file exists, returns 1 if exists, 0 if it doesn't
int fileExists(char *fileName)
{
  if(access(fileName, F_OK) == 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
// confirms whether user wants to delete things - e.g. lines or files, returns 1 if yes, 0 if no
int confirmDelete()
{
  int choice = 0;
  printf("If you want to follow through with the deleting, choose 1, any other input will be interpreted as your unwillingness to not delete...\n");
  scanf("%d", &choice);
  if(choice == 1)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
// returns the number of lines in a given file, if incorrect file given, program is exited
int showNumOfLines(char *fileName)
{
  FILE *fp;
  int counter = 0;
  char c;
  // checks file to find lines of exists
  if(fileExists(fileName) == 0)
  {
    printf("File doesn't exist, exiting program...\n");
    exit(0);
  }
  fp = fopen(fileName, "r");
  // every time a new line character '\n' is found, a counter is incremented
  while((c = fgetc(fp)) != EOF)
  {
    if(c=='\n')
    {
      counter++;
    }
  }
  fclose(fp);
  return counter;
}
// renames a file from its original name, to its new name, returns 1 if successful, exits otherwise
int renameFile(char *oldFileName, char *newFileName)
{
  // below ifs to pinpoint why file not renamed - multiple ifs so user knows exactly what they did incorrectly
  if(strcmp(oldFileName, "changeLog.txt") == 0 || strcmp(newFileName, "changeLog.txt") == 0)
  {
    printf("Cannot rename 'changeLog.txt' or change file to have the name: 'changeLog.txt', exiting program...\n");
    exit(0);
  }
  if(fileExists(oldFileName) == 0)
  {
    printf("File to rename doesn't exist, exiting program...\n");
    exit(0);
  }
  if(fileExists(newFileName) == 1)
  {
    if(overwriteFile(newFileName) == 0)
    {
      printf("You didn't want to overwrite an existing file, exiting program...\n");
      exit(0);
    }
  }
  rename(oldFileName, newFileName);
  printf("File '%s' renamed to '%s'\n", oldFileName, newFileName);
  return 1;
}

// creates a file with the desired name, returns 1 if file created, exits otherwise
int createFile(char *fileName)
{
  FILE *fp;
  // can't create file called changeLog.txt
  if(strcmp(fileName, "changeLog.txt") == 0)
  {
    printf("Cannot create a file with name 'changeLog.txt', exiting program...\n");
    exit(0);
  }
  // makes sure user wants to overwrite file if it already exists
  if(overwriteFile(fileName) == 0 && fileExists(fileName) != 0)
  {
    printf("File not created due to the desire to not overwrite an existing file, exiting program...\n");
    exit(0);
  }
  fp  = fopen(fileName, "w");
  fclose (fp);
  printf("File created...\n");
  return 1;
}

// copies contents from one file to another, returns 1 if successful, exits otherwise
int copyFile(char *sourceFile, char *targetFile)
{
  FILE *source, *target;
  char c;
  // can't copy to changeLog
  if(strcmp(targetFile, "changeLog.txt") == 0)
  {
    printf("Cannot copy contents to 'changeLog.txt', exiting program...\n");
    exit(0);
  }
  // make sure file exists
  if(fileExists(sourceFile) == 0)
  {
    printf("Source file doesn't exist, exiting program...\n");
    exit(0);
  }
  // make sure that if the file exists, the user wants to overwrite it
  if(overwriteFile(targetFile) == 0 && fileExists(targetFile) != 0)
  {
    printf("You didn't want to overwrite the target file, exiting program...\n");
    exit(0);
  }
  source = fopen(sourceFile, "r");
  target = fopen(targetFile, "w");
  // move every character of the source file to the destination (target file)
  while((c = fgetc(source)) != EOF)
  {
    fputc(c, target);
  }
  fclose(source);
  fclose(target);
  printf("Contents of file '%s' copied to file '%s'\n", sourceFile, targetFile);
  return 1;
}
// deletes file, returns 1 if successful, exits otherwise
int deleteFile(char *fileName)
{
  // check file exists
  if(fileExists(fileName) == 0)
  {
    printf("File doesn't exist, exiting program...\n");
    exit(0);
  }
  // make sure user wants to delete the file
  if(confirmDelete() == 0)
  {
    printf("You didn't want to delete the file, exiting program...\n");
    exit(0);
  }
  printf("File deleted...\n");
  remove(fileName);
  return 1;
}
// reads files line by line, returns 1 if successful, exits otherwise
int showFile(char *fileName)
{
  FILE *fp;
  char c;
  // make sure file exists
  if(fileExists(fileName) == 0)
  {
    printf("File doesn't exist, exiting program...\n");
    exit(0);
  }
  fp = fopen(fileName, "r");
  // print each character from the file specified
  while(c!=EOF)
  {
    printf("%c", c);
    c = fgetc(fp);
  }
  fclose(fp);
  return 1;
}
// appends a line of text to the end of a file, returns 1 if successful, exits otherwise
int appendLine(char *fileName, char *line)
{
  FILE *fp;
  // can't append to changeLog
  if(strcmp(fileName, "changeLog.txt") == 0)
  {
    printf("Cannot append to changeLog.txt, exiting program...\n");
    exit(0);
  }
  // makes sure file exists
  if(fileExists(fileName) == 0)
  {
    printf("File to append to doesn't exist, exiting program...\n");
    exit(0);
  }
  fp = fopen(fileName, "a");
  fprintf(fp, "%s\n", line);;
  fclose(fp);
  printf("Line '%s' appended to file '%s'...\n", line, fileName);
  return 1;
}
// deletes a line number specified by the user, returns 1 if successful, exits otherwise (done by copying contents from one file to another, omitting the line specified, then renaming the temporary file to the original name)
int deleteLine(char *fileName, int lineNumToDelete)
{
  int currentLineNum = 1;
  char str[1000];
  FILE *oriFile, *tempFile;
  // can't delete line from changeLog
  if(strcmp(fileName, "changeLog.txt") == 0)
  {
    printf("Cannot delete line from changeLog.txt, exiting program...\n");
    exit(0);
  }
  // make sure file exists
  if(fileExists(fileName) == 0)
  {
    printf("File to delete from doesn't exist, exiting program...\n");
    exit(0);
  }
  // make sure line number entered is a valid line number
  if(lineNumToDelete<1)
  {
    printf("Entered a line number to delete that was too low, exiting program...\n");
    exit(0);
  }
  else if(showNumOfLines(fileName)<lineNumToDelete)
  {
    printf("You entered a line number which is too large, exiting program...\n");
    exit(0);
  }
  oriFile = fopen(fileName,"r");
  tempFile = fopen("temp.txt", "w");
  // make sure temporary file exists
  if(!tempFile)
  {
    printf("Temporary file doesn't exist, exiting program...\n");
    fclose(oriFile);
    fclose(tempFile);
    exit(0);
  }
  // make sure user wants to delete the line
  if(confirmDelete() == 0)
  {
    printf("You didn't want to delete the line, exiting program...\n");
    fclose(oriFile);
    fclose(tempFile);
    exit(0);
  }
  // copy all lines to temp file apart from the line to delete
  while(!feof(oriFile))
  {
    strcpy(str, "\0");
    fgets(str, 1000, oriFile);
    if(currentLineNum != lineNumToDelete)
    {
      fprintf(tempFile, "%s", str);
    }
    currentLineNum++;
  }
  fclose(oriFile);
  fclose(tempFile);
  printf("Line %d removed from file '%s'\n", lineNumToDelete, fileName);
  // use remove and rename instead of prior functions to bypass any confirmations
  remove(fileName);
  rename("temp.txt",fileName);
  return 1;
}
// insert line of text at line specified by user to a particular line in a particular file, returns 1 if successful, exits otherwise
int insertLine(char *fileName, int lineNumToInsert, char *line)
{
  FILE *oriFile, *temp;
  int lineCounter=1;
  char str[1000];
  // can't insert into changeLog
  if(strcmp(fileName, "changeLog.txt") == 0)
  {
    printf("Cannot insert line to changeLog.txt, exiting program...\n");
    exit(0);
  }
  // make sure file exists
  if(fileExists(fileName) == 0)
  {
    printf("File doesn't exist, exiting program...\n");
    exit(0);
  }
  // make sure valid line number is input
  if(lineNumToInsert<1)
  {
    printf("Line number entered too low, exiting program...\n");
    exit(0);
  }
  else if(lineNumToInsert>showNumOfLines(fileName))
  {
    printf("Line number entered too large, exiting program...\n");
    exit(0);
  }
  oriFile = fopen(fileName, "r");
  temp = fopen("temp.txt", "w");
  // if temp file can't be created, exit
  if(!temp)
  {
    printf("Cannot create temporary file, exiting program...\n");
    fclose(oriFile);
    fclose(temp);
    exit(0);
  }
  // copy each line to temp file, apart from line to insert, where the text wanted is inserted
  while(!feof(oriFile))
  {
    strcpy(str, "\0");
    fgets(str, 1000, oriFile);
    if(lineCounter == lineNumToInsert)
    {
      fputs(line, temp);
      fputc('\n', temp);
    }
    fputs(str, temp);
    lineCounter++;
  }
  fclose(oriFile);
  fclose(temp);
  // use remove and rename instead of prior functions to bypass any confirmations
  remove(fileName);
  rename("temp.txt", fileName);
  printf("Line '%s' inserted at line %d in file '%s'\n", line, lineNumToInsert, fileName);
  return 1;
}
// shows a specific line from a specified file, returns 1 if successful, exits otherwise
int showLine(char *fileName, int lineNumToShow)
{
  int currentLineNum=1;
  char str[1000];
  FILE *fp;
  // if file doesn't exist, exit program
  if(fileExists(fileName) == 0)
  {
    printf("File doesn't exist, exiting program...\n");
    exit(0);
  }
  // if too low a line number is asked to be shown, exit program
  if(lineNumToShow<1)
  {
    printf("You entered too little a line number, exiting program...\n");
    exit(0);
  }
  else if(lineNumToShow>showNumOfLines(fileName)) // if too large a line number is asked to be shown, exit program
  {
    printf("You entered too large a line number, exiting program...\n");
    exit(0);
  }
  fp = fopen(fileName, "r");
  // loop through the file with a line number counter, when that num is equal to the desired line number, output that line's contents
  while(!feof(fp))
  {
    strcpy(str, "\0");
    fgets(str, 1000, fp);
    if(currentLineNum==lineNumToShow)
    {
      printf("%s", str);
    }
    currentLineNum++;
  }
  fclose(fp);
  return 1;
}
// shows the changeLog by using the showFile function, always successful due to the file being created if it doesn't exist at the beginning of the program
int showChangeLog()
{
  showFile("changeLog.txt");
  return 1;
}
// replaces a single line's contents with different content specified by a user in a file, returns 1 if successful, exits otherwise
int replaceLine(char *fileName, int lineNumToReplace, char *line)
{
  // can't replace line in changeLog
  if(strcmp(fileName, "changeLog.txt") == 0)
  {
    printf("Cannot replace any line in changeLog.txt, exiting program...\n");
    exit(0);
  }
  if(deleteLine(fileName, lineNumToReplace) == 1)
  {
    // all functions that are written prior have the necessary checks for files existing, therefore no need to check if file exists here
    int isLastLine = showNumOfLines(fileName) + 1 - lineNumToReplace;
    // if we're replacing the last line, after deleting it, we just append a line, otherwise we can use the insert function prior
    if(isLastLine == 0)
    {
       if(appendLine(fileName, line) == 1)
       {
         printf("Line '%s' replaces line %d's contents in file '%s'\n", line, lineNumToReplace, fileName);
         return 1;
       }
    }
    if(insertLine(fileName, lineNumToReplace, line) == 1)
    {
      printf("Line '%s' replaces line %d's contents in file '%s'\n", line, lineNumToReplace, fileName);
      return 1;
    }
  }
  printf("Line not replaced, exiting program...\n");
  exit(0);
}
// gets text of a line that's about to be deleted - used in change log for replace line and delete line so contents of line that was there can be known
const char* getLine(char *fileName, int lineNumToGet)
{
  int currentLineNum=1;
  char str[1000];
  static char lineToReturn[1000];
  FILE *fp;
  // make sure file exists
  if(fileExists(fileName) == 0)
  {
    printf("File doesn't exist, exiting program...\n");
    exit(0);
  }
  // make sure a valid number of lines are entered
  if(lineNumToGet<1)
  {
    printf("You entered too little a line number, exiting program...\n");
    exit(0);
  }
  else if(lineNumToGet>showNumOfLines(fileName))
  {
    printf("You entered too large a line number, exiting program...\n");
    exit(0);
  }
  fp = fopen(fileName, "r");
  // when the correct line is found, copy the line to an array of chars, and return this
  while(!feof(fp))
  {
    strcpy(str, "\0");
    fgets(str, 1000, fp);
    if(currentLineNum==lineNumToGet)
    {
      strcpy(lineToReturn, str);
    }
    currentLineNum++;
  }
  fclose(fp);
  return lineToReturn;
}


int main(int argc, char *argv[]) // ensure closure of change log when incorrect num of arguments provided
{
  FILE *changeLog;
  // below is for the date and time to apply to the chane log so the time of changes can be applied to the change log
  int year, month, day, hours, minutes, seconds;
  time_t now;
  time(&now);
  struct tm *local = localtime(&now);
  hours = local->tm_hour;          // get hours since midnight (0-23)
  minutes = local->tm_min;         // get minutes passed after the hour (0-59)
  seconds = local->tm_sec;         // get seconds passed after minute (0-59)
  day = local->tm_mday;            // get day of month (1 to 31)
  month = local->tm_mon + 1;       // get month of year (0 to 11)
  year = local->tm_year + 1900;    // get year since 1900


  changeLog = fopen("changeLog.txt", "a");

  // the below if statements are generally the same, it checks what operation the user wants to do, checks the correct amount of arguments have been provided then performs the operation
  // a statement is then appended to the change log
  // if there are any specifics for me to add, they'll be in specific if statements
  if(strcmp(argv[1], "createFile") == 0)
  {
    if(argc != 3)
    {
      printf("You haven't provided the correct number of arguments - 2 required - 'createFile', file_name - exiting program...\n");
      fclose(changeLog);
      return 0;
    }
    if(createFile(argv[2]) == 1)
    {
      fprintf(changeLog, "%02d/%02d/%d - %02d:%02d:%02d | File '%s' created with %d lines...\n",day,month,year,hours,minutes, seconds, argv[2], showNumOfLines(argv[2]));
    }
  }

  if(strcmp(argv[1], "copyFile") == 0)
  {
    if(argc != 4)
    {
      printf("You haven't provided the correct number of arguments - 3 required - 'copyFile', old_file_name, copied_file_name - exiting program...\n");
      fclose(changeLog);
      return 0;
    }
    if(copyFile(argv[2], argv[3]) == 1)
    {
      fprintf(changeLog, "%02d/%02d/%d - %02d:%02d:%02d | The contents of file '%s' (which has %d lines) were copied to %s...\n",day,month,year,hours,minutes, seconds,argv[2], showNumOfLines(argv[2]), argv[3]);
    }
  }

  if(strcmp(argv[1], "deleteFile") == 0)
  {
    if(argc != 3)
    {
      printf("You haven't provided the correct number of arguments - 2 required - 'deleteFile', file_name - exiting program...\n");
      fclose(changeLog);
      return 0;
    }
    // store num of lines to display in changeLog
    int numOfLinesBeforeDeletion = showNumOfLines(argv[2]);
    if(deleteFile(argv[2]) == 1)
    {
      fprintf(changeLog, "%02d/%02d/%d - %02d:%02d:%02d | File '%s' deleted, it had %d lines...\n",day,month,year,hours,minutes, seconds,argv[2],numOfLinesBeforeDeletion);
    }
  }


  if(strcmp(argv[1], "showFile") == 0)
  {
    if(argc != 3)
    {
      printf("You haven't provided the correct number of arguments - 2 required - 'showFile', file_name - exiting program...\n");
      fclose(changeLog);
      return 0;
    }
    if(showFile(argv[2]) == 1)
    {
      fprintf(changeLog, "%02d/%02d/%d - %02d:%02d:%02d | File '%s' was displayed which has %d lines...\n",day,month,year,hours,minutes, seconds,argv[2], showNumOfLines(argv[2]));
    }
  }


  if(strcmp(argv[1], "appendLine") == 0)
  {
    if(argc != 4)
    {
      printf("You haven't provided the correct number of arguments - 3 required - 'appendLine', file_name, line_to_append - exiting program...\n");
      fclose(changeLog);
      return 0;
    }
    if(appendLine(argv[2], argv[3]) == 1)
    {
      fprintf(changeLog, "%02d/%02d/%d - %02d:%02d:%02d | The line '%s' was appended to file '%s'. This file now has %d lines...\n",day,month,year,hours,minutes, seconds,argv[3], argv[2], showNumOfLines(argv[2]));
    }
  }


  if(strcmp(argv[1], "deleteLine") == 0)
  {
    if(argc != 4)
    {
      printf("You haven't provided the correct number of arguments - 3 required - 'deleteLine', file_name, line_num_to_delete - exiting program...\n");
      fclose(changeLog);
      return 0;
    }
    // get the line of text that's going to be deleted, only if the file exists
    char prevLine[1000];
    if(fileExists(argv[2]) == 1)
    {
      strcpy(prevLine, getLine(argv[2], atoi(argv[3])));
    }
    // remove the new line at the end of the text
    prevLine[strcspn(prevLine, "\n")] = 0;
    if(deleteLine(argv[2], atoi(argv[3])) == 1)
    {
      fprintf(changeLog, "%02d/%02d/%d - %02d:%02d:%02d | Line %s with contents '%s' was deleted from file '%s'. This file now has %d lines...\n",day,month,year,hours,minutes, seconds, argv[3], prevLine, argv[2], showNumOfLines(argv[2]));
    }
  }


  if(strcmp(argv[1], "showLine") == 0)
  {
    if(argc != 4)
    {
      printf("You haven't provided the correct number of arguments - 3 required - 'showLine', file_name, line_num_to_show - exiting program...\n");
      fclose(changeLog);
      return 0;
    }
    if(showLine(argv[2], atoi(argv[3])) == 1)
    {
      fprintf(changeLog, "%02d/%02d/%d - %02d:%02d:%02d | Line %s (out of %d lines) from file '%s' was shown...\n",day,month,year,hours,minutes, seconds, argv[3], showNumOfLines(argv[2]), argv[2]);
    }
  }


  if(strcmp(argv[1], "insertLine") == 0)
  {
    if(argc != 5)
    {
      printf("You haven't provided the correct number of arguments - 4 required - 'insertFile', file_name, line_num_to_insert, text_to_insert - exiting program...\n");
      fclose(changeLog);
      return 0;
    }
    if(insertLine(argv[2], atoi(argv[3]), argv[4]) == 1)
    {
      fprintf(changeLog, "%02d/%02d/%d - %02d:%02d:%02d | The line '%s' was inserted at line %s in file '%s'. This file now has %d lines...\n",day,month,year,hours,minutes, seconds, argv[4], argv[3], argv[2], showNumOfLines(argv[2]));
    }
  }


  if(strcmp(argv[1], "showChangeLog") == 0)
  {
    if(argc != 2)
    {
      printf("You haven't provided the correct number of arguments - 1 required - 'showChangeLog' - exiting program...\n");
      fclose(changeLog);
      return 0;
    }
    fprintf(changeLog, "%02d/%02d/%d - %02d:%02d:%02d | Change log (with %d lines) viewed...\n",day,month,year,hours,minutes, seconds, showNumOfLines("changeLog.txt"));
    showChangeLog();
  }


  if(strcmp(argv[1], "showNumOfLines") == 0)
  {
    if(argc != 3)
    {
      printf("You haven't provided the correct number of arguments - 2 required - 'showNumOfLines', file_name - exiting program...\n");
      fclose(changeLog);
      return 0;
    }
    int numOfLines = showNumOfLines(argv[2]);
    if(numOfLines == 1)
    {
      printf("File '%s' has %d line\n", argv[2], numOfLines);
      fprintf(changeLog, "%02d/%02d/%d - %02d:%02d:%02d | File '%s' had its number of lines shown (%d line)...\n",day,month,year,hours,minutes, seconds, argv[2], showNumOfLines(argv[2]));
    }
    else if(numOfLines > -1)
    {
      printf("File '%s' has %d lines\n", argv[2], numOfLines);
      fprintf(changeLog, "%02d/%02d/%d - %02d:%02d:%02d | File '%s' had its number of lines shown (%d lines)...\n",day,month,year,hours,minutes, seconds, argv[2], showNumOfLines(argv[2]));
    }
  }


  if(strcmp(argv[1], "renameFile") == 0)
  {
    if(argc != 4)
    {
      printf("You haven't provided the correct number of arguments - 3 required - 'renameFile', old_file_name, new_file_name - exiting program...\n");
      fclose(changeLog);
      return 0;
    }
    if(renameFile(argv[2], argv[3]) == 1)
    {
      fprintf(changeLog, "%02d/%02d/%d - %02d:%02d:%02d | File '%s' (with %d lines) was renamed '%s'...\n",day,month,year,hours,minutes, seconds, argv[2], showNumOfLines(argv[3]), argv[3]);
    }
  }

  if(strcmp(argv[1], "replaceLine") == 0)
  {
    if(argc != 5)
    {
      printf("You haven't provided the correct number of arguments - 4 required - 'replaceLine', file_name, line_num_to_replace, text_to_replace - exiting program...\n");
      fclose(changeLog);
      return 0;
    }
    // get text of line that's going to be replaced if the file exists
    char prevLine[1000];
    if(fileExists(argv[2]) == 1)
    {
      strcpy(prevLine, getLine(argv[2], atoi(argv[3])));
    }
    // remove new line character at the end of the string
    prevLine[strcspn(prevLine, "\n")] = 0;
    if(replaceLine(argv[2], atoi(argv[3]), argv[4]) == 1)
    {
      fprintf(changeLog, "%02d/%02d/%d - %02d:%02d:%02d | File '%s' had line %s (out of %d lines) with contents '%s' replaced with the following line: '%s'\n",day,month,year,hours,minutes, seconds, argv[2], argv[3], showNumOfLines(argv[2]), prevLine, argv[4]);
    }
  }
  fclose(changeLog);
  return 0;
}
