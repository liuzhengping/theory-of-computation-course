/*
 * string_utils.h
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
 * string_utils.h
 *
 *  Created on: Apr 13, 2010
 *      Author: Leo Osvald
 */

#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

#include <string>
#include <vector>
#include <sstream>

class StringUtils {
private:
	StringUtils();

public:

	/**
	 * Razdvaja string na temelju znakova u nizu delimiters.
	 * Npr. split("a,b,cd.efg,", ",.") -> "a", "b", "cd", "efg"
	 */
	static std::vector<std::string> split(const std::string& s,
			const std::string& delimiters);

	template <class InputIterator>
	static std::string to_string(InputIterator iterFrom, InputIterator iterTo,
			const std::string& separator = ", ",
			const std::string& begin = "[", const std::string& end = "]") {
		std::stringstream ss;
		ss << begin;
		for (bool first = true; iterFrom != iterTo; ++iterFrom) {
			if (!first) ss << separator;
			else first = false;
			ss << *iterFrom;
		}
		ss << end;
		return ss.str();
	}

	template<typename Tx, typename Ty>
	static std::string to_string(const std::pair<Tx, Ty>& _pair,
			const std::string& separator = ", ",
			const std::string& begin = "(", const std::string& end = ")") {
		std::stringstream ss;
		ss << begin << _pair.first << separator << _pair.second << end;
		return ss.str();
	}

	static inline std::string trim_right(const std::string& source,
			const std::string& stripChars = " ") {
		int i;
		for(i = (int)source.size()-1; i >= 0
		&& stripChars.find(source[i]) != std::string::npos; --i)
			;
		return std::string(source.begin(), source.begin()+i+1);
	}

	static inline std::string trim_left(const std::string& source,
			const std::string& stripChars = " ") {
		int i;
		for(i = 0; i < (int)source.size()
		&& stripChars.find(source[i]) != std::string::npos; ++i)
			;
		return std::string(source.begin()+i, source.end());
	}

	static inline std::string trim(const std::string& source,
			const std::string& stripChars = " ") {
		return trim_left(trim_right(source, stripChars), stripChars);
	}

	template<typename T>
	static std::string to_bitstring(T mask, int n) {
		std::string ret;
		for(int i = 0; i < n; ++i, mask >>= 1) {
			ret.push_back((mask & 1) ? '1' : '0');
		}
		return trim_right(ret, "0");
	}

	template<typename T>
	static std::string to_bitstring(T mask) {
		return to_bitstring<T>(mask, sizeof(mask)*8);
	}

	template<typename T>
	static std::string toString(const T& t) {
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

};

#endif /* STRING_UTILS_H_ */
