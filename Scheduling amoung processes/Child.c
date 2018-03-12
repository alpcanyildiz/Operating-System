#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#define BUFSIZE 100 
#pragma warning(disable:4996)

int counter = 0;
float predictedTime = 0;
float alpha = 0.5;

int main(int argc, char* argv[])
{
	HANDLE hStdin, hStdout;
	//Create handles and connect to parent process.
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);


	if ((hStdout == INVALID_HANDLE_VALUE) || (hStdin == INVALID_HANDLE_VALUE))
	{
		printf("Not able to create pipe ! ");
		exit(0);
	}



	//check arguments for error
	if (argc != 2)
	{
		printf("Error in child process...now exiting %s\n", argv[0]);
		exit(0);
	}
	float burstTime = atof(argv[1]);

	//read initial values from arguments, it is not that important because other informations will sent via pipe
	srand(time(NULL));
	while (1) {

		counter++; //This is for the 5 times

		char ReadFromPipe[BUFSIZE]; //Every time I initialize array to clear it up
		char message[100];
		int bytesToWrite;
		int bytesWritten = 0;
		bytesToWrite = strlen(message);
		bytesToWrite++;

		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		hStdin = GetStdHandle(STD_INPUT_HANDLE);


		// Read from standard input and stop on error or no data.
		if (!ReadFile(hStdin, ReadFromPipe, BUFSIZE, &bytesToWrite, NULL)) { //Read value from parent process //When it reads, it starts to run
			printf("Waiting from pipe");
			exit(0);
			break;

		}
		else {

			Sleep(2000);

			int random_number = rand() % 300 + 50; //This random number does not work fine


			predictedTime = alpha*random_number + (1 - alpha)*(burstTime);  //This is the formula 
			burstTime = predictedTime;




			char ArrayToWrite[1000];
			sprintf(ArrayToWrite, "%.2f", burstTime);
			bytesToWrite = 0;
			bytesWritten = 0;
			bytesToWrite = strlen(ArrayToWrite);

			// Write to standard output and stop on error.
			if (!WriteFile(hStdout, ArrayToWrite, bytesToWrite, &bytesWritten, NULL)) { //When estimates predicted value it will be sent with pipe 
				printf("Can not Write To File");
				exit(0);

			}

		}

		if (counter == 6) {		//To finish loop
			break;
		}

	}



	CloseHandle(hStdout);
	CloseHandle(hStdin);
	return 1;
}