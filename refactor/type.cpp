/* 
 * (C) Copyright 2001 Diomidis Spinellis.
 *
 * For documentation read the corresponding .h file
 *
 * $Id: type.cpp,v 1.9 2001/09/21 08:56:31 dds Exp $
 */

#include <iostream>
#include <string>
#include <cassert>
#include <fstream>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <vector>
#include <list>

#include "ytab.h"

#include "fileid.h"
#include "fileid.h"
#include "cpp.h"
#include "fileid.h"
#include "tokid.h"
#include "eclass.h"
#include "token.h"
#include "error.h"
#include "ptoken.h"
#include "macro.h"
#include "pdtoken.h"
#include "ctoken.h"
#include "type.h"
#include "stab.h"
#include "debug.h"



Type&
Type::operator=(const Type& rhs)
{
	// The operation sequence handles self-assignment
	rhs.p->use++;
	if (--p->use == 0)
		delete p;
	p = rhs.p;
	return (*this);
}

Type
Type_node::subscript() const
{
	Error::error(E_ERR, "subscript not on array or pointer");
	return basic(b_undeclared);
}


Type
Type_node::call() const
{
	Error::error(E_ERR, "object is not a function");
	return basic(b_undeclared);
}

Type
Tidentifier::call() const
{
	// Undeclared identifiers f when called are declared as int f(...)
	obj_define(this->get_token(), function_returning(basic(b_int)));
	Error::error(E_WARN, "assuming declaration int " + this->get_name() + "(...)");
	return basic(b_int);
}


const Ctoken& 
Type_node::get_token() const
{
	static Ctoken c;
	Error::error(E_INTERNAL, "attempt to get token value of a non-identifier");
	if (DP())
		this->print(cerr);
	return c;
}

Type
Type_node::clone() const
{
	Error::error(E_INTERNAL, "unable to clone type");
	return Type();
}

Tbasic *
Type_node::tobasic()
{
	Error::error(E_INTERNAL, "unknown tobasic() conversion");
	return NULL;
}

const string&
Type_node::get_name() const
{
	static string s("unknown");
	Error::error(E_INTERNAL, "attempt to get the name of a non-identifier");
	return s;
}

Type
Type_node::deref() const
{
	Error::error(E_ERR, "illegal pointer dereference");
	return basic(b_undeclared);
}


Type
Type_node::type() const
{
	Error::error(E_INTERNAL, "object is not a typedef or identifier");
	return basic(b_undeclared);
}


enum e_storage_class 
Type_node::get_storage_class() const
{
	Error::error(E_INTERNAL, "object has no storage class");
	return c_unspecified;
}

Id
Type_node::member(const string& s) const
{
	Error::error(E_ERR, "invalid member access: not a structure or union");
	return Id(Token(), basic(b_undeclared));
}

Id
Tsu::member(const string& s) const
{
	map<string, Id>::const_iterator i;

	if ((i = members.find(s)) == members.end()) {
		Error::error(E_ERR, "structure or union does not have a member " + s);
		return Id(Token(), basic(b_undeclared));
	} else
		return ((*i).second);
}
Type
basic(enum e_btype t = b_abstract, enum e_sign s = s_none, enum e_storage_class sc = c_unspecified)
{
	return Type(new Tbasic(t, s, sc));
}

Type
array_of(Type t)
{
	return Type(new Tarray(t));
}

Type
pointer_to(Type t)
{
	return Type(new Tpointer(t));
}

Type
function_returning(Type t)
{
	return Type(new Tfunction(t));
}

Type
enum_tag()
{
	return Type(new Ttag(tt_enum));
}

Type
struct_tag()
{
	return Type(new Ttag(tt_struct));
}

Type
union_tag()
{
	return Type(new Ttag(tt_union));
}

Id
Type::member(const string& name) const
{
	return p->member(name);
}

void
Tsu::add_member(string& name, Id i)
{
	members[name] = i;
}

Type
identifier(const Ctoken& t)
{
	return Type(new Tidentifier(t));
}

Type
label()
{
	return Type(new Tlabel());
}

void
Type_node::print(ostream &o) const
{
	o << "Unknown type node";
}

void
Tbasic::print(ostream &o) const
{
	switch (sclass) {
	case c_unspecified: break;
	case c_typedef: o << "typedef "; break;
	case c_extern: o << "extern "; break;
	case c_static: o << "static "; break;
	case c_auto: o << "auto "; break;
	case c_register: o << "register "; break;
	}

	switch (sign) {
	case s_none: break;
	case s_signed: o << "signed "; break;
	case s_unsigned: o << "unsigned "; break;
	}

	switch (type) {
	case b_abstract: o << "ABSTRACT "; break;
	case b_void: o << "void "; break;
	case b_char: o << "char "; break;
	case b_short: o << "short "; break;
	case b_int: o << "int "; break;
	case b_long: o << "long "; break;
	case b_llong: o << "long long "; break;
	case b_float: o << "float "; break;
	case b_double: o << "double "; break;
	case b_ldouble: o << "long double "; break;
	case b_undeclared: o << "UNDECLARED "; break;
	}
}

void
Tarray::print(ostream &o) const
{
	o << "array of " << of;
}

void
Tpointer::print(ostream &o) const
{
	o << "pointer to " << to;
}

void
Tfunction::print(ostream &o) const
{
	o << "function returning " << returning;
}

void
Tlabel::print(ostream &o) const
{
	o << "label ";
}

void
Ttag::print(ostream &o) const
{
	switch (type) {
	case tt_struct: o << "struct "; break;
	case tt_union: o << "union "; break;
	case tt_enum: o << "enum "; break;
	}
}

void
Tsu::print(ostream &o) const
{
	map<string,Id>::const_iterator i;

	o << "{";
	for (i = members.begin(); i != members.end(); ) {
		o << (*i).first << ": " << ((*i).second.get_type());
		i++;
		if (i != members.end())
			o << ", ";
	}
	o << "} ";
}

void
Tidentifier::print(ostream &o) const
{
	o << t.get_name() << ":" << of;
}

/*
 * Merge two basic types constisting of sign, type specifiers, and
 * storage class, returning the composite type
 * See ANSI 3.5.2
 */
Type
Tbasic::merge(Tbasic *b)
{
	enum e_btype t;
	enum e_sign s;
	enum e_storage_class c;

	if (b == NULL)
		return Type_node::merge(b);
	if (this->type == b_abstract || this->type == b_undeclared)
		t = b->type;
	else if (b->type == b_abstract || b->type == b_undeclared)
		t = this->type;
	else if ((this->type == b_long && b->type == b_int) ||
	         (this->type == b_int && b->type == b_long))
		t = b_long;
	else if ((this->type == b_short && b->type == b_int) ||
	         (this->type == b_int && b->type == b_short))
		t = b_short;
	else if ((this->type == b_long && b->type == b_double) ||
	         (this->type == b_double && b->type == b_long))
		t = b_ldouble;
	else if ((this->type == b_long && b->type == b_long) ||
	         (this->type == b_long && b->type == b_long))
		t = b_llong;		// Extension to ANSI
	else {
		Error::error(E_ERR, "illegal combination of type specifiers");
		t = b_undeclared;
	}

	if (this->sign == s_none)
		s = b->sign;
	else if (b->sign == s_none)
		s = this->sign;
	else {
		Error::error(E_WARN, "illegal combination of sign specifiers");
		s = s_none;
	}
	// Signed or unsigned on its own means "int"
	if ((t == b_abstract || t == b_undeclared) && s != s_none)
		t = b_int;
	if (s != s_none && (t == b_float || t == b_double || t == b_ldouble)) {
		Error::error(E_WARN, "sign specification on non-integral type - ignored");
		s = s_none;
	}

	if (this->sclass == c_unspecified)
		c = b->sclass;
	else if (b->sclass == c_unspecified)
		c = this->sclass;
	else {
		Error::error(E_ERR, "at most one storage class can be specified");
		c = this->sclass;
	}
	if (DP()) {
		cout << "merge a=";
		this->print(cout);
		cout << "\nmerge b=";
		b->print(cout);
		Type r = basic(t, s, c);
		cout << "\nmerge r=" << r << "\n";
	}
	return basic(t, s, c);
}

Type
Type_node::merge(Tbasic *b)
{
	Error::error(E_ERR, "invalid application of basic type, storage class, or type specifier");
	if (DP()) {
		cout << "a=";
		this->print(cout);
		cout << "\nb=";
		b->print(cout);
		cout << "\n";
	}
	return basic();
}

void
Type_node::set_abstract(Type t)
{
	Error::error(E_ERR, "invalid type specification");
}

void
Tarray::set_abstract(Type t)
{
	if (of.is_basic()) {
		if (of.is_abstract())
			of = t;
		else {
			Error::error(E_ERR, "array not an abstract type");
			cerr << "[" << of << "]\n";
		}
	} else
		of.set_abstract(t);
}

void
Tpointer::set_abstract(Type t)
{
	if (to.is_basic()) {
		if (to.is_abstract())
			to = t;
		else {
			Error::error(E_ERR, "pointer not an abstract type");
			cerr << "[" << to << "]\n";
		}
	} else
		to.set_abstract(t);
}

void
Tfunction::set_abstract(Type t)
{
	if (returning.is_basic()) {
		if (returning.is_abstract())
			returning = t;
		else
			Error::error(E_ERR, "pointer not an abstract type");
	} else
		returning.set_abstract(t);
}

void
Tidentifier::set_abstract(Type t)
{
	if (of.is_basic()) {
		if (of.is_abstract())
			of = t;
		else
			Error::error(E_ERR, "pointer not an abstract type");
	} else
		of.set_abstract(t);
}

void
Type::declare()
{
	obj_define(this->get_token(), this->type());
}

Type 
Tbasic::clone() const
{
	return Type(new Tbasic(type, sign, sclass));
}

void
Tbasic::set_storage_class(Type t)
{
	enum e_storage_class newclass = t.get_storage_class();

	if (sclass != c_unspecified &&
	    sclass != c_typedef &&
	    newclass != c_unspecified)
		Error::error(E_ERR, "multiple storage classes in type declaration");
	// if sclass is already e.g. extern and t is just volatile don't destry sclass
	if (sclass == c_unspecified || sclass == c_typedef)
		sclass = newclass;
}

void
Type_node::set_storage_class(Type t)
{
	Error::error(E_INTERNAL, "object can not set storage class");
}