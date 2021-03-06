/*****************************************************************************
 * PJINLINE.H - Define inline code sequences for common functions.
 *
 *	Watcom has an inline facility, but their idea of 'good' inline code
 *	stinks.  Below are some better alternatives, plus some custom things.
 *
 * 10/12/92 - Fixed glitch in pj_xlate(), now handles count of 1.
 ****************************************************************************/

#ifndef PJINLINE_H
#define PJINLINE_H

#undef __INLINE_FUNCTIONS__  // disable Watcom's inline facility, we replace it

/*----------------------------------------------------------------------------
 * strcpy
 *	there's a shortcut in this sequence which borders on sleazy...
 *	the 'done' label refers to the last byte of the sequence, a STOSB instr.
 *	when the AL reg holds the nullterm byte, a branch is taken to the 0xAA
 *	(STOSB) to store the last byte.  when the AH reg holds the nullterm byte,
 *	the JNE LOOP is a fallthru, and it hits the 0xA8 byte, which together
 *	with the 0xAA is an innocuous TEST instruction.
 *--------------------------------------------------------------------------*/

char *strcpy(char *dst, const char *src);

#pragma aux strcpy =												\
	  0x8B 0xD7 	 /* 			mov 	edx,edi 		   */	\
	  0x66 0xAD 	 /* loop:		lodsw					   */	\
	  0x84 0xC0 	 /* 			test	al,al			   */	\
	  0x74 0x07 	 /* 			jz		short done		   */	\
	  0x66 0xAB 	 /* 			stosw					   */	\
	  0x84 0xE4 	 /* 			test	ah,ah			   */	\
	  0x75 0xF4 	 /* 			jne 	short loop		   */	\
	  0xA8 0xAA 	 /* done:		test al,0AAh | stosb	   */	\
	parm caller [edi] [esi] 										\
	value [edx] 													\
	modify exact [eax edx esi edi];

/*----------------------------------------------------------------------------
 * memcpy
 *--------------------------------------------------------------------------*/

void *memcpy(void *dst, const void *src, unsigned count);

#pragma aux memcpy =												\
	  0x8B 0xD7 	 /* 			mov 	edx,edi 		   */	\
	  0xD1 0xE9 	 /* 			shr 	ecx,1			   */	\
	  0xF3 0x66 0xA5 /* 			rep movsw				   */	\
	  0x13 0xC9 	 /* 			adc 	ecx,ecx 		   */	\
	  0xF3 0xA4 	 /* 			rep movsb				   */	\
	parm caller [edi] [esi] [ecx]									\
	value [edx] 													\
	modify exact [ecx edx esi edi];

/*----------------------------------------------------------------------------
 * memset
 *--------------------------------------------------------------------------*/

void *memset(void *dst, int value, unsigned count);

#pragma aux memset =												\
	  0x8B 0xD7 	 /* 			mov 	edx,edi 		   */	\
	  0x8A 0xE0 	 /* 			mov 	ah,al			   */	\
	  0xD1 0xE9 	 /* 			shr 	ecx,1			   */	\
	  0xF3 0x66 0xAB /* 			rep stosw				   */	\
	  0x13 0xC9 	 /* 			adc 	ecx,ecx 		   */	\
	  0xF3 0xAA 	 /* 			rep stosb				   */	\
	parm caller [edi] [eax] [ecx]									\
	value [edx] 													\
	modify exact [eax ecx edx edi];

#endif /* PJINLINE_H */
