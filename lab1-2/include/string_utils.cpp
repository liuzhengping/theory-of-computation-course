/*
 * string_utils.cpp
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
 * string_utils.cpp
 *
 *  Created on: Apr 13, 2010
 *      Author: Leo Osvald
 */

#include "string_utils.h"

#include <sstream>
#include <vector>
#include <set>

/**
 * Razdvaja string na temelju znakova u nizu delimiters.
 * Npr. split("a,b,cd.efg,", ",.") -> "a", "b", "cd", "efg"
 */
std::vector<std::string> StringUtils::split(const std::string& s,
		const std::string& delimiters) {
	std::vector<std::string> tokens;
	std::set<char> delim(delimiters.begin(), delimiters.end());
	int last = 0;
	for ( int i = 0; i <= (int)s.size(); ++i ) {
		if ( i == (int)s.size() || delim.find(s[i]) != delim.end() ) {
			if(i-last >= 1)
				tokens.push_back(std::string(s.begin()+last, s.begin()+i));
			last = i+1;
		}
	}
	return tokens;
}

//std::string StringUtils::to_bitstring(int mask, int n) {
//	std::string ret;
//	for(int i = n-1; i >= 0; --i)
//		ret.push_back(((1<<i) & mask) ? '1' : '0');
//	return ret;
//}
