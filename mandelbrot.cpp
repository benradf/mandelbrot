#include <iostream>
#include <complex>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3) {
        cout << "usage: mandelbrot SIZE SCALE" << endl;
        return 1;
    }

    auto size = atoi(argv[1]);
    auto scale = atoi(argv[2]);

    cout << "size = " << size << endl;
    cout << "scale = " << scale << endl;




    return 0;
}
