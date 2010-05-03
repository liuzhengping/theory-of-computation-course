/*
 * tuple.h
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
 * tuple.h
 *
 *  Created on: May 3, 2010
 *      Author: Leo Osvald
 */

#ifndef TUPLE_H_
#define TUPLE_H_

template<typename T1, typename T2, typename T3, typename T4>
struct Tuple {
	T1 first;
	T2 second;
	T3 third;
	T4 fourth;
	Tuple() { }
	Tuple(T1 a, T2 b, T3 c, T4 d) : first(a), second(b), third(c), fourth(d) { }
};

#endif /* TUPLE_H_ */
