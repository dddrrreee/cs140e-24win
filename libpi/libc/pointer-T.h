#ifndef __POINTER_T_H__
#define __POINTER_T_H__

// need to have a pointer-T.h
#define is_aligned(_p,_n) (((unsigned)(_p))%(_n) == 0)
#define is_aligned4(ptr) is_aligned(ptr,4)
#define is_aligned8(ptr) is_aligned(ptr,8)


// cast <x> to type <T>, checking that <T> has enough bits to hold <x>
// note: this isn't enough to guarantee safety in general, but should
// work fine for our gcc tool chain on arm1176.
#define safecast_T(T,x) \
    ({ assert(sizeof(typeof(T)) >= sizeof(x));  (typeof(T))x; })

// simple wrapper to do "safe" casting assignment of <y> to <x>,
// casting <y> to typeof(x).
#define assign_T(x,y) \
    do { x = safecast_T(typeof(x), y); } while(0)

#define clone(xx) \
 ({ typeof(xx) *tmp = kalloc(sizeof *tmp); memcpy(tmp,xx,sizeof *tmp); tmp; })

// note: if you want to do big SD cards, probably should make this
// uint64_t.
#if 0
typedef uint32_t off_t;
#endif

// b - a;
static inline long ptr_diff(const void *b, const void *a) {
    assert(b >= a);
    return (const char *)b - (const char *)a;
}

// a + off
static inline const void *ptr_add(const void *a, long off) {
    return (const char *)a + off;
}

static inline void *ptr_add_mut(void *a, long off) {
    return (char *)a + off;
}

// assign a and b to tmps in case there are side-effects.
#define min(a,b) ({                 \
    let a_t = a;                    \
    let b_t = b;                    \
    (a_t<b_t) ? a_t : b_t;          \
)}

#define max(a,b) min(b,a)

#if 0
static inline uint32_t min_u32(uint32_t a, uint32_t b) {
    return a < b ? a : b;
}
#endif

static inline uint32_t roundup_u32(uint32_t x, uint32_t n) {
    return (x+(n-1)) & (~(n-1));
}
#endif
