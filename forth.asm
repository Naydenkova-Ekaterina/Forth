%include 'lib.inc'
%include 'macro.inc'
%include 'dictionary.inc'

global _start

section .data
  last_word: dq link
  dp: dq user_mem
  in_fd: dq 0
  xt_run: dq run
  xt_loop: dq main_loop
  program_stub: dq 0
  errormsg: db "error", 0
  imode_message: db "Interpreter mode", 0
  cmode_message: db "Compiler mode", 0
  mode: dq 0              
  was_branch: db 0
  here: dq forth_mem
  stack_start: dq 0
  unknown_word: db "No such word", 0
  underflow:db 'Stack underflow exception', 10, 0

section .bss
  
  rstack_start: resq 1
  forth_mem: resq 65536
  input_buf: resb 1024
  user_buf: resb 1024
  user_mem: resq 65536  
  state: resq 1
  stackHead: resq 1
  ustackHead:resq 1

section .text
_start:
  xor eax, eax
  push rax         
  jmp init_impl

run:
  dq docol_impl

  main_loop:
    dq xt_buffer
    dq xt_word               
    branchif0 exit          
    dq xt_buffer
    dq xt_find               
    dq xt_pushmode
    branchif0 .interpreter

  .compiler_mode:
    dq xt_dup                 
    branchif0 .compiler_num
    dq xt_cfa                 
    dq xt_isInstant
    branchif0 .not_instant

     .instant:
      dq xt_execute
      branch main_loop

    .not_instant:
      dq xt_isbranch
      dq xt_comma
      branch main_loop

    .compiler_num:
      dq xt_drop
      dq xt_buffer
      dq xt_parse_int
      branchif0 .error
      dq xt_wasbranch
      branchif0 .lit
      dq xt_unsetbranch
      dq xt_savenum
      branch main_loop

     .lit:
      dq xt_lit, xt_lit
      dq xt_comma
      dq xt_comma
      branch main_loop

  .interpreter:
    dq xt_dup
    branchif0 .interpreter_num
    dq xt_cfa                
    dq xt_execute
    branch main_loop

    .interpreter_num:
      dq xt_drop
      dq xt_buffer
      dq xt_parse_int
      branchif0 .error
      branch main_loop

  .error:
    dq xt_drop
    dq xt_error
    branch main_loop

find_word:
   xor eax, eax            
   mov rsi, [last_word]    
  .loop:
    push rdi
    push rsi
    add rsi, link_size
    call string_equals      
    pop rsi               
    pop rdi             
    test rax, rax
    jnz .found              
    mov rsi, [rsi]         
    test rsi, rsi         
    jnz .loop
    xor eax, eax
    ret
  .found:
    mov rax, rsi
    ret

next:
  mov w, [pc]
  add pc, 8
  jmp [w]

call_from_address:
  xor eax, eax
  add rdi, link_size
  push rdi
  call string_length       
  pop rdi
  add rax, 1                
  add rax, 1               
  add rdi, rax
  mov rax, rdi
  ret

exit:
  dq xt_bye


