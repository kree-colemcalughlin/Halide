#include <Halide.h>
#include <stdio.h>

using namespace Halide;

int bits_diff(float fa, float fb) {
    uint32_t a = *((uint32_t *)(&fa));
    uint32_t b = *((uint32_t *)(&fb));
    uint32_t a_exp = a >> 23;
    uint32_t b_exp = b >> 23;
    if (a_exp != b_exp) return -100;
    uint32_t diff = a > b ? a - b : b - a;
    int count = 0;
    while (diff) {
        count++;
        diff /= 2;
    }
    return count;
}

// Check the mantissas match except for the last few bits.
void check(Image<float> a, Image<float> b) {
    for (int i = 0; i < a.width(); i++) {
        int err = bits_diff(a(i), b(i));
        if (err > 8) {
            printf("Mismatch in mantissa at %d: %10.10f %10.10f. Differs by %d bits.\n", i, a(i), b(i), err);
            //exit(-1);
        }
    }
}

int main(int argc, char **argv) {

    Func f1, f2, f3, f4;
    Func g1, g2, g3, g4;

    Var x;
    Expr v = x*1.34f + 1.0142f;

    Param<float> p;
    p.set(1.0f);

    // Test accuracy of reciprocals.

    // First prevent any optimizations by hiding 1.0 in a param.
    f1(x) = p / v;

    // Now test various vectorization widths with an explicit 1.0. On
    // arm 2 and 4 trigger optimizations. On x86 4 and 8 do.
    f2(x) = 1.0f / v;
    f2.vectorize(x, 2);

    f3(x) = 1.0f / v;
    f3.vectorize(x, 4);

    f4(x) = 1.0f / v;
    f4.vectorize(x, 8);

    // Same thing for reciprocal square root.
    g1(x) = p / sqrt(v);

    g2(x) = 1.0f / sqrt(v);
    g2.vectorize(x, 2);

    g3(x) = 1.0f / sqrt(v);
    g3.vectorize(x, 4);

    g4(x) = 1.0f / sqrt(v);
    g4.vectorize(x, 8);

    Image<float> imf1 = f1.realize(10000);
    Image<float> imf2 = f2.realize(10000);
    Image<float> imf3 = f3.realize(10000);
    Image<float> imf4 = f4.realize(10000);

    Image<float> img1 = g1.realize(10000);
    Image<float> img2 = g2.realize(10000);
    Image<float> img3 = g3.realize(10000);
    Image<float> img4 = g4.realize(10000);

    printf("Testing accuracy of inverse\n");
    check(imf1, imf2);
    check(imf1, imf3);
    check(imf1, imf4);
    printf("Pass.\n");
    printf("Testing accuracy of inverse sqrt\n");
    check(img1, img2);
    check(img1, img3);
    check(img1, img4);
    printf("Pass.\n");

    printf("Success!\n");
    return 0;
}
