/*
 * util.h
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

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <sstream>
#include <vector>
#include <string>
#include <set>

#define FORC(it, c) for( __typeof((c).begin()) it = (c).begin(); it != (c).end(); ++it)

class Utils {
public:

	template<typename T>
	static std::string vec_to_string(const std::vector<T>& v, const std::string& separator = ", ",
			const std::string& begin = "[", const std::string& end = "]") {
		std::stringstream ss;
		ss << begin;
		for (int i = 0; i < (int)v.size(); ++i) {
			if (i > 0) ss << separator;
			ss << v[i];
		}
		ss << end;
		return ss.str();
	}

	static std::vector<std::string> split(const std::string& s,
			const std::string& delimiters) {
		std::vector<std::string> tokens;
		std::set<char> delim(delimiters.begin(), delimiters.end());
		int last = 0;
		for ( int i = 0; i <= (int)s.size(); ++i ) {
			if ( i == (int)s.size() || delim.find(s[i]) != delim.end() ) {
				tokens.push_back(std::string(s.begin()+last, s.begin()+i));
				last = i+1;
			}
		}
		return tokens;
	}

	static std::string to_bitstring(int mask, int n) {
		std::string ret;
		for(int i = n-1; i >= 0; --i)
			ret.push_back(((1<<i) & mask) ? '1' : '0');
		return ret;
	}

};

#endif // UTIL_H_INCLUDED
