/* 
 * (C) Copyright 2001 Diomidis Spinellis.
 *
 * An identifier equivalence class.
 * Contains all tokids that belong to the same class
 *
 * Include synopsis:
 * #include <deque>
 * #include "tokid.h"
 * #include "tokmap.h"
 *
 * $Id: eclass.h,v 1.1 2001/08/15 18:08:00 dds Exp $
 */

#ifndef ECLASS_
#define ECLASS_

typedef deque <Tokid> EC_members;

class Eclass {
private:
	int len;			// Identifier length
	EC_members members;		// Class members
public:
	// An equivalence class shall know its length
	inline Eclass(int len);
	// It can be constructed from an initiall Tokid
	inline Eclass(Tokid t, int len);
	// Add t to the class; also updates the Tokmap
	void add_tokid(Tokid t);
	// Merge two equivalence classes returning the resulting one
	// After the merger the values of a and b are undefined
	friend Eclass *merge(Eclass *a, Eclass *b);
	// Split an equivalence class at character position pos
	friend void split(Eclass *ec, int pos);
	friend ostream& operator<<(ostream& o,const Eclass& ec);
};

inline
Eclass::Eclass(int l)
	: len(l)
{
}

inline
Eclass::Eclass(Tokid t, int l)
	: len(l)
{
	add_tokid(t);
}

#endif /* ECLASS_ */
