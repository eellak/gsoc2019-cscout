/*
 * (C) Copyright 2006-2015 Diomidis Spinellis
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
 * A user interface option
 *
 */

#ifndef OPTION_
#define OPTION_

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <cstring>

using namespace std;

#include "error.h"
#include "headers.h"

class BoolOption;
class IntegerOption;
class SelectionOption;
class TextOption;

class Option {
protected:
	const char *short_name;				// File token
	const char *user_name;				// User-interface description
	static vector<Option *> options;		// Options in the order they were added
	static map<string, Option *> omap;	// For loading options
public:
	// Constructor
	Option(const char *sn, const char *un) : short_name(sn), user_name(un) {}
	virtual ~Option() {}

	// Save to a file
	virtual void save(ofstream &ofs) const = 0;
	// Load from a file
	virtual void load(ifstream &ifs) = 0;
	// Set from a submitted page
	virtual void set() = 0;
	// Return JSON 
	virtual json::value display() = 0;
	// Return true if the option can be saved
	virtual bool is_saveable() { return true; }

	// Add a new option to the pool
	static void add(Option *o);
	// Save to a file
	static void save_all(ofstream &ofs);
	// Load from a file
	static void load_all(ifstream &ifs);
	// Set from a submitted page
	static void set_all();
	// Return JSON on a web page
	static json::value display_all();

	// Global Web options
	static BoolOption *fname_in_context;		// Remove common file prefix
	static BoolOption *show_true;			// Only show true identifier properties
	static BoolOption *show_line_number;		// Annotate source with line numbers
	static BoolOption *file_icase;			// File name case-insensitive match
	static BoolOption *sort_rev;			// Reverse sorting of query results
	static BoolOption *show_projects;		// Show associated projects
	static BoolOption *show_identical_files;	// Show a list of identical files

	static BoolOption *rename_override_ro;	// Renames will override read-only identifiers
	static BoolOption *refactor_fun_arg_override_ro;// Refactoring of function arguments will override read-only identifiers
	static BoolOption *refactor_check_clashes;	// Check for renamed identifier clashes when saving refactored code

	static IntegerOption *tab_width;		// Tab width for code output
	static TextOption *dot_graph_options;		// Graph options passed to dot
	static TextOption *dot_node_options;		// Node options passed to dot
	static TextOption *dot_edge_options;		// Edge options passed to dot
	static SelectionOption *cgraph_type;		// Call graph type t(text h(tml d(ot s(vg g(if
	static SelectionOption *cgraph_show;		// Call graph show e(dge n(ame f(ile p(ath
	static SelectionOption *fgraph_show;		// File graph show e(dge n(ame p(ath
	static TextOption *sfile_re_string;		// Saved files replacement location RE string
	static TextOption *sfile_repl_string;		// Saved files replacement string
	static TextOption *start_editor_cmd;		// Command to invoke an external editor
	static IntegerOption *entries_per_page;		// Number of elements to show in a page
	static IntegerOption *cgraph_depth;		// How deep to descend in a call graph
	static IntegerOption *fgraph_depth;		// How deep to descend in a file graph
	static BoolOption *cgraph_dot_url;		// Include URLs in dot output
	// Initialize the global web options
	static void initialize();
};

// A boolean value
class BoolOption : public Option {
private:
	bool	v;		// The value
public:
	// Constructor
	BoolOption(const char *sn, const char *un, bool iv = false) : Option(sn, un), v(iv) {}
	// Save to a file
	void save(ofstream &ofs) const { ofs << v; }
	// Load from a file
	void load(ifstream &ifs) { ifs >> v; }
	// Set from a submitted page
	void set() { v = !!server.getIntParam(short_name); }
	// Set from a submitted page
	bool get() { return v; }
	// Return JSON in form
	// {
	// 		html: "html code",
	// 		short_name: "short_name_value",
	// 		user_name: "user_name_value",
	// 		value: "value"
	// }
	json::value display() {
		json::value to_return;
		to_return["html"] = json::value::string(
			"<tr>"
			"<td>" + string(user_name) + "</td>\n"
			"<td><input type=\"checkbox\" name=\"" + short_name +
			"\" value=\"1\" "+ (v ? "checked" : "") + "></td>"
			"</tr>\n"
			);
		to_return["short_name"] = json::value(short_name);
		to_return["user_name"] = json::value(user_name);
		to_return["value"] = json::value(v);
		return to_return;
	}
};

// A radion selection value represented through a character code
class SelectionOption : public Option {
private:
	char	v;		// The value
	struct SelectionElement {
		char c;
		const char *name;
		SelectionElement(const char *s) : c(*s), name(s + 2) { csassert(strlen(s) > 2 && s[1] == ':'); }
	} ;
	vector <SelectionElement> options;
public:
	/*
	 * Construct from name, short name, initial value, and a NULL-terminated list
	 * list of arguments of the form "letter:Description"
	 */
	SelectionOption(const char *sn, const char *un, char iv, ...);
	// Save to a file
	void save(ofstream &ofs) const { ofs << v; }
	// Load from a file
	void load(ifstream &ifs) { ifs >> v; }
	// Set from a submitted page
	void set() {
		const char *m ;

		if ((m = server.getStrParam(short_name).c_str()))
			v = *m;
	}
	// Return the value
	char get() { return v; }
	// Return JSON
	json::value display();
};

// A textual value
class TextOption : public Option {
private:
	string	v;		// The value
	int size;		// Field size
public:
	// Constructor
	TextOption(const char *sn, const char *un, const string &iv = "", int s = 20) : Option(sn, un), v(iv), size(s) {}
	// Save to a file
	void save(ofstream &ofs) const { ofs << v; }
	// Load from a file
	void load(ifstream &ifs);
	// Set from a submitted page
	void set() { v = server.getStrParam(short_name); }
	// Erase the value
	void erase() { v.erase(); }
	// Return the value
	const string &get() { return v; }
	// Return JSON
	json::value display();
};

// A (positive) integer value
class IntegerOption : public Option {
private:
	int	v;		// The value
public:
	// Constructor
	IntegerOption(const char *sn, const char *un, int iv) : Option(sn, un), v(iv) {}
	// Save to a file
	void save(ofstream &ofs) const { ofs << v; }
	// Load from a file
	void load(ifstream &ifs) { ifs >> v; }
	// Set from a submitted page
	void set() {
		int vt;
		vt = (unsigned int)server.getIntParam(short_name);
		if (vt > 0)
			v = vt;
	}
	// Return the value
	int get() { return v; }
	// Return JSON in form
	// {
	// 		html: "html code",
	// 		short_name: "short_name_value",
	// 		user_name: "user_name_value",
	// 		value: "value"
	// }
	json::value display() {
		json::value to_return;
		to_return["html"] = json::value::string(
			"<tr>"
			"<td>"+string(user_name) + "</td>\n"
			"<td><input type=\"text\" name=\"" + short_name + "\" size=\"5\" maxlength=\"5\" value=\""
			+ to_string(v) + "\"></td>"
			"</tr>\n");
		to_return["short_name"] = json::value(short_name);
		to_return["user_name"] = json::value(user_name);
		to_return["value"] = json::value(v);

		return to_return;
	}
};


// A title separator for the options
// Not a real option
class TitleOption : public Option {
private:
public:
	// Constructor
	TitleOption(const char *un) : Option("", un) {}
	// Save to a file - noop
	void save(ofstream &ofs) const {}
	// Load from a file - noop
	void load(ifstream &ifs) {}
	// Set from a submitted page - noop
	void set() {}
	// Return true if the option can be saved
	virtual bool is_saveable() { return false; }
	// Return JSON in form
	// {
	// 		html: "html code",
	// 		user_name: "user_name_value"
	// }
	json::value display() {
		json::value to_return;
		to_return["html"] = json::value::string(
			"<tr><td class='opthead'>" + string(user_name) + "</td></tr>\n"
			);
		to_return["user_name"] = json::value::string(user_name);
		return to_return;
	}
};

#endif /* OPTION_ */
