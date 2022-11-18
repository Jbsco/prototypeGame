#pragma once

/* old tradition
#ifndef EXCEPTION_H
#define EXCEPTION */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Exception{
	public:
		string message;
		Exception(string newMessage);
		friend ostream &operator <<(ostream &out,const Exception &other);
};

/* old tradition
#endif */
