/* 
 * (C) Copyright 2001 Diomidis Spinellis.
 *
 * For documentation read the corresponding .h file
 *
 * $Id: ptoken.cpp,v 1.9 2003/06/01 08:55:15 dds Exp $
 */

#include <iostream>
#include <map>
#include <string>
#include <deque>
#include <list>
#include <vector>
#include <cassert>

#include "cpp.h"
#include "ytab.h"
#include "attr.h"
#include "metrics.h"
#include "fileid.h"
#include "tokid.h"
#include "token.h"
#include "ptoken.h"

ostream& 
operator<<(ostream& o,const Ptoken &t)
{
	o << (Token)t;
	o << "Value: [" << t.val << "]\n";
	return (o);
}

ostream& operator<<(ostream& o,const listPtoken &t)
{
	listPtoken::const_iterator i;

	for (i = t.begin(); i != t.end(); i++)
		o << (*i).get_val();
	return (o);
}

#ifdef UNIT_TEST
// cl -GX -DWIN32 -c eclass.cpp fileid.cpp tokid.cpp tokname.cpp token.cpp
// cl -GX -DWIN32 -DUNIT_TEST ptoken.cpp token.obj tokid.obj eclass.obj tokname.obj fileid.obj kernel32.lib

main()
{
	Ptoken t(IDENTIFIER, "hello");

	cout << t;

	return (0);
}
#endif /* UNIT_TEST */
