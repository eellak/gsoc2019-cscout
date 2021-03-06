/*
 * (C) Copyright 2008-2015 Diomidis Spinellis
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
 * Web directory browsing functions
 *
 */

#ifndef DIRBROWSE_
#define DIRBROWSE_

#include <cstdio>
#include "headers.h"

using namespace std;

class Fileid;

// Add a file to the directory tree for later browsing
void *dir_add_file(Fileid f);
// Return a directory's contents
// 	Response JSON object in form
// 	{
// 		dir: "html code here",
// 	}
web::json::value dir_page(void *p);
// 	Return a URL and HTML for browsing the project's top dir
// 	Response JSON object in form
// 	{
// 		html: "html code here",
// 		addr: "dir?dir=Memory address" //resource link for the top directory
// 	}
json::value dir_top(const char *name);

#endif /* DIRBROWSE_ */
