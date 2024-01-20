#include "rpi-asm.h"



.LC0:
	.ascii	"hello\000"
	.align	2

.L2:
	.word	.LC0

MK_FN(todo_test_asm1)
	ldr	r0, .L2
    push {lr}
	bl todo_test
    pop {lr}
	bx	lr

MK_FN(todo_test_asm2)
	add r0, pc, #0
	b todo_test
	.ascii	"hello\000"
	.align	2

MK_FN(todo_test_asm)
    asm_not_reached();
    asm_not_implemented();
    asm_todo(todo test)

MK_FN(notreached_test_asm)
    asm_not_reached();
MK_FN(unimplemented_test_asm)
    asm_not_implemented();


#define concat(x,y)

concat(foo, __LINE__)
