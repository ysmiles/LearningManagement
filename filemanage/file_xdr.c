/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "file.h"

bool_t
xdr_mystring (XDR *xdrs, mystring *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, objp, BUFSIZE))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_myres (XDR *xdrs, myres *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->sz))
		 return FALSE;
	 if (!xdr_mystring (xdrs, &objp->content))
		 return FALSE;
	return TRUE;
}
