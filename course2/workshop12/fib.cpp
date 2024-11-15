template <typename Int_t> 
Int_t fib(Int_t n)
{
    Int_t a {0};
    Int_t b {1};
    Int_t x {0};
    Int_t y {1};
    Int_t t0 {};
    Int_t t1 {};
    while (n != 0) {
        switch(n % 2) {
            case 1:
                t0 = a * x + b * y;
                t1 = b * x + a * y + b * y;
                x = t0;
                y = t1;
                --n;
                continue;
            default:
                t0 = a * a + b * b;
                t1 = 2 * a * b + b * b;
                a = t0;
                b = t1;
                n /= 2;
                continue;
        }
    }
    return x;
}