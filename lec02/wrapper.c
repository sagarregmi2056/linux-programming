#include <unistd.h>
int main(){
	char str[]={"welcome to the system programming \n"};
		int rv= write(1,str,sizeof str);
		return rv;
	}
	

