/*
 * alphabet.h
 * 
 * Copyright (C) 2010 Leo Osvald <leo.osvald@gmail.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License.
 *  
 * You may obtain a copy of the License at 
 * 		http://www.apache.org/licenses/LICENSE-2.0
 *  
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * See the License for the specific language governing permissions and 
 * limitations under the License. 
 */
/*
 * alphabet.h
 *
 *  Created on: Jun 4, 2010
 *      Author: Leo Osvald
 */

#ifndef ALPHABET_H_
#define ALPHABET_H_

#include <set>

namespace turing_machine {

template<typename SymbolType>
class Alphabet {
private:
	std::set<SymbolType> s_;
	SymbolType blank_;
public:
	Alphabet(SymbolType blank = '#') : blank_(blank) {
	}

	SymbolType blank_symbol() const {
		return blank_;
	}

	void set_blank_symbol(SymbolType symbol) {
		blank_ = symbol;
	}

	void add(SymbolType symbol) {
		s_.insert(symbol);
	}

	bool contains(SymbolType symbol) const {
		return s_.find(symbol) != s_.end();
	}

	bool isBlank(SymbolType symbol) const {
		return symbol == blank_;
	}

	template<typename SymbolType2>
	friend std::ostream& operator<<(std::ostream& os,
			const Alphabet<SymbolType2>& a) {
		os << StringUtils::to_string(a.s_.begin(), a.s_.end())
		 << "(blank = " << a.blank_symbol() << ")";
		return os;
	}
};

}

#endif /* ALPHABET_H_ */
