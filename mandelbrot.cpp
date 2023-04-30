#include <cassert>
#include <complex>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

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


#pragma pack(push, 1)

struct FileHeader {
    uint16_t magic;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
};

struct InfoHeader {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t depth;
    uint32_t compression;
    uint32_t imageSize;
    uint32_t horizontalRes;
    uint32_t verticalRes;
    uint32_t paletteSize;
    uint32_t importantColours;
};

#pragma pack(pop)

template<typename Pixel>
void writeBitmap(const char* filename, int width, int height, const vector<Pixel>& pixels)
{
    FileHeader fileHeader;
    fileHeader.magic = 0x4D42;
    fileHeader.size = sizeof(fileHeader) + sizeof(InfoHeader);
    fileHeader.reserved1 = 0;
    fileHeader.reserved2 = 0;
    fileHeader.offset = fileHeader.size;

    InfoHeader infoHeader;
    infoHeader.size = sizeof(infoHeader);
    infoHeader.width = width;
    infoHeader.height = height;
    infoHeader.planes = 1;
    infoHeader.depth = 24;
    infoHeader.compression = 0;
    infoHeader.horizontalRes = 1;
    infoHeader.verticalRes = 1;
    infoHeader.paletteSize = 0;
    infoHeader.importantColours = 0;

    uint32_t unpaddedWidth = (infoHeader.depth >> 3) * width;
    uint32_t paddedWidth = ((unpaddedWidth - 1) & 0xFFFFFFFC) + 4;
    uint32_t padBytes = paddedWidth - unpaddedWidth;

    infoHeader.imageSize = paddedWidth * height;
    fileHeader.size += infoHeader.imageSize;

    std::ofstream file(filename, std::ios::binary);
    assert(file.is_open());

    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    for (int32_t y = height - 1; y >= 0; y--) {
        for (uint32_t x = 0; x < width; x++) {
            auto pixel = pixels.at(y * width + x);
            file.write(reinterpret_cast<char*>(&pixel), 3);
        }
        for (uint32_t i = 0; i < padBytes; i++) 
            file.put(0);
    }

    file.close();
}


int main(int argc, char* argv[])
{
    const auto MAX_K = 500;
    const auto PI = 3.1415;
    const auto OFFSET_X = -0.7;
    const auto OFFSET_Y = 0.0;

    if (argc != 4) {
        cout << "usage: mandelbrot 60 20 12" << endl;
        return 1;
    }

    auto size = atoi(argv[1]);
    auto scale = atoi(argv[2]);
    auto kmult = atoi(argv[3]);

    auto width = size; // * 2;
    auto height = size;

    vector<tuple<uint8_t, uint8_t, uint8_t>> pixels(width * height);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            auto c = complex(
                    OFFSET_X + (double(j) - width / 2.0) / double(scale), // / 2.0,
                    OFFSET_Y + (double(i) - height / 2.0) / double(scale)
            );
            auto x = complex(0.0, 0.0);
            int k = 0;
            for (; k < MAX_K; ++k) {
                x = x * x + c;
                if (abs(x) > pow(10.0, 12.0)) {
                    break;
                }
            }
            if (k == MAX_K) {
                //cout << " ";
            } else {
                auto l = 0.1 + 0.9 * min(1.0, double(k * kmult) / MAX_K);
                auto h =  (arg(c) + PI) * 360.0 / (2 * PI);
                auto [r, g, b] = hsl(h, 1.0, l);
                //cout << "\033[48;2;" << r << ";" << g << ";" << b << "m \033[0m";
                pixels.at(i * width + j) = hsl(h, 1.0, l);
            }
        }
        //cout << endl;
    }

    writeBitmap("mandelbrot.bmp", width, height, pixels);

    return 0;
}
