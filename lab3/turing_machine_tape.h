/*
 * turing_machine_tape.h
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
 * turing_machine_tape.h
 *
 *  Created on: Jun 4, 2010
 *      Author: Leo Osvald
 */

#ifndef TURING_MACHINE_TAPE_H_
#define TURING_MACHINE_TAPE_H_

#include <stack>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>

#include "turing_machine_base.h"
#include "alphabet.h"

#include "utils.h"
#include "string_utils.h"

namespace turing_machine {


template<typename SymbolType>
class Tape {
public:

	struct InvalidHeadPositionException : public Exception {
		InvalidHeadPositionException()
		: Exception("Glava je otisla izvan najlijevijeg mjesta") {
		}
	};

	struct InvalidTapeSymbol : public Exception {
		template<typename SymbolType2>
		InvalidTapeSymbol(const SymbolType2& symbol)
				: Exception("Nevaljani znak trake: '"
						+StringUtils::toString(symbol)+"'") {
		}
	};
private:
	mutable std::stack<SymbolType> left_;
	mutable std::stack<SymbolType> right_;
	SymbolType current_;
	const Alphabet<SymbolType>& alphabet_;
public:
	Tape(const Alphabet<SymbolType>& tape_alphabet);

	const Alphabet<SymbolType>& tape_alphabet() const;
	void moveLeft();
	void moveRight();

	template <class InputIterator>
	Tape(const Alphabet<SymbolType>& tape_alphabet,
			InputIterator iterFrom, InputIterator iterTo)
			: alphabet_(tape_alphabet) {
		int n = 0;
		for (; iterFrom != iterTo; ++iterFrom, ++n) {
			if(!tape_alphabet.contains(*iterFrom)) {
				throw InvalidTapeSymbol(*iterFrom);
			}
			left_.push(*iterFrom);
		}
		if(n == 0) {
			left_.push(tape_alphabet.blank_symbol());
			++n;
		}

		current_ = left_.top();
		left_.pop();
		n--;

		while(n--) {
			moveLeft();
		}
	}



	const SymbolType& read() const {
		return current_;
	}

	void write(SymbolType symbol) {
		current_ = symbol;
	}

	std::string toString() const;

private:
	static std::vector<SymbolType> toVector(std::stack<SymbolType>& s);

	template<typename SymbolType2>
	friend std::ostream& operator<<(std::ostream& os, const Tape<SymbolType2>& tape);
};



template<typename AlphabetType>
Tape<AlphabetType>::Tape(const Alphabet<AlphabetType>& tape_alphabet)
: alphabet_(tape_alphabet) {
	// TODO Auto-generated constructor stub

}

template<typename AlphabetType>
const Alphabet<AlphabetType>& Tape<AlphabetType>::tape_alphabet() const {
	return alphabet_;
}

template<typename AlphabetType>
void Tape<AlphabetType>::moveLeft() {
  right_.push(current_);
  if(!left_.empty()) {
    current_ = left_.top();
    left_.pop();
  }
  else {
    current_ = alphabet_.blank_symbol();
  }
}

template<typename AlphabetType>
void Tape<AlphabetType>::moveRight() {
	left_.push(current_);
	if(!right_.empty()) {
		current_ = right_.top();
		right_.pop();
	}
	else {
		current_ = alphabet_.blank_symbol();
	}
}

template<typename AlphabetType> std::string Tape<AlphabetType>::toString() const {
	using namespace std;
	stringstream ss;
	vector<AlphabetType> v = toVector(left_);
	reverse(v.begin(), v.end());
	ss << StringUtils::to_string(v.begin(), v.end(), " ", "", "");
	ss << "[" << current_ << "]";
	v = toVector(right_);
	ss << StringUtils::to_string(v.begin(), v.end(), " ", "", "");
	return ss.str();
}

template<typename AlphabetType>
std::vector<AlphabetType>
Tape<AlphabetType>::toVector(std::stack<AlphabetType>& s) {
	std::vector<AlphabetType> ret;
	ret.reserve(s.size());
	for(; !s.empty(); s.pop())
		ret.push_back(s.top());
	for(int i = (int)ret.size()-1; i >= 0; --i)
		s.push(ret[i]);
	return ret;
}

template<typename AlphabetType>
std::ostream& operator<<(std::ostream& os, const Tape<AlphabetType>& tape) {
	os << tape.toString();
	return os;
}

}

#endif /* TURING_MACHINE_TAPE_H_ */
