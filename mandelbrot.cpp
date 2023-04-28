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
    cout << endl;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            auto c = complex(
                    (double(j) - size / 2.0) / double(scale),
                    (double(i) - size / 2.0) / double(scale)
            );
            auto x = complex(0.0, 0.0);
            int k = 0;
            for (; k < 255; ++k) {
                x = x * x + c;
                if (abs(x) > 1000000.0) {
                    break;
                }
            }
            if (k == 255) {
                cout << " ";
            } else {
                auto g = min(255, k * 15);
                cout << "\033[38;2;0;" << g << ";0m*\033[0m";
            }
            //cout << "c = " << c << ", k = " << k << endl;
        }
        cout << endl;
    }




    return 0;
}
