#include "./app.hpp"

#define W 800
#define H 600

int main(int, char**)
{
	App app(W, H, "WebGPU Demo");
	app.Run();
    return 0;
}
