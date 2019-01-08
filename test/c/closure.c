int foo (int i) { return i + 1; }

typedef int (*functiontype)();
int bar (functiontype f) { return f(1); }

int main () {
    functiontype func = &foo;
    bar(func);
}