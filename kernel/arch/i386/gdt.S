
#*************************************************
#	gdt.inc
#		-GDT Routines
#
#	OS Development Series
#*************************************************

#*************************************************
#	can't find translation to at&t
#	leaving it in case there is any trobule in the future
#
#	%ifndef __GDT_INC_67343546FDCC56AAB872_INCLUDED__
#	%define __GDT_INC_67343546FDCC56AAB872_INCLUDED__
#	bits	16 
#
#*************************************************

#*******************************************
# InstallGDT()
#	- Install our GDT
#*******************************************
.section .text
.global InstallGDT
.type InstallGDT, @function
InstallGDT:

	cli				    # clear interrupts
	pusha				# save registers
	lgdt 	(toc)		# load GDT into GDTR
	sti				    # enable interrupts
	popa				# restore registers
	ret				    # All done!

#*******************************************
# Global Descriptor Table (GDT)
#*******************************************

gdt_data: 
	.long 0 				# null descriptor
	.long 0 

# gdt code:					# code descriptor
	.int 0xFFFF 			# limit low
	.int 0 					# base low
	.byte 0 				# base mi.longle
	.byte 154 			 	#access
	.byte 207				# granularity
	.byte 0 				# base high

# gdt data:					# data descriptor
	.int 0x0FFFF 			# limit low (Same as code)
	.int 0 					# base low
	.byte 0 				# base mi.longle
	.byte 146 				# access
	.byte 207 				# granularity
	.byte 0					# base high

end_of_gdt:
toc: 
	.int end_of_gdt - gdt_data - 1 	# limit (Size of GDT)
	.long gdt_data 			        # base of GDT

#*******************************************
#	same casa as above
#	.endif #__GDT_INC_67343546FDCC56AAB872_INCLUDED__
#*******************************************