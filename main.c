
#define _CRT_SECURE_NO_DEPRECATE
#define MAX 1000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
 * function that returns a number for each command
 * 0 -> SELECT
 * 1 -> DELETE 
 * 2 -> UPDATE
 * 3 -> INSERT
 * 4 -> CREATE
 * 5 -> DROP
 * -1 -> invalid input
 */
int get_command_number(char line[1000]);

/*
 * every table in the DB has two files
 * file for the table format and a file for the contents of the table
 * 
 * named table_name_content.txt  and table_name_format.txt
 */

typedef void(*Functions)();                 //declare typdef

void select_from_DB(char tokens[100][100] , int numOfTokens);
void delete_from_DB(char tokens[100][100], int numOfTokens);
void update_to_DB(char tokens[100][100], int numOfTokens);
void insert_to_DB(char tokens[100][100], int numOfTokens);
void create_DB(char tokens[100][100], int numOfTokens);
void drop_DB(char tokens[100][100], int numOfTokens);
void get_file_names(char tableName[], char *contentFile, char * formatFile);
void delete_lines(char *fileName, int lineNumber);
int convert_line_to_tokens(char* line, char  tokens[100][100]);
void parse_fields_for_creation(char* fields);
void get_field_and_value(char token[100], char field[50], char value[50]);
void get_matching_lines(char field[50], char value[50], int matching_lines[100], int* number_of_matching_lines);
// array of functions that holds the DB methods
Functions func[6] = { &select_from_DB, &delete_from_DB, &update_to_DB, &insert_to_DB, &create_DB , &drop_DB };

int main(int argc , char * argv [])
{
	char line[1000], tokens[100][100];
	
		
	fgets(line, 1000, stdin);
	int commandNumber = get_command_number(line);
	if(commandNumber == -1 )
	{
		puts("Invalid input");
	}
	else
	{
		int numOfTokens= convert_line_to_tokens(line, tokens);
		func[commandNumber](tokens ,  numOfTokens);
	}
	
	getchar();

	return 0;
}

char colNames[100][100] , contentFileName[100];
int numberOfCol;

/*
 * Load cols namess from the cols line
 */
int load_col_names(char * tableName )
{
	char forFName[50];
	get_file_names(tableName, contentFileName, forFName);
	FILE * formatFile = fopen(forFName, "r");

	fscanf(formatFile, "%d", &numberOfCol);  
	for (int i = 0; i < numberOfCol; ++i)
	{
		fscanf(formatFile, "%s", colNames[i]);
	}
	return 0;
}
int get_command_number(char line[1000])
{
	char  command[100];
	sscanf(line, "%s", command);

	if (strcmp(command, "SELECT") == 0)
	{
		return 0;
	}
	if (strcmp(command, "DELETE") == 0)
		return 1;
	if (strcmp(command, "UPDATE") == 0)
		return 2;
	if (strcmp(command, "INSERT") == 0)
		return 3;
	if (strcmp(command, "CREATE") == 0)
		return 4;
	if (strcmp(command, "DROP") == 0)
		return 5;

	return -1;

}

void print_matching_lines(int matching_lines[100], int number_of_matching_lines)
{
	char *fname = contentFileName;
	char str[MAX], temp[] = "temp.txt";
	
	char tokens[100][100];

	for (int i = 0; i < number_of_matching_lines; ++i)
	{

		int ctr = 0;
		int lno = matching_lines[i];
		FILE* fptr1 = fopen(fname, "r");
		if (!fptr1)
		{
			printf(" File not found or unable to open the input file!!\n");
			return;
		}

		// copy all contents to the temporary file except the specific line
		while (!feof(fptr1))
		{
			strcpy(str, "\0");
			fgets(str, MAX, fptr1);
			if (strcmp(str, "") == 0)
				break;


			/* update the line at given line number */
			if (ctr == lno)
			{
				printf("%s\n", str);
			}

			ctr++;

		}
		fclose(fptr1);
	}
		
}


void get_field_select(char token[100], char field1[50])
{
	//TableName.Field2
	int idx = 0;
	int len = strlen(token);
	int start = 0;
	for (int i = 0; i < len; ++i)
	{
		if(start)
		{
			field1[idx] = token[i];
			idx++;
		}
		if(token[i] == '.')
		{
			start = 1;
		}
	}
	field1[idx] = 0;
}

int load_cols_to_arr(char table_name[100]  , char cols_names[100][100])
{
	load_col_names(table_name);
	for (int i = 0; i < numberOfCol; ++i)
	{
		strcpy(cols_names[i], colNames[i]);

	}
	return numberOfCol;
}


int load_rows_to_arr(char tabl_name[100], char rows[100][100])
{
	FILE * cont_file = fopen(tabl_name, "r");
	int ctr=0;
	while (!feof(cont_file))
	{
		strcpy(rows[ctr], "\0");
		fgets(rows[ctr], MAX, cont_file);
		ctr++;
	}
	return ctr ;
}

int get_field_idx_select(char cols_names[100][100], char field[50] , int number_of_cols)
{
	for (int i = 0; i < number_of_cols; ++i)
	{
		if (strcmp(field, cols_names[i]) == 0)
			return i;
	}
	return -1;
}

void select_from_DB(char tokens[100][100], int numOfTokens)
{
	load_col_names(tokens[3]);
	char field[100], value[100];
	if(numOfTokens == 6)
	{
		int matching_lines[100], num_of_mat_lines;
		get_field_and_value(tokens[5], field, value);
		get_matching_lines(field, value, matching_lines, &num_of_mat_lines);
		print_matching_lines(matching_lines, num_of_mat_lines);
	}
	else if (numOfTokens == 10){
		
		char field1[50], field2[50];

		get_field_select(tokens[7], field1);
		get_field_select(tokens[9], field2);

		char lines1[100][100], lines2[100][100];
		char cols_names1[100][100], cols_names2[100][100];
		int num_of_cols1 = load_cols_to_arr(tokens[3], cols_names1);
		int num_of_cols2 = load_cols_to_arr(tokens[5] , cols_names2);
		int number_of_rows1 = load_rows_to_arr(tokens[3], lines1);
		int number_of_rows2 = load_rows_to_arr(tokens[5], lines2);

		char table1_tok[500][100][100], table2_tok[500][100][100];

		for (int i = 0; i < number_of_rows1; ++i)
		{
			convert_line_to_tokens(lines1[i], table1_tok[i]);
		}
		for (int i = 0; i < number_of_rows2; ++i)
		{
			convert_line_to_tokens(lines2[i], table2_tok[i]);
		}
		int field_idx1 = get_field_idx_select(cols_names1, field1 , num_of_cols1);
		int field_idx2 = get_field_idx_select(cols_names2 , field2, num_of_cols2);
		if(~field_idx1 || ~field_idx2)
		{
			return;
		}
		for (int i = 0; i < number_of_rows1; ++i)
		{
			for (int j = 0; j < number_of_rows2; ++j)
			{
				if(strcmp(table1_tok[i][field_idx1] , table2_tok[j][field_idx2]) == 0)
				{
					printf("%s\n%s\n", lines1[i], lines2[j]);
				}
			}
		}

	}
}

void get_field_and_value(char token[100], char field[50], char value[50])
{
	int idx = 0;
	int len = strlen(token);
	char * holder = field;
	strtok(token, "\n");
	for (int i = 0; i < len; ++i)
	{
		holder[idx] = token[i];
		idx++;
		if( idx>= 0&& holder[idx-1] == '=')
		{
			holder[idx-1] = 0;
			holder = value;
			idx = 0;
			i++;
		}
		if(idx >= 0 &&  holder[idx-1] == '\"')
		{
			holder[idx-1] = 0;
			break;
		}
		
	}

}

int get_field_idx(char field[50])
{
	for (int i = 0; i < numberOfCol; ++i)
	{
		if(strcmp(field, colNames[i]) == 0)
		{
			return i;
		}
	}
	return -1;
}

void get_matching_lines(char field[50], char value[50], int matching_lines[100], int* number_of_matching_lines)
{
	int fieldIdx = get_field_idx(field);
	FILE * contFile = fopen(contentFileName, "r");
	if(contFile == NULL)
	{
		puts("Table not found");
		exit(1);
	}
	int lno = 0, idx_mat_lines = 0, fIdx=0  ;
	char holder[50];
	*number_of_matching_lines = 0;
	while (fscanf(contFile , "%s" ,holder ) == 1)
	{
		if(fIdx == fieldIdx)
		{
			if(strcmp(holder, value) == 0)
			{
				matching_lines[idx_mat_lines] = lno;
				idx_mat_lines++;
				(*number_of_matching_lines)++;
			}
		}
		fIdx++;
		
		if (fIdx == numberOfCol) fIdx = 0, lno++;
	}
	fclose(contFile);
}

void delete_from_DB(char tokens[100][100], int numOfTokens)
{
	load_col_names(tokens[2]);
	char field[50], value[50];
	get_field_and_value(tokens[4], field, value);
	int matchingLines[100];
	int numberOfMatchingLines;
	get_matching_lines(field, value, matchingLines, &numberOfMatchingLines);
	for (int i = 0; i < numberOfMatchingLines; ++i)
	{
		delete_lines(contentFileName, matchingLines[i]);
	}
	puts("rows deleted");
}

void update_lines_with_new_value(int matching_lines[100], int number_of_matching_lines, char field[50], char value[50])
{
	char *fname = contentFileName;
	char str[MAX], temp[] = "temp.txt";
	int fIdx = get_field_idx(field);
	char tokens[100][100];
	
	for (int i = 0; i < number_of_matching_lines; ++i)
	{

		int ctr = 0;
		int lno = matching_lines[i];
		FILE* fptr1 = fopen(fname, "r");
		if (!fptr1)
		{
			printf(" File not found or unable to open the input file!!\n");
			return;
		}
		FILE* fptr2 = fopen(temp, "w"); // open the temporary file in write mode 
		if (!fptr2)
		{
			printf("Unable to open a temporary file to write!!\n");
			fclose(fptr1);
			return;
		}
		// copy all contents to the temporary file except the specific line
		while (!feof(fptr1))
		{
			strcpy(str, "\0");
			fgets(str, MAX, fptr1);
			if(strcmp(str , "")==0 )
				break;
			convert_line_to_tokens(str, tokens);
			

			/* update the line at given line number */
			if (ctr != lno)
			{
				for (int j = 0; j < numberOfCol; ++j)
				{
					
					fprintf(fptr2, "%s%c", tokens[j] , " \n"[j==numberOfCol-1]);
				}
				
				
			}
			else
			{
				strcpy(tokens[fIdx], value);
				for (int j = 0; j < numberOfCol; ++j)
				{
					
					fprintf(fptr2, "%s%c", tokens[j], " \n"[j == numberOfCol - 1]);
					

				}
				

				
			}
			ctr++;
			
		}
		fclose(fptr1);
		fclose(fptr2);
		remove(fname);  		// remove the original file 
		rename(temp, fname); 	// rename the temporary file to original name
	}
}

void update_to_DB(char tokens[100][100], int numOfTokens)
{

	load_col_names(tokens[1]);
	char field[50], value[50];
	get_field_and_value(tokens[5], field, value);
	int matchingLines[100];
	int numberOfMatchingLines;
	get_matching_lines(field, value, matchingLines, &numberOfMatchingLines);
	get_field_and_value(tokens[3], field, value);
	update_lines_with_new_value(matchingLines, numberOfMatchingLines, field, value);
}
void parse_fields_for_insertion(char* fields , char fields_arr [100][100], int  *num_of_fields)
{
	int len = strlen(fields), colIdx = 0, colCIdx = 0;
	*num_of_fields = 0;
	for (int i = 1; i < len; ++i)
	{
		fields_arr[colIdx][colCIdx] = fields[i];
		if (fields[i] == ',' || fields[i] == ']' || fields[i] == ')')
		{
			fields_arr[colIdx][colCIdx] = 0;
			colCIdx = -1;
			colIdx++;
			(*num_of_fields)++;
			i++;
		}
		colCIdx++;
	}
}
void insert_to_DB(char tokens[100][100], int numOfTokens)
{
	int num_of_fields;
	char fields_arr[100][100];
	parse_fields_for_insertion(tokens[3], fields_arr, &num_of_fields);

	char field[50], value[50] , cont_file[50] , for_file[50];
	FILE * contentFile;
	get_file_names(tokens[2], contentFileName, for_file);
	contentFile = fopen(contentFileName, "a"); // open the file append
	if(contentFile == NULL)
	{
		puts("Table no found");
		exit(0);
	}
	for (int i = 0; i < num_of_fields; ++i)
	{
		get_field_and_value(fields_arr[i], field, value);
		fprintf(contentFile, "%s%c", value, " \n"[i == num_of_fields - 1]);
	}
	fclose(contentFile);
}

void parse_fields_for_creation(char* fields)
{
	int len = strlen(fields), colIdx = 0, colCIdx=0;
	for (int i = 1; i < len; ++i)
	{
		colNames[colIdx][colCIdx] = fields[i];
		if (fields[i] == ',' || fields[i] == ']' || fields[i] == ')')
		{
			colNames[colIdx][colCIdx] = 0;
			colCIdx = -1;
			colIdx++;
			numberOfCol++;
			i++;
		}
		colCIdx++;
	}
}

void save_format(char fields[100], char * formatFileName)
{
	parse_fields_for_creation(fields);
	FILE * formatFile = fopen(formatFileName, "w");
	fprintf(formatFile, "%d\n", numberOfCol);
	for (int i = 0; i < numberOfCol; ++i)
	{
		fprintf(formatFile, "%s\n", colNames[i]);
	}
	fclose(formatFile);
}

void create_DB(char tokens[100][100], int numOfTokens)
{
	FILE * formatFile,* contentFile;
	char formatFName[50];
	get_file_names(tokens[2], contentFileName, formatFName);
	formatFile = fopen(formatFName, "w");
	fclose(formatFile);
	contentFile = fopen(contentFileName, "w");
	fclose(contentFile);

	save_format(tokens[4], formatFName);
	puts("DB Created");
}

void drop_DB(char tokens[100][100], int numOfTokens)
{
	char contFname[50], forFileName[50];
	int ret1, ret2;
	for (int i = 2; i < numOfTokens; ++i)
	{
		get_file_names(tokens[i], contFname, forFileName);
		ret1 = remove(contFname);
		
		ret2 = remove(forFileName);
		if (ret1 == 0 && ret2 == 0)
		{
			printf("%s deleted successfully\n", tokens[i]);
		}
	}

}

void get_file_names(char tableName[], char *contentFile , char * formatFile)
{
	strcpy(contentFile, tableName);
	strcat(contentFile, "_content.txt");

	strcpy(formatFile, tableName);
	strcat(formatFile, "_format.txt");

}

int convert_line_to_tokens(char* line, char tokens[100][100])
{
	int len = strlen(line);
	int tokIdx = 0;
	int tokWord = 0, idxAfter = -1;
	strtok(line, "\n");
	for (int i = 0; i < len; ++i)
	{
		tokens[tokWord][tokIdx] = line[i];
		
		if (line[i] == ' ' || line[i] == '\n' )
		{
			tokens[tokWord][tokIdx] = 0;
			tokIdx = -1;
			tokWord++;
		}
		tokIdx++;
		if(i == len -1)
		{
			tokens[tokWord][tokIdx] = 0;
			tokIdx = 0;
			tokWord++;
		}
		if (line[i + 1] == '(' || line[i + 1] == '[')
		{
			idxAfter = i + 1;
			break;
		}
		
	}
	if (idxAfter != -1)
	{
		for (int i = idxAfter; i < len; ++i)
		{
			tokens[tokWord][tokIdx] = line[i];
			tokIdx++;
		}
	}
	return  tokWord;
}


void delete_lines(char *fileName , int lineNumber)
{
	int lno, ctr = 0;
	
	FILE *fptr1, *fptr2;
	char *fname = fileName;
	char str[MAX], temp[] = "temp.txt";
	
	fptr1 = fopen(fname, "r");
	if (!fptr1)
	{
		printf(" File not found or unable to open the input file!!\n");
		return ;
	}
	fptr2 = fopen(temp, "w"); // open the temporary file in write mode 
	if (!fptr2)
	{
		printf("Unable to open a temporary file to write!!\n");
		fclose(fptr1);
		return ;
	}
	lno = lineNumber;
	// copy all contents to the temporary file except the specific line
	while (!feof(fptr1))
	{
		strcpy(str, "\0");
		fgets(str, MAX, fptr1);
		if (!feof(fptr1))
		{
			
			/* skip the line at given line number */
			if (ctr != lno)
			{
				fprintf(fptr2, "%s", str);
			}
			ctr++;
		}
	}
	fclose(fptr1);
	fclose(fptr2);
	remove(fname);  		// remove the original file 
	rename(temp, fname); 	// rename the temporary file to original name	

}
