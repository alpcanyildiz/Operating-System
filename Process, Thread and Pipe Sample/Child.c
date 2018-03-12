#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#define BUFSIZE 100 
#pragma warning(disable:4996)

//Alpcan YILDIZ 220201049
typedef struct
{
	int threadNo;
	int* sum;
	int day;

}THREAD_PARAMETERS;

DWORD WINAPI threadWork(LPVOID parameters);


int main(int argc, char* argv[])
{
	HANDLE hStdin, hStdout;
	CHAR ReadFromPipe[BUFSIZE];
	//Create handles and connect to parent process.
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);


	if ((hStdout == INVALID_HANDLE_VALUE) ||(hStdin == INVALID_HANDLE_VALUE))
	{
		printf("Could not do it");
		exit(0);
	}

	HANDLE* handles;
	THREAD_PARAMETERS* lpParameter;
	int* threadID;
	int i = 0;
	int threadCount = 0;
	int totalsum = 0;


	//check arguments for error
	if (argc != 2)
	{
		printf("error in child process...now exiting %s\n", argv[0]);
	
		exit(0);
	}

	//read thread count from arguments
	int childday;
	threadCount = atoi(argv[1]);




	char message[100];
	int bytesToWrite;
	int bytesWritten = 0;
	bytesToWrite = strlen(message);
	bytesToWrite++;

	// Read from standard input and stop on error or no data.
	ReadFile(hStdin, ReadFromPipe, BUFSIZE, &bytesToWrite, NULL);

	int val = 0;
	sscanf(ReadFromPipe, "%d", &val);
	childday = val;

	//allocate memory for every parameters needed
	handles = malloc(sizeof(HANDLE)* threadCount);
	lpParameter = malloc(sizeof(THREAD_PARAMETERS)* threadCount);
	threadID = malloc(sizeof(int)* threadCount);

	//for each thread
	for (i = 0; i < threadCount; i++)
	{
		
		//initialize parameters

		lpParameter[i].day = childday;
		lpParameter[i].threadNo = i;
		lpParameter[i].sum = 0;
		handles[i] = CreateThread(NULL, 0, threadWork, &lpParameter[i], 0, &threadID[i]);

		//check errors in creation
		if (handles[i] == INVALID_HANDLE_VALUE)
		{
			printf("error when creating thread\n");
			exit(0);
		}

		

	}
	//Wait for the threads to do their job.
	WaitForMultipleObjects(threadCount, handles, TRUE, INFINITE);


	for (i = 0; i < threadCount; i++)

	{
		CloseHandle(handles[i]);

	}
	//Some way to write to pipe. Because of I can not send int , I converted them to a char and merged them.
	char ArrayToWrite[100];
	char append[2]; //2 is enough to write.
	sprintf(ArrayToWrite, "%d%d%d%d%d",childday, (int)lpParameter[0].sum, (int)lpParameter[1].sum, (int)lpParameter[2].sum, (int)lpParameter[3].sum);
	bytesToWrite = 0;
	bytesWritten = 0;
	bytesToWrite = strlen(ReadFromPipe);
	bytesToWrite++;

	// Write to standard output and stop on error.
	if (!WriteFile(hStdout, ArrayToWrite, 100, &bytesWritten, NULL)); {
		printf("Can not Write To File");
	}

	//Free handles.
	free(handles);
	free(lpParameter);
	free(threadID);

	CloseHandle(hStdout);
	CloseHandle(hStdin);
	return 1;
}



//Thread function.
DWORD WINAPI threadWork(LPVOID parameters)
{
	THREAD_PARAMETERS* param = (THREAD_PARAMETERS*)parameters;
	//Varibles for each thread.
	int sum_milk = 0;
	int sum_coke = 0;
	int sum_biscuit = 0;
	int sum_chips = 0;
	//Reads the file it is not that complicated samething for each day.
	char c[5000];
	FILE *fptr;

	if ((fptr = fopen("C:\\Users\\UlpJuan\\Desktop\\market.txt","r")) == NULL)    //I did not find a way to read just market.txt We just need to give a specific
	{
		printf("Error! Can not open File ");
		exit(1);
	}


	while (!feof(fptr)) {
		fgets(c, 5000, fptr);
		char * pch;
		pch = strtok(c, "(#)");

		if (param->day == 1 && (strcmp("START DAY 1", pch) == 0)) {

		

			while (!(strcmp("END", pch) == 0))
			{
				fgets(c, 5000, fptr);
				char * pch;

				pch = strtok(c, "( #,)");
				while (pch != NULL) {
					if (strcmp("END", pch) == 0)
						break;
				
					if (param->threadNo == 0 && (strcmp("MILK", pch) == 0)) {
						sum_milk++;
						
					}

					else if (param->threadNo == 1 && (strcmp("BISCUIT", pch) == 0)) {
						sum_biscuit++;
						
					}
					else if (param->threadNo == 2 && (strcmp("CHIPS", pch) == 0)) {
						
						sum_chips++;
					}
					else if (param->threadNo == 3 && (strcmp("COKE", pch) == 0)) {
						sum_coke++;
					}
				
					pch = strtok(NULL, "( ,)");

				}
				
			}
			

		}

		else if (param->day == 2 && (strcmp("START DAY 2", pch) == 0)) {



			while (!(strcmp("END", pch) == 0))
			{
				fgets(c, 5000, fptr);
				char * pch;

				pch = strtok(c, "( #,)");
				while (pch != NULL) {
					if (strcmp("END", pch) == 0)
						break;

					if (param->threadNo == 0 && (strcmp("MILK", pch) == 0)) {
						sum_milk++;

					}

					else if (param->threadNo == 1 && (strcmp("BISCUIT", pch) == 0)) {
						sum_biscuit++;

					}
					else if (param->threadNo == 2 && (strcmp("CHIPS", pch) == 0)) {

						sum_chips++;
					}
					else if (param->threadNo == 3 && (strcmp("COKE", pch) == 0)) {
						sum_coke++;
					}

					pch = strtok(NULL, "( ,)");

				}

			}


		}
		else if (param->day == 3 && (strcmp("START DAY 3", pch) == 0)) {



			while (!(strcmp("END", pch) == 0))
			{
				fgets(c, 5000, fptr);
				char * pch;

				pch = strtok(c, "( #,)");
				while (pch != NULL) {
					if (strcmp("END", pch) == 0)
						break;

					if (param->threadNo == 0 && (strcmp("MILK", pch) == 0)) {
						sum_milk++;

					}

					else if (param->threadNo == 1 && (strcmp("BISCUIT", pch) == 0)) {
						sum_biscuit++;

					}
					else if (param->threadNo == 2 && (strcmp("CHIPS", pch) == 0)) {

						sum_chips++;
					}
					else if (param->threadNo == 3 && (strcmp("COKE", pch) == 0)) {
						sum_coke++;
					}

					pch = strtok(NULL, "( ,)");

				}

			}


		}
		else if (param->day == 4 && (strcmp("START DAY 4", pch) == 0)) {



			while (!(strcmp("END", pch) == 0))
			{
				fgets(c, 5000, fptr);
				char * pch;

				pch = strtok(c, "( #,)");
				while (pch != NULL) {
					if (strcmp("END", pch) == 0)
						break;

					if (param->threadNo == 0 && (strcmp("MILK", pch) == 0)) {
						sum_milk++;

					}

					else if (param->threadNo == 1 && (strcmp("BISCUIT", pch) == 0)) {
						sum_biscuit++;

					}
					else if (param->threadNo == 2 && (strcmp("CHIPS", pch) == 0)) {

						sum_chips++;
					}
					else if (param->threadNo == 3 && (strcmp("COKE", pch) == 0)) {
						sum_coke++;
					}

					pch = strtok(NULL, "( ,)");

				}

			}


		}
		else if (param->day == 5 && (strcmp("START DAY 5", pch) == 0)) {



			while (!(strcmp("END", pch) == 0))
			{
				fgets(c, 5000, fptr);
				char * pch;

				pch = strtok(c, "( #,)");
				while (pch != NULL) {
					if (strcmp("END", pch) == 0)
						break;

					if (param->threadNo == 0 && (strcmp("MILK", pch) == 0)) {
						sum_milk++;

					}

					else if (param->threadNo == 1 && (strcmp("BISCUIT", pch) == 0)) {
						sum_biscuit++;

					}
					else if (param->threadNo == 2 && (strcmp("CHIPS", pch) == 0)) {

						sum_chips++;
					}
					else if (param->threadNo == 3 && (strcmp("COKE", pch) == 0)) {
						sum_coke++;
					}

					pch = strtok(NULL, "( ,)");

				}

			}


		}
		else if (param->day == 6 && (strcmp("START DAY 6", pch) == 0)) {



			while (!(strcmp("END", pch) == 0))
			{
				fgets(c, 5000, fptr);
				char * pch;

				pch = strtok(c, "( #,)");
				while (pch != NULL) {
					if (strcmp("END", pch) == 0)
						break;

					if (param->threadNo == 0 && (strcmp("MILK", pch) == 0)) {
						sum_milk++;

					}

					else if (param->threadNo == 1 && (strcmp("BISCUIT", pch) == 0)) {
						sum_biscuit++;

					}
					else if (param->threadNo == 2 && (strcmp("CHIPS", pch) == 0)) {

						sum_chips++;
					}
					else if (param->threadNo == 3 && (strcmp("COKE", pch) == 0)) {
						sum_coke++;
					}

					pch = strtok(NULL, "( ,)");

				}

			}


		}

		else if (param->day == 7 && (strcmp("START DAY 7", pch) == 0)) {



			while (!(strcmp("END", pch) == 0))
			{
				fgets(c, 5000, fptr);
				char * pch;

				pch = strtok(c, "( #,)");
				while (pch != NULL) {
					if (strcmp("END", pch) == 0)
						break;

					if (param->threadNo == 0 && (strcmp("MILK", pch) == 0)) {
						sum_milk++;

					}

					else if (param->threadNo == 1 && (strcmp("BISCUIT", pch) == 0)) {
						sum_biscuit++;

					}
					else if (param->threadNo == 2 && (strcmp("CHIPS", pch) == 0)) {

						sum_chips++;
					}
					else if (param->threadNo == 3 && (strcmp("COKE", pch) == 0)) {
						sum_coke++;
					}

					pch = strtok(NULL, "( ,)");

				}

			}


		}



	}


	fclose(fptr); //Close File

	//Assign the values to thread sum.
	if (param->threadNo == 0) {
		param->sum = sum_milk;
	}
	else if (param->threadNo == 1) {
		param->sum = sum_biscuit;
	}
	else if (param->threadNo == 2) {
		param->sum = sum_chips;
	}
	else if (param->threadNo == 3) {
		param->sum = sum_coke;
	}
	

	return 1;
}


