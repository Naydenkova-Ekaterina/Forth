native '+', plus
    pop rax
    add [rsp], rax
    jmp next

native '-', minus
    pop rax
    pop rcx
    sub rcx, rax
    push rcx
    jmp next

native '*', mul
  pop rax
  pop rdx
  imul rdx
  push rax
  jmp next

native '/', div
    pop rcx
    pop rax
    cqo
    idiv rcx
    push rax
    jmp next

native 'word', word
    pop rdi
    call read_word
    push rdx
    jmp next

native 'prints', prints
  pop rdi
  call print_string
  jmp next

native 'printc', printc
  pop rdx
  pop rsi
  mov rdi, 1
  mov rax, 1
  syscall
  jmp next

native '.', dot
   cmp     rsp, [stackHead]
    je      .error
    pop     rdi
    call    print_int
    call    print_newline
    jmp     .f
.error:
    mov     rdi, underflow
    call    print_string
    call    print_newline
.f:
    jmp     next

native 'drop', drop
    pop     rax
    jmp     next

native 'dup', dup
    pop     rax
    push    rax
    push    rax
    jmp     next

native 'swap', swap
    pop     rax
    pop     rcx
    push    rax
    push    rcx
    jmp     next

native '<', less
  	pop rax
	pop rdx
	cmp rdx, rax
	jge .false 
	.true:
	push 1
	jmp .final
	.false:
	push 0
	.final:
	jmp next

native '.S', show_stack
	mov rax, rsp
	.loop:
	cmp rax, [stack_start]
	jge next
	mov rdi, [rax]
	push rax
	call print_int
	call print_newline
	pop rax
	add rax, 8
	jmp .loop

native '=', equals
  	pop rax
	pop rdx
	cmp rax, rdx
	jne .false
	.true:
	push 1
	jmp .final
	.false:
	push 0
	.final:
	jmp next

native "%", mod
	xor edx, edx
	pop rcx
	pop rax
	idiv rcx
	push rdx
	jmp next

native 'and', and
    pop rax
    and [rsp], rax
    jmp next

native 'or', or
    pop rax
    or [rsp], rax
    jmp next

native 'not', not
  pop rax
  test rax, rax
  jz .true
.false:
  push 0
  jmp next
.true:
  push 1
  jmp next

native 'rot', rot
    pop     rax
    pop     rcx
    pop     rdx
    push    rcx
    push    rax
    push    rdx
    jmp     next 

native 'key', key
  	call read_char
	push rax
	jmp next

native 'emit', emit
  	mov rdi, [rsp]
	call print_char
	jmp next

native 'mem', mem
	push qword[ustackHead]
	jmp next

native '!', write
  	pop rax
  pop rdx
  mov [rax], rdx
  jmp next

native '@', fetch
  pop rax
  push qword[rax]
  jmp next

native 'c!', write_char
  pop rax
  pop rdx
  mov [rax], dl
  jmp next

native 'c@', fetch_char
  pop rax
  movzx rax, byte [rax]
  push rax
  jmp next

native '>r', to_r
  pop rax
  rpush rax
  jmp next

native 'r>', from_r
  rpop rax
  push rax
  jmp next

native "r@", r_fetch
  push qword [rstack]
  jmp next

native 'lor', lor
    dq      xt_not
    dq      xt_swap
    dq      xt_not
    dq      xt_and
    dq      xt_not
    dq      xt_exit

native "land", land
    pop     rax
    pop     rcx
    and     rcx, rax
    cmp     rcx, 0
    je      .null
    mov     rcx, 1
.null:
    push    rcx
    jmp     next

native 'count', count
	pop rdi
	call string_length
    	push rax
	jmp next
	
native 'number', number
  	call read_word
	mov rax, rdi
	call parse_int
	push rax
	jmp next

native 'find', find
  pop rdi               
  call find_word
  push rax
  jmp next

native 'cfa', cfa
    pop rsi
    add rsi, 9
    .loop:
    mov al, [rsi]
    test al, al
    jz .end
    inc rsi
    jmp .loop
    .end:
    add rsi, 2
    push rsi
    jmp next

native 'bye', bye
  mov rax, 60
  xor rdi, rdi
  syscall

native 'error', error
  mov  rdi, unknown_word
  call print_string
  call print_newline
  jmp next

native 'isInstant', isInstant
  pop rax
  push rax
  xor rdx, rdx
  mov dl, byte[rax - 1]
  push rdx
  jmp next

native 'parse_int', parse_int
  pop rdi
  call parse_int         
  push rax              
  push rdx
  jmp next

native 'pushmode', pushmode
  xor eax, eax
  mov rax, [mode]
  push rax
  jmp next

native 'pushmode_address', pushmode_address
  xor eax, eax
  mov rax, mode
  push rax
  jmp next

native ',', comma
  mov rax, [here]
  pop qword [rax]
  add qword [here], 8
  jmp next

native 'isbranch', isbranch
  pop rax
  push rax
  cmp byte[rax - 1], 2
  jne .continue
  mov byte[was_branch], 1
  .continue:
  jmp next

native 'lit', lit
  push qword [pc]
  add pc, 8
  jmp next

native 'init', init
  mov rstack, rstack_start
  mov stack, stack_start
  xor eax, eax
  mov rax, [in_fd]
  rpush rax
  xor eax, eax
  pop rax
  mov [in_fd], rax
  mov qword[mode], 0
  mov pc, xt_run
  cmp qword [stack_start], 0
  je  .first
  mov rsp, [stack_start]
  jmp next
  .first:
  mov [stack_start], rsp
  jmp next

native 'buffer', buffer
  push qword input_buf
  jmp next

native 'savenum', savenum
  pop rax
  mov [here], rax
  xor eax, eax
  mov rax, here
  add rax, word_size
  mov qword[here], rax
  jmp next

native 'push_lw', push_lastword
  push last_word
  jmp next

native 'docol', docol
  	sub rstack, 8
	mov [rstack], pc
	add w, 8
	mov pc, w
	jmp next

native 'exit', exit
  mov pc, [rstack]
  add rstack, 8
  jmp next

native 'branch', branch
  mov pc, [pc]
  jmp next

native 'branch0', branch0
    pop rax
    cmp rax, 0
    jne .f
    mov pc, [pc]
    jmp next
    .f:
    add pc, 8
    jmp next

native 'unsetbranch', unsetbranch
  mov byte[was_branch], 0
  jmp next

native 'wasbranch', wasbranch
  xor rdx, rdx
  mov dl, byte[was_branch]
  push rdx
  jmp next


native 'create', create
  mov rcx, [last_word]
  mov rsi, [here]
  mov [rsi], rcx
  mov [last_word], rsi
  add rsi, 8
  pop rdi
  push rsi
  call string_copy
  pop rsi
  push rsi
  mov rdi, rsi
  call string_length
  pop rsi
  add rsi, rax
  inc rsi              
  pop rax
  mov [rsi], al
  inc rsi
  mov [here], rsi
  jmp next
	
const dp, dp
const here, [here]
const in_fd, in_fd
const state, state

native "syscall", syscall
  pop r9
  pop r8
  pop r10
  pop rdx
  pop rsi
  pop rdi
  pop rax
  syscall
  push rax
  push rdx
  jmp next
    
native 'execute', execute
	pop rax
	mov w, rax
	jmp [rax]

colon '>', greater
  dq xt_swap
  dq xt_less
  dq xt_exit

colon ':', colon
  .restart:
    dq xt_buffer
    dq xt_word
    branchif0 .restart
    dq xt_lit, 0         
    dq xt_buffer
    dq xt_create
    dq xt_lit, 1, xt_pushmode_address
    dq xt_write
    dq xt_lit, docol_impl
    dq xt_comma
    dq xt_exit

colon ';', semicolon, 1
  dq xt_lit, 0, xt_pushmode_address
  dq xt_write
  dq xt_lit, xt_exit
  dq xt_comma
  dq xt_exit

colon "'", tick, 1
  dq xt_buffer
  dq xt_word
  dq xt_drop
  dq xt_buffer
  dq xt_find, xt_dup
  branchif0 .no_such_word
  dq xt_cfa
  dq xt_pushmode
  branchif0 .interpret
  dq xt_lit, xt_lit
  dq xt_comma
  dq xt_comma
  .interpret:
    dq xt_exit
    .no_such_word:
      dq xt_error
      dq xt_exit
