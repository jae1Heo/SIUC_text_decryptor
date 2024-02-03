#include "text_decryptor.h"
#include <Windows.h>
#include <vector>
#include <stack>
#include <iostream>
#include <queue>

#define BUF_LEN 2048
#define TOKEN_LEN 36
#pragma warning(disable:4996)

const int E = 12345;
using namespace std;

int main() {

	char buffer[BUF_LEN]; // buffer that receives data from opened file
	std::vector<char*>tokens; // vector storing tokenized data from array buffer
	std::vector<char*>line; // vector stores decrypted data
	std::stack<char*>tk; // temporary storage for code's letter in encrypted decimal and its decryptor
	char nToken[TOKEN_LEN]; // for order n
	char Token[TOKEN_LEN]; // completed token
	char _tToken[TOKEN_LEN]; // for each letter
	char _idxToken[TOKEN_LEN]; // for index returned

	const char* cutter = " "; // seperator that seperates each encrypted decimal from buffer
	HANDLE hRead; // handle that stores data of the file
	DWORD bRead = 0; // bytes read from the file
	hRead = CreateFileA("test.txt", GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hRead == INVALID_HANDLE_VALUE) {
		printf("Cannot open the file\n");
		return -1;
	}

	memset(buffer, 0, sizeof(char) * BUF_LEN);


	if (!ReadFile(hRead, buffer, BUF_LEN, &bRead, NULL)) {
		printf("Cannot read the file\n");
		return -1;
	}

	char* token = strtok(buffer, cutter); // tokenizing
	while (token != NULL) {
		tokens.push_back(token);
		token = strtok(NULL, cutter);
	}


	strcpy_s(nToken, TOKEN_LEN, tokens.at(tokens.size() - 1));
	DWORD N = _atoi64(nToken);
	int tokenIDX = 0; // for letter index
	int counter = 1; // for letter counter


	for (int i = 0; i <= tokens.size() -1; i++) {
		if (strcmp(tokens.at(i), "32")) { // if it is not the end of the line, 
			if (counter == 2) { // if current data is decryptor
				tk.push(tokens.at(i)); // push current data
				strcpy_s(_idxToken, sizeof(char) * TOKEN_LEN, tk.top()); // stores it to the temporary buffer for decryptor
				tk.pop(); // pop out
				strcpy_s(_tToken, sizeof(char) * TOKEN_LEN, tk.top()); // stores it to the temporary buffer for letter
				tk.pop(); // pop out
				Token[tokenIDX] = (_atoi64(_tToken) + (N * _atoi64(_idxToken))) / E; // decryption
				tokenIDX++; // increasing letter index
				Token[tokenIDX] = '\0'; // putting terminal 0 to the end of current string
				counter = 1; // reset to the counter to 1
			}
			else {
				tk.push(tokens.at(i)); // push current data
				counter++; // increase counter
			}
		}
		else {
			line.push_back(Token); // push completed data to the vector
			cout << Token << endl;
			tokenIDX = 0; // reseting letter index to 0
			memset(Token, 0, sizeof(char) * TOKEN_LEN); // reseting the memory
		}
	}
	
	CloseHandle(hRead);

	return 0;
}