#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#define NO_OF_PROCESS 7
#define BUFSIZE 100 
#pragma warning(disable:4996)
//Alpcan YILDIZ 220201049
int main(int argc, char* argv[])
{
	STARTUPINFO si[NO_OF_PROCESS];
	PROCESS_INFORMATION pi[NO_OF_PROCESS];
	SECURITY_ATTRIBUTES sa[NO_OF_PROCESS];

	HANDLE g_hChildStd_IN_Wr[NO_OF_PROCESS];
	HANDLE g_hChildStd_IN_Rd[NO_OF_PROCESS];
	HANDLE g_hChildStd_OUT_Wr[NO_OF_PROCESS];
	HANDLE g_hChildStd_OUT_Rd[NO_OF_PROCESS];
	HANDLE processHandles[NO_OF_PROCESS];

	char* lpCommandLine[NO_OF_PROCESS] = {"Child.exe 4 ","Child.exe 4 ","Child.exe 4 ","Child.exe 4 ","Child.exe 4 ","Child.exe 4 ","Child.exe 4"};
	int i = 0;

	for (i = 0; i < NO_OF_PROCESS; i++)
	{
		// Set the bInheritHandle flag so pipe handles are inherited. 
		SecureZeroMemory(&sa[i], sizeof(SECURITY_ATTRIBUTES));
		sa[i].bInheritHandle = TRUE;
		sa[i].lpSecurityDescriptor = NULL;
		sa[i].nLength = sizeof(SECURITY_ATTRIBUTES);
		// Create a pipe for the child process's STDOUT. 
		if (!CreatePipe(&g_hChildStd_OUT_Rd[i], &g_hChildStd_OUT_Wr[i], &sa[i], 0)) //use the default buffer size.
		{
			printf("Unable to create pipe\n");
			system("pause");
			exit(0);
		}
		//check errors 
		// Create a pipe for the child process's STDIN. 

		if (!CreatePipe(&g_hChildStd_IN_Rd[i], &g_hChildStd_IN_Wr[i], &sa[i], 0)) //use the default buffer size.
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
		si[i].hStdInput = g_hChildStd_IN_Rd[i];
		si[i].hStdOutput = g_hChildStd_OUT_Wr[i];
		si[i].hStdError = g_hChildStd_OUT_Wr[i];
		si[i].dwFlags = STARTF_USESTDHANDLES;

	
		// Allocate Memory
		SecureZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));
		if (!CreateProcess(NULL,
			lpCommandLine[i], // command line 
			NULL,  // process security attributes 
			NULL,  // primary thread security attributes
			TRUE, // handles are inherited
			CREATE_NO_WINDOW,  //Create no windows for the child process
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
			
			int bytesToWrite = 0;
			int bytesWritten = 0;
			char message[100];
			bytesToWrite = strlen(message);
			bytesToWrite++;
			sprintf(message, "%d", i + 1);

			
			// WriteFile function writes data to the specified file or input / output(I / O) device.
			// Send messages through pipe
		if (!WriteFile(g_hChildStd_IN_Wr[i], message, bytesToWrite, &bytesWritten, NULL))
				{
				
					printf("Unable to write to pipe \n");
					system("pause");
					exit(0);
				}
				//Error Check

		processHandles[i] = pi[i].hProcess;
		CloseHandle(g_hChildStd_IN_Wr[i]); // Close the pipe handle so the child process stops reading. 


	}


	// Wait for the childs to exit 
	WaitForMultipleObjects(NO_OF_PROCESS, processHandles, TRUE, INFINITE);
	
	//Create variables to calculate child process outputs and do our job.

	int TotalChips = 0;   
	int TotalMilk = 0;
	int TotalCoke = 0;
	int TotalBiscuit = 0;



	int chips;
	int milk;
	int coke;
	int biscuit;


	int l = 0;
	for (i = 0; i < NO_OF_PROCESS; i++) {
		milk = 0;
		biscuit = 0;
		coke = 0;
		chips = 0;
		//Initialize the variables to zero in the loop so each child process assign their number of products.
		
		char ReadFromChildArray[BUFSIZE];  //This is sample from our lab code.
		int bytesToWrite;
		int bytesWritten = 0;
		bytesToWrite = strlen(ReadFromChildArray);
		bytesToWrite++;

		// Read output from the child process's pipe for STDOUT
		// Stop when there is no more data. 

		// This parent process reads all the pipes in a loop and do the calculations.
		ReadFile(g_hChildStd_OUT_Rd[i], ReadFromChildArray, bytesToWrite, &bytesWritten, NULL);

		char *pNext;
		long OutputFromPipe;
		OutputFromPipe = strtol(ReadFromChildArray, &pNext, 10);  //This a code from sample on the C library, reads the pipe and converts char to LONG.

		int counter = 0;

		while (OutputFromPipe > 0)
		{
			int digit = OutputFromPipe % 10;  //This is for the digits in the array.
			OutputFromPipe /= 10;
			
			if (counter == 0) {
				coke = coke + digit;
				TotalCoke += coke;
			}
			else if (counter == 1) {
				chips = chips + digit;
				TotalChips += chips;
			}

			else if (counter == 2) {
				biscuit = biscuit + digit;
				TotalBiscuit += biscuit;
			}
			else if (counter == 3) {
				milk = milk + digit;
				TotalMilk += milk;
			}

			counter++;
		}

		
		printf("%d.Day of the Coke number  : %d\n", (i+1), coke);
		printf("%d.Day of the Chips number :  %d\n", (i+1), chips);
		printf("%d.Day of the Biscuit number  : %d\n", (i+1), biscuit);
		printf("%d.Day of the Milk number  : %d\n", (i+1), milk);
		printf("                 ---              \n");
		//Finding most sold items in a day for each.
		int myArray[4] = {coke,chips,biscuit,milk };
		int temp = 0; //This is for finding max elements
		int selected = 0;
		int y = 0;
		for (y = 0; y < 4; y++) {
			if (myArray[y] > temp) {
				temp = myArray[y];
				selected = y;
			}
		}

		if (selected == 0)
			printf("The most sold item in  %d.  DAY is COKE :%d\n", (i + 1), myArray[selected]);
		if (selected == 1)
			printf("The most sold item in  %d.  DAY is CHIPS :%d\n", (i + 1), myArray[selected]);
		if (selected == 2)
			printf("The most sold item in  %d.  DAY is BISCUIT :%d\n", (i + 1), myArray[selected]);
		if (selected == 3)
			printf("The most sold item in  %d.  DAY is MILK :%d\n", (i + 1), myArray[selected]);
		printf("                 ---              \n  ");
		printf("%d.Day of the Total Coke number  : %d\n", (i + 1), TotalCoke);
		printf("%d.Day of the Total Chips number  : %d\n", (i + 1), TotalChips);
		printf("%d.Day of the Total Biscuit number  : %d\n", (i + 1), TotalBiscuit);
		printf("%d.Day of the Total Milk number  : %d\n", (i + 1), TotalMilk);
		printf("                 ---              \n");
	
		
	}
	int myArrayy[4] = { TotalCoke, TotalChips, TotalBiscuit, TotalMilk };
	int temp2 = 0;
	int selected = 0;
	int y = 0;
	for (y = 0; y < 4; y++) {
		if (myArrayy[y] > temp2) {
			temp2 = myArrayy[y];
			selected = y;
		}
	}
	//Finding most sold item in the week.
	if (selected == 0)
		printf("Most sold item in this week is COKE by %d\n", myArrayy[selected]);
	if (selected == 1)
		printf("The most sold item in this week is CHIPS by %d\n", myArrayy[selected]);
	if (selected == 2)
		printf("The most sold item in this week is BISCUIT by %d\n", myArrayy[selected]);
	if (selected == 3)
		printf("The most sold item in this week is MILK by %d\n", myArrayy[selected]);

	system("pause");
	
	for (i = 0; i < NO_OF_PROCESS; i++)
	{
		//Close the Handles
		CloseHandle(g_hChildStd_IN_Rd[i]);
		CloseHandle(pi[i].hThread);
		CloseHandle(pi[i].hProcess);
	}


	return 1;
}

