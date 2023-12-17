.code

ptr_exchange proc
	xchg	rdx, [rcx]
	mov	rax, rdx
	ret
ptr_exchange endp

current_thread proc
	mov	rax, gs:[188h]
	ret
current_thread endp

end