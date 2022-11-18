#include "Exception.h"

Exception::Exception(string newMessage){
	message=newMessage;
}
ostream &operator <<(ostream &out,const Exception &other){
	return out<<other.message<<endl;
}
