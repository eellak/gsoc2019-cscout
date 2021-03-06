/*
 * (C) Copyright 2001-2015 Diomidis Spinellis
 *
 * This file is part of CScout.
 *
 * CScout is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CScout is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CScout.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Encapsulates the common parts of a (user interface) query
 *
 */

#include <map>
#include <string>
#include <vector>
#include <stack>
#include <iterator>
#include <iostream>
#include <fstream>
#include <list>
#include <set>
#include <cctype>
#include <sstream>		// ostringstream
#include <cstdio>		// perror, rename
#include <cstdlib>		// atoi

#include <regex.h>

#include "cpp.h"
#include "debug.h"
#include "error.h"
#include "parse.tab.h"
#include "attr.h"
#include "compiledre.h"
#include "query.h"

// URL-encode the given string
string
Query::url(const string &s)
{
	string r;

	for (string::const_iterator i = s.begin(); i != s.end(); i++)
		if (*i == ' ')
			r += '+';
		else if (isalnum(*i) || *i == '_')
			r += *i;
		else {
			char buff[4];

			sprintf(buff, "%%%02x", (unsigned)*i);
			r += buff;
		}
	return r;
}

// Compile regular expression specs
char *
Query::compile_re(const char *name, const char *varname, CompiledRE &re, bool &match,  string &str, int compflags)
{	
	 
	const char *s = server.getCharPParam(varname);
	match = false;
	char* to_return = NULL;
	if (s && *s) {
		match = true;
		str = s;
		re = CompiledRE(s, REG_EXTENDED | REG_NOSUB | compflags);
		if (!re.isCorrect()) {
			to_return = new char[100];
			sprintf(to_return, "%s regular expression error %s", name, re.getError().c_str());
			valid = return_val = false;
			lazy = true;
			return to_return;
		}
	}
	return to_return;
}

// Display an equality selection box
string
Query::equality_selection()
{
	
	return(
		"<option value=\""+to_string(ec_ignore) + "\">ignore"
		"<option value=\""+to_string(ec_eq) + "\">=="
		"<option value=\""+to_string(ec_ne) + "\">!="
		"<option value=\""+to_string(ec_lt) + "\">&lt;"
		"<option value=\""+to_string(ec_gt) + "\">&gt;"
		"</select>"
		);
}
