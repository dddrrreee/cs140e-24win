// this is a boehm example without threads.
// 1. can the compiler re-order or remove a write?
// 2. if compiler removed branch: what is going on?
void foo(int *p, int *q) {
    if(q != p) {
        *q = 1;
        *q = 3;
        *p = 2;
    }
    return;
}
