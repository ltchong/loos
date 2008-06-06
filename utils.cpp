/*
  This file is part of LOOS.

  LOOS (Lightweight Object-Oriented Structure library)
  Copyright (c) 2008, Tod D. Romo, Alan Grossfield
  Department of Biochemistry and Biophysics
  School of Medicine & Dentistry, University of Rochester

  This package (LOOS) is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation under version 3 of the License.

  This package is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pwd.h>

#include <string>
#include <sstream>


#include "utils.hpp"


string getNextLine(istream& is, int *lineno = 0) {
  string s;
  string::size_type i;

  for (;;) {
    if (getline(is, s, '\n').eof())
      break;

    if (lineno != 0)
      *lineno += 1;

    // Strip off comments
    i = s.find('#');
    if (i != string::npos)
      s.erase(i, s.length() - i);

    // Remove leading whitespace
    i = s.find_first_not_of(" \t");
    if (i > 0)
      s.erase(0, i);

    // Is string non-empty?
    if (s.length() != 0)
      break;
  }

  return(s);
}



vector<int> readIndexMap(istream& is) {
  vector<int> indices;
  int i;
  string s;

  for (;;) {
    s = getNextLine(is);
    if (s.length() == 0)
      break;

    istringstream iss(s);
    iss >> i;
    indices.push_back(i);
  }

  return(indices);
}



string invocationHeader(int argc, char *argv[]) {
  string invoke, user;
  int i;
  
  time_t t = time(0);
  string timestamp(asctime(localtime(&t)));
  timestamp.erase(timestamp.length() - 1, 1);

  struct passwd* pwd = getpwuid(getuid());
  if (pwd == 0)
    user = "UNKNOWN USER";
  else
    user = pwd->pw_name;

  invoke = string(argv[0]) + " ";
  string sep(" ");
  for (i=1; i<argc; i++) {
    if (i == argc-1)
      sep = "";
    invoke += "'" + string(argv[i]) + "'" + sep;
  }

  invoke += " - " + user + " (" + timestamp + ")";

  return(invoke);
}



GCoord boxFromRemarks(const Remarks& r) {
  int n = r.size();
  int i;

  GCoord c(99999.99, 99999.99, 99999.99);

  for (i=0; i<n; i++) {
    string s = r[i];
    if (s.substr(0, 6) == " XTAL ") {
      stringstream is(s.substr(5));
      if (!(is >> c.x()))
	throw(runtime_error("Unable to parse " + s));
      if (!(is >> c.y()))
	throw(runtime_error("Unable to parse " + s));
      if (!(is >> c.z()))
	throw(runtime_error("Unable to parse " + s));

      break;
    }
  }

  return(c);
}



bool remarksHasBox(const Remarks& r) {
  int n = r.size();
  for (int i = 0; i<n; i++) {
    string s = r[i];
    if (s.substr(0, 6) == " XTAL ")
      return(true);
  }
  return(false);
}



loos::base_generator_type& loos::rng_singleton(void) {
  static base_generator_type rng;

  return(rng);
}


void loos::randomSeedRNG(void) {
  base_generator_type& rng = rng_singleton();

  rng.seed(static_cast<unsigned int>(time(0)));
}
