.global get_cr3
.type get_cr3, @function
.extern save_cr3
get_cr3:
	push %rdi
	movq %cr3, %rdi
	call save_cr3
	pop %rdi
	ret 
