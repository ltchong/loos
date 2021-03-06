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


#include <KernelStack.hpp>


namespace loos {
  namespace internal {

    void ValueStack::requireNotEmpty(void) const {
      if (values.size() == 0)
        throw(LOOSError("Operation requested on an empty stack."));
    }

    void ValueStack::push(const Value& val) { values.push_back(val); };

    Value ValueStack::pop(void) {
      requireNotEmpty();
      Value val = values.back();
      values.pop_back();
      return(val); 
    }

    // Duplicate the top entry...
    void ValueStack::dup(void) {
      requireNotEmpty();
      Value val = values.back();
      push(val);
    }

    // Just drop the top entry, i.e. (void)pop()
    void ValueStack::drop(void) {
      requireNotEmpty();
      values.pop_back();
    }

    // Peek at the top value without popping it...
    Value ValueStack::peek(int i) {
      if (i < 0)
        i += values.size();
      if ((unsigned int)i >= values.size())
        throw(LOOSError("Peeking beyond the stack!"));
      
      return(values[i]);
    }
    
    unsigned int ValueStack::size(void) const { return(values.size()); }

    void ValueStack::clear(void) { values.clear(); }

    std::ostream& operator<<(std::ostream& os, const ValueStack& s) {
      os << "<STACK>\n";
      std::vector<Value>::const_iterator i;
      
      for (i=s.values.begin(); i != s.values.end(); i++)
        os << "  " << *i << std::endl;
      
      return(os);
    }
    
  }
}

