#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#define NO_OF_PROCESS 5
#define BUFSIZE 100 
#pragma warning(disable:4996)



float processTime[5] = { 300, 220, 180, 45,255 };

float processTimeCopy[5] = { 300, 220, 180, 45,255 };

int processSchedule[5] = { 0, 1, 2, 3, 4 }; //Child Process number

int mainFlag = 1; //This is like a flag 1 or 0 for while loop
int finishing = 0;



int main(int argc, char* argv[])
{
	STARTUPINFO si[NO_OF_PROCESS];
	PROCESS_INFORMATION pi[NO_OF_PROCESS];
	SECURITY_ATTRIBUTES sa[NO_OF_PROCESS];

	HANDLE ChildStd_IN_Wr[NO_OF_PROCESS];  //Handles for pipe, this is from my hw1
	HANDLE ChildStd_IN_Rd[NO_OF_PROCESS];
	HANDLE ChildStd_OUT_Wr[NO_OF_PROCESS];
	HANDLE ChildStd_OUT_Rd[NO_OF_PROCESS];
	HANDLE processHandles[NO_OF_PROCESS];

	char* lpCommandLine[NO_OF_PROCESS] = { "Child.exe 300","Child.exe 220","Child.exe 180","Child.exe 45","Child.exe 255" };
	int i = 0;

	for (i = 0; i < NO_OF_PROCESS; i++)
	{
		// Set the bInheritHandle flag so pipe handles are inherited. 
		SecureZeroMemory(&sa[i], sizeof(SECURITY_ATTRIBUTES));
		sa[i].bInheritHandle = TRUE;
		sa[i].lpSecurityDescriptor = NULL;
		sa[i].nLength = sizeof(SECURITY_ATTRIBUTES);
		// Create a pipe for the child process's STDOUT. 
		if (!CreatePipe(&ChildStd_OUT_Rd[i], &ChildStd_OUT_Wr[i], &sa[i], 0)) //use the default buffer size.
		{
			printf("Unable to create pipe\n");
			system("pause");
			exit(0);
		}
		//check errors 
		// Create a pipe for the child process's STDIN. 

		if (!CreatePipe(&ChildStd_IN_Rd[i], &ChildStd_IN_Wr[i], &sa[i], 0)) //use the default buffer size.
		{
			printf("Unable to create pipe\n");
			system("pause");
			exit(0);
		}
		//check errors 


		// Set up members of the STARTUPINFO structure. 
		// This structure specifies the STDIN and STDOUT handles for redirection
		// Allocate Memory

		SecureZeroMemory(&si[i], sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);
		si[i].hStdInput = ChildStd_IN_Rd[i];
		si[i].hStdOutput = ChildStd_OUT_Wr[i];
		si[i].hStdError = ChildStd_OUT_Wr[i];
		si[i].dwFlags = STARTF_USESTDHANDLES;


		// Allocate Memory
		SecureZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));
		if (!CreateProcess(NULL,
			lpCommandLine[i], // command line 
			NULL,  // process security attributes 
			NULL,  // primary thread security attributes
			TRUE, // handles are inherited
			CREATE_NEW_CONSOLE,  //Create no windows for the child process
			NULL, // use parent's environment 
			NULL, // use parent's current directory 
			&si[i],  // STARTUPINFO pointer 
			&pi[i])) // receives PROCESS_INFORMATION 
		{
			printf("Unable to Create child process: %d\n", i);
			system("pause");
			ExitProcess(0);
		}
		// Get a handle to an input file for the parent. 


		processHandles[i] = pi[i].hProcess;
	


	}




	while (mainFlag) {

		int size = 5;
		int i, j;
		float  temp;



		for (i = 0; i<size; i++)
		{
			/*
			* Place the currently selected element array[i]
			* to its correct place.
			*/
			for (j = i + 1; j<size; j++)
			{
				/*
				* Swap if currently selected array element
				* is not at its correct position.
				*/
				if (processTime[i] > processTime[j])
				{
					temp = processTime[i];
					processTime[i] = processTime[j];
					processTime[j] = temp;

				}
			}
		}

		/* Print the sorted array */
		//printf("\nElements of array in sorted ascending order: ");


		for (i = 0; i<size; i++)
		{
			for (j = 0; j<size; j++)
			{

				if (processTime[j] == processTimeCopy[i]) {  
					processSchedule[j] = i;
					
				}
			}
		}


		//From here all those above codes from while loop beginning for the sorting algorithm. It just sorts the arrays

		for (i = 0; i<size; i++)
		{
			printf("Process Scheduled by processes : (%d) \n", processSchedule[i]); //Print them out
		}


		for (i = 0; i<size; i++)
		{
			int order;

			int bytesToWrite = 0;
			int bytesWritten = 0;
			char message[100];
			bytesToWrite = strlen(message);
			bytesToWrite++;


			sprintf(message, "%d", i + 1);        
			order = processSchedule[i];   //After sorting array, order number decides which child process pipe will be used 


			printf("Process (%d) started\n", order);
			if (!WriteFile(ChildStd_IN_Wr[order], message, bytesToWrite, &bytesWritten, NULL)) //Just sent information that process pipe, it does not matter what we have sent. When child process receives it starts to run.I just sent i values in for loop
			{
				printf("Unable to write to pipe \n");
				system("pause");
				exit(0);
			}
		
			char ReadFromChildArray[BUFSIZE]; 
			int bytesToWritee;
			int bytesWrittenn = 0;
			bytesToWritee = strlen(ReadFromChildArray);
			bytesToWritee++;


			if (!ReadFile(ChildStd_OUT_Rd[order], ReadFromChildArray, bytesToWritee, &bytesWrittenn, NULL)) { //Try to read value from child process 
				printf("Trying to read from child process");
				exit(0);
		
			}
			
			float val = 0;
		
			val = atof(ReadFromChildArray);

			printf("Process (%d) Ended\n", order);

			printf("Comes from child process (%d) and calculated time is : %2.f\n", order, val);

			processTime[order] = val;

			processTimeCopy[order] = val;
		
		}
		printf( "FINISHING THE STEP %d\n", finishing);
		if (finishing == 5)
			break;
		finishing++;
	} 
	 
	system("pause");


	// Wait for the childs to exit 
	WaitForMultipleObjects(NO_OF_PROCESS, processHandles, TRUE, INFINITE);

	for (i = 0; i < NO_OF_PROCESS; i++)
	{
		//Close the Handles
		CloseHandle(ChildStd_IN_Rd[i]);
		CloseHandle(ChildStd_IN_Wr[i]);
		CloseHandle(ChildStd_OUT_Rd[i]);
		CloseHandle(ChildStd_OUT_Wr[i]);
		CloseHandle(pi[i].hThread);
		CloseHandle(pi[i].hProcess);
	}


	return 1;
}

