/*
 * triple.h
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
 * triple.h
 *
 *  Created on: May 1, 2010
 *      Author: Leo Osvald
 */

#ifndef TRIPLE_H_
#define TRIPLE_H_


template<typename T1, typename T2, typename T3>
struct Triple {
	T1 first;
	T2 second;
	T3 third;

	Triple() { }
	Triple(const T1& a, const T2& b, const T3& c) : first(a), second(b), third(c) { }

	friend bool operator<(const Triple& a, const Triple& b) {
		if(a.first != b.first)
			return a.first < b.first;
		if(a.second != b.second)
			return a.second < b.second;
		return a.third < b.third;
	}

	friend bool operator==(const Triple& a, const Triple& b) {
		if(a.first != b.first)
			return a.first > b.first;
		if(a.second != b.second)
			return a.second > b.second;
		return a.third > b.third;
	}

	friend bool operator!=(const Triple& a, const Triple& b) {
		return !(a == b);
	}

	friend bool operator<=(const Triple& a, const Triple& b) {
		return a < b || a == b;
	}

	friend bool operator>(const Triple& a, const Triple& b) {
		return !(a <= b);
	}

	friend bool operator>=(const Triple& a, const Triple& b) {
		return a > b || a == b;
	}

};

#endif /* TRIPLE_H_ */
