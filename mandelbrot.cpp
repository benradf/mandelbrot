#include <iostream>
#include <complex>
#include <tuple>

using namespace std;


/*
    private fun hsl(h: Double, s: Double, l: Double): String {
        fun fracMod(x: Double, n: Int): Double {
            val y = truncate(x)
            return (y.toInt() % n).toDouble() + (x - y)
        }
        val a = s * min(l, 1 - l)
        fun k(n: Int): Double = fracMod(n + h / 30.0, 12)
        fun f(n: Int): Double = l - a * max(-1.0, min(min(k(n) - 3.0, 9.0 - k(n)), 1.0))
        fun g(n: Int) = (255.0 * max(0.0, min(255.0, f(n)))).toInt()
        return "#%02x%02x%02x".format(g(0), g(8), g(4))
    }
    */


tuple<int, int, int> hsl(double h, double s, double l)
{
    auto fracMod = [](double x, int n) {
        auto y = trunc(x);
        return double(int(y) % n) + (x - y);
    };
    auto a = s * min(l, 1 - l);
    auto k = [&](int n) {
        return fracMod(n + h / 30.0, 12.0);
    };
    auto f = [&](int n) {
        return l - a * max(-1.0, min(min(k(n) - 3.0, 9.0 - k(n)), 1.0));
    };
    auto g = [&](int n) {
        return int(255.0 * max(0.0, min(255.0, f(n))));
    };
    return make_tuple(g(0), g(8), g(4));
}

int main(int argc, char* argv[])
{
    auto size = atoi(argv[1]);
    auto scale = atoi(argv[2]);
    const auto ITER_COUNT = 355;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size * 2; ++j) {
            auto c = complex(
                    (double(j) - size) / double(scale) / 2.0,
                    (double(i) - size / 2.0) / double(scale)
            );
            auto x = complex(0.0, 0.0);
            int k = 0;
            for (; k < ITER_COUNT; ++k) {
                x = x * x + c;
                if (abs(x) > 1000000.0) {
                    break;
                }
            }
            if (k == ITER_COUNT) {
                cout << " ";
            } else {
                auto b = min(255, k * 15);
            }

            auto [r, g, b] = k < ITER_COUNT ? hsl(double(k), 1.0, 0.7) : make_tuple(0, 0, 0);

            cout << "\033[48;2;" << r << ";" << g << ";" << b << "m \033[0m";
        }
        cout << endl;
    }

    return 0;
}
