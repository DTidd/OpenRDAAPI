#include <cstdio>

extern "C" int c_main(int,char **);

int main( int argc, char* argv[] )
{
	std::printf("CPP_MAIN\n");
	c_main(argc,argv);
}
