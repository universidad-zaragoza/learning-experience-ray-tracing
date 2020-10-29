//Ruben Gran

    .arch armv8-a
    .text
    .global mi_fetch_and_add
    .type mi_fetch_and_add, %function

mi_fetch_and_add:
    ldaxr w2, [x0]
    add w4, w1, w2
    stlxr w3, w4, [x0]
    cbnz w3, mi_fetch_and_add
    mov w0, w2
    ret

