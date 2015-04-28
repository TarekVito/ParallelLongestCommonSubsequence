#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <conio.h>
#include <omp.h>

using namespace std;

#define N 10000
int DP[N][N];
int DP_Parallel[N][N];
char lcs_String[N];

/* Read text from a file */
string readFile(string fileName)
{
	string text = "";
	ifstream myfile(fileName + ".txt");
	if (myfile.is_open())
	{
		string line;
		while (getline(myfile, line))
		{
			text += line;
			text += "@";
		}
		text.erase(text.size()-1, 1);
		myfile.close();

		return text;
	}

}

/* Returns length of LCS */
int calculateLCS(string X, string Y)
{
	for (int i = 0; i <= X.length(); i++)
	{
		for (int j = 0; j <= Y.length(); j++)
		{
			if (i == 0 || j == 0)
				DP[i][j] = 0;

			else if (X[i - 1] == Y[j - 1])
				DP[i][j] = DP[i - 1][j - 1] + 1;

			else
				DP[i][j] = max(DP[i - 1][j], DP[i][j - 1]);
		}
	}
	return DP[X.length()][Y.length()];
}

int calculateLCS_Parallel(string X, string Y,int numThreads)
{
	//The idea is that the elements of the same anti-diagonal can be computed independently therefore it could be threaded
	//This for loop moves along the upper and right side of the 2D array .. referring to the initial element of anti-diagonal
	for (int i=0,j = 0; j <= Y.length() && i <= X.length(); j++)
	{
		
		int sz = min(j,(int)X.length()-i);						//The size of the anti-diagonal
		#pragma omp parallel for num_threads(numThreads)
		for(int k=0;k<=sz;++k)									//for each anti-diagonal element
		{
			int ii=i+k,jj=j-k;									//Calculating element position
			if (ii == 0 || jj == 0)
				DP_Parallel[ii][jj] = 0;

			else if (X[ii - 1] == Y[jj - 1])
				DP_Parallel[ii][jj] = DP_Parallel[ii - 1][jj - 1] + 1;

			else
				DP_Parallel[ii][jj] = max(DP_Parallel[ii - 1][jj], DP_Parallel[ii][jj - 1]);
		}
		if(j>=Y.length())								//If the upper side is finished .. increment the right side
			j=Y.length()-1,i++;
	}
	return DP_Parallel[X.length()][Y.length()];
}

/* Storing the LCS string in char array */
void lcsString(string X, string Y)
{
	int index = DP[X.size()][Y.size()];
	lcs_String[index] = '\0'; // Set the terminating character
	// Start from the right-most-bottom-most corner and
	// one by one store characters in lcs[]
	int i = X.size(), j = Y.size();
	while (i > 0 && j > 0)
	{
		// If current character in X[] and Y are same, then
		// current character is part of LCS
		if (X[i - 1] == Y[j - 1])
		{
			lcs_String[index - 1] = X[i - 1]; // Put current character in result
			i--; j--; index--;     // reduce values of i, j and index
		}

		// If not same, then find the larger of two and
		// go in the direction of larger value
		else if (DP[i - 1][j] > DP[i][j - 1])
			i--;
		else
			j--;
	}

}

/* Driver program to test above function */
int main()
{
	string string1, string2;
	printf("Read File? (y/n)\n");
	char ch;
	cin >> ch;
	printf("\n");
	if (ch == 'y')
	{
		string file;
		cout << "Enter file name:\n";
		cin.ignore();
		getline(cin,file);
		string1 = readFile(file);
		int delimPos = string1.find("@");
		string2 = string1.substr(delimPos + 1);
		string1.erase(delimPos, string2.size() + 1);
		cout << "String 1: " << string1 << "\n";
		cout << "String 2: " << string2 << "\n\n";
	}
	else if (ch == 'n')
	{
		int sz = 0;
		printf("Enter length of string: ");
		cin >> sz;
		printf("Generating random strings...\n\n");
		for (int i = 0; i < sz; ++i)
			if (!(rand() % 4))
				string1 += " ";
			else
				string1 += rand() % 26 + 'a';

		int startPos = rand() % (sz - 3);
		string2 = string1.substr(startPos, string1.size() - startPos - rand() % (startPos - 3));

		//cout << "String 1: " << string1 << "\n";
		//cout << "String 2: " << string2 << "\n\n";
	}

	double t = omp_get_wtime();

	int LCS_Parallel = calculateLCS_Parallel(string1, string2,4);
	cout << "Longest Common Subsequence = " << LCS_Parallel << "\n";
	cout << "Using OpenMP .. Time : " << omp_get_wtime() - t << endl << endl;
	t = omp_get_wtime();
	// Computing LCS
	int LCS = calculateLCS(string1, string2);
	cout << "Longest Common Subsequence = " << LCS << "\n";
	
	cout << "Without Threading .. Time : " << omp_get_wtime() - t << endl << endl;

	// Print the LCS String
	//lcsString(string1, string2);
	//cout << "LCS of " << string1 << " and " << string2 << " is " << lcs_String << endl;

	_getch();
	return 0;
}