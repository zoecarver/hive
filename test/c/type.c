struct T {
    int i;
    double d;

};

int main (int a) {
    struct T t;

    unsigned long foo = 5;
    int bar = 0;
    double dar = 0;

    if (a) bar = (typeof(t.i))foo;
    else dar = (typeof(t.d))foo;
}