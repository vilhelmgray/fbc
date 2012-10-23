/* ERASE for static arrays of objects: re-init the elements */

#include "fb.h"

void fb_hArrayCtorObj( FBARRAY *array, FB_DEFCTOR ctor, int base_idx )
{
	int	elements, element_len, i;
	FBARRAYDIM *dim;
	const char *this_;

	if( array->ptr == NULL )
		return;

    dim = &array->dimTB[0];
    elements = dim->elements - base_idx;
    ++dim;

    for( i = 1; i < array->dimensions; i++, dim++ )
	   	elements *= dim->elements;

	/* call ctors */
	element_len = array->element_len;
	this_ = (const char *)array->ptr;

	while( elements > 0 ) {
		/* !!!FIXME!!! check exceptions (only if rewritten in C++) */
		ctor( this_ );
		this_ += element_len;
		--elements;
	}
}

FBCALL void fb_ArrayClearObj( FBARRAY *array, FB_DEFCTOR ctor, FB_DEFCTOR dtor )
{
	/* destruct all objects in the array
	   (dtor can be NULL if there only is a ctor) */
	if( dtor )
		fb_ArrayDestructObj( array, dtor );

	/* re-initialize (ctor can be NULL if there only is a dtor) */
	fb_ArrayClear( array );
	if( ctor )
		fb_hArrayCtorObj( array, ctor, 0 );
}