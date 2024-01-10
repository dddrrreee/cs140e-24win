// static example
static int x;
void bar();

// 1. can the compiler remove the first write to <x>?
// 2. what happens when we add -mcpu=arm1176jzf-s to gcc's flags?
void foo(int *p) {
    x = 1;
    bar();
    x = 2;
    return;
}
