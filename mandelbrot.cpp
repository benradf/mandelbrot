#include <iostream>
#include <complex>

using namespace std;

int main(int argc, char* argv[])
{
    auto size = atoi(argv[1]);
    auto scale = atoi(argv[2]);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size * 2; ++j) {
            auto c = complex(
                    (double(j) - size) / double(scale) / 2.0,
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
                auto b = min(255, k * 15);
                cout << "\033[48;2;0;0;" << b << "m \033[0m";
            }
        }
        cout << endl;
    }

    return 0;
}

