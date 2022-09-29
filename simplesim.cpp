//***************************************************************************
//
//  sml.h
//  CSCI 241 Assignment 4
//
//  Created by Aaron Hall (z1980019)
//
//***************************************************************************

//On line 

#include "simplesim.h"
#include "sml.h"
#include <iostream>
#include <iomanip>
using namespace std;

simplesim::simplesim()
{
	//initiallizing all variables
	for (int i = 0; i < 100; i++)
		memory[i] = 7777;
	accumulator = 0;
	instruction_counter = 0;
	instruction_register = 0;
	operation_code = 0;
	operand = 0;
}

bool simplesim::load_program()
{
	int count = 0;
	int instruction = 0;

	while(cin >> instruction && instruction != -99999)
	{
		//checking for invalid word
		if (instruction < -9999 || instruction > 9999)
		{
			cout << "***ABEND: pgm load : invalid word ***\n";
			dump();
			exit(1);
		}

		//checking for too large program
		if (count < 100)
			memory[count] = instruction;
		else
		{
			cout << "*** ABEND: pgm load: pgm too large ***\n";
			dump();
			exit(1);
		}

		count++;
	}

	return true;
}

void simplesim::execute_program()
{
	bool done = false;
	bool branch = false;
	int input = 0;

	while (!done)
	{
		//check for addressability error.
		input = 0;
		//fetch instruction
		instruction_register = memory[instruction_counter];
		operation_code = instruction_register / 100;
		operand = instruction_register % 100;
		
		branch = false;
		//Execute instruction.
		switch (operation_code)
		{
		case READ:
			cin >> input;
			if(input > 9999 || input < -9999)
			{
			cout << "*** ABEND: illegal input ***\n";
			dump();
			exit(1);
			}
			else
			{
			memory[operand] = input;
			cout << "READ: " << showpos << setfill('0') << internal << setw(5) << input << "\n";
			}
			break;

		case WRITE:
			cout << showpos << setfill('0') << internal << setw(5) << memory[operand] << "\n";
			break;

		case STORE:
			accumulator = memory[operand];
			break;

		case LOAD:
			memory[operand] = accumulator;
			break;

		case ADD:
			if((accumulator += memory[operand]) > 9999)
			{
				cout << "*** ABEND: overflow ***\n";
				dump();
				exit(1);
			}
			if((accumulator += memory[operand]) < -9999)
			{
				cout << "*** ABEND: underflow ***\n";
				dump();
				exit(1);
			}
				accumulator += memory[operand];
			break;

		case SUBTRACT:
			if((accumulator -= memory[operand]) < -9999)
			{
				cout << "*** ABEND: underflow ***\n";
				dump();
				exit(1);
			}
			if((accumulator -= memory[operand]) > 9999)
			{
				cout << "*** ABEND: overflow ***\n";
				dump();
				exit(1);
			}
				accumulator -= memory[operand];
			break;

		case MULTIPLY:
			if((accumulator *= memory[operand]) < -9999)
			{
				cout << "*** ABEND: underflow ***\n";
				dump();
				exit(1);
			}
			if((accumulator *= memory[operand]) > 9999)
			{
				cout << "*** ABEND: overflow ***\n";
				dump();
				exit(1);
			}
				accumulator *= memory[operand];
			break;

		case DIVIDE:
			if(memory[operand] != 0)
				accumulator /= memory[operand];
			else
			{
				cout << "*** ABEND: attempted division by 0 ***\n";
				dump();
				exit(1);
			}
			break;

		case BRANCH:
			branch = true;
			instruction_counter = operand;
			break;

		case BRANCHZERO:
			branch = true;
			if(accumulator == 0)
				instruction_counter = memory[operand];
			else
				instruction_counter++;
			break;

		case BRANCHNEG:
			branch = true;
			if(accumulator < 0)
				instruction_counter = memory[operand];
			else
				instruction_counter++;
			break;

		case HALT:
			done = true;
			return;

		default:
			cout << "*** ABEND: invalid opcode ***\n";
			return;
		}

		if(!done && !branch)
			instruction_counter++;
	}

	//Successful termination message.
	cout << "*** simplesim execution terminated ***\n\n";

	//dump();
}

void simplesim::dump() const
{
	//going to need showpos for the words
	//setfill for the columns/rows

	cout << "REGISTERS:\n";
	cout << "accumulator:            " << setfill('0') << internal << setw(5) << showpos << accumulator << "\n";
	cout << "instruction_counter:    " << setfill('0') << internal << setw(2) << noshowpos << instruction_counter << "\n";
	cout << "instruction_register:   " << setfill('0') << internal << setw(5) << showpos << instruction_register << "\n";
	cout << "operation_code:         " << setfill('0') << internal << setw(2) << noshowpos << operation_code << "\n";
	cout << "operand:                " << setfill('0') << internal << setw(2) << noshowpos << operand << "\n\n";

	//setting up the top part of the table
	cout << "MEMORY:" << "\n";
	cout << setfill(' ') << internal << setw(8) << "0" << setw(6) << "1" << setw(6) << "2";
	cout << setw(6) << "3" << setw(6) << "4" << setw(6) << "5";
	cout << setw(6) << "6" << setw(6) << "7" << setw(6) << "8";
	cout << setw(6) << "9" << "\n";

	//adding + and - symbols to the words
	cout << showpos;
	
	//loops for each row of the table
	cout << " 0";
	for (int i = 0; i <= 9; i++)
		cout << " " << setfill('0') << internal << setw(5) << memory[i];
	cout << "\n";

	cout << "10";
	for (int i = 10; i <= 19; i++)
		cout << " " << setfill('0') << internal << setw(5) << memory[i];
	cout << "\n";

	cout << "20";
	for (int i = 20; i <= 29; i++)
		cout << " " << setfill('0') << internal << setw(5) << memory[i];
	cout << "\n";

	cout << "30";
	for (int i = 30; i <= 39; i++)
		cout << " " << setfill('0') << internal << setw(5) << memory[i];
	cout << "\n";

	cout << "40";
	for (int i = 40; i <= 49; i++)
		cout << " " << setfill('0') << internal << setw(5) << memory[i];
	cout << "\n";

	cout << "50";
	for (int i = 50; i <= 59; i++)
		cout << " " << setfill('0') << internal << setw(5) << memory[i];
	cout << "\n";

	cout << "60";
	for (int i = 60; i <= 69; i++)
		cout << " " << setfill('0') << internal << setw(5) << memory[i];
	cout << "\n";

	cout << "70";
	for (int i = 70; i <= 79; i++)
		cout << " " << setfill('0') << internal << setw(5) << memory[i];
	cout << "\n";

	cout << "80";
	for (int i = 80; i <= 89; i++)
		cout << " " << setfill('0') << internal << setw(5) << memory[i];
	cout << "\n";

	cout << "90";
	for (int i = 90; i <= 99; i++)
		cout << " " << setfill('0') << internal << setw(5) << memory[i];
	cout << "\n";

}

