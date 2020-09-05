//windows编程的案例
#include<Windows.h>
#include<stdio.h>
class CSomeObj
{
public:
	CSomeObj()
	{
		printf("Constrctor\r\n");
	}
	~CSomeObj()
	{
		printf("Destructor\r\n");
	}
};
CSomeObj g_GlobalObj;
void main(int argc, char* argv)
{
	CSomeObj LocalObj;
	ExitProcess(0);//
}