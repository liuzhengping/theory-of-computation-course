/*
 * turing_machine_transition.h
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
 * turing_machine_transition.h
 *
 *  Created on: Jun 4, 2010
 *      Author: Leo Osvald
 */

#ifndef TURING_MACHINE_TRANSITION_H_
#define TURING_MACHINE_TRANSITION_H_

#include <iostream>

#include "turing_machine_state.h"

namespace turing_machine {

template<typename TapeSymbol>
struct Transition {
	enum HeadMoveDirection {
		LEFT, RIGHT, HOLD
	};

	const State& from;
	const TapeSymbol read_symbol;
	const State& to;
	const TapeSymbol write_symbol;
	const HeadMoveDirection head_move_dir;

	Transition(const State& from, TapeSymbol read_symbol,
			const State& to,
			TapeSymbol write_symbol,
			HeadMoveDirection head_move_dir)
	: from(from), read_symbol(read_symbol), to(to),
	  write_symbol(write_symbol), head_move_dir(head_move_dir) {
	}

	template<typename TapeSymbol2>
	friend std::ostream& operator<<(std::ostream& os,
			const Transition<TapeSymbol2>& t) {
		char dir = 'H';
		if(t.head_move_dir == LEFT)
			dir = 'L';
		else if(t.head_move_dir == RIGHT)
			dir = 'R';
		os << "d(" << t.from << ", " << t.read_symbol << ") = ("
				<< t.to << ", " << t.write_symbol << ", " << dir << ")";
		return os;
	}

};


//template<typename TapeSymbol>
//Transition<TapeSymbol>::Transition(const State& from, TapeSymbol read_symbol,
//		const State& to,
//		TapeSymbol write_symbol,
//		HeadMoveDirection head_move_dir)
//: from(from), read_symbol(read_symbol), to(to),
//  write_symbol(write_symbol),
//  head_move_dir(head_move_dir) {
//
//}


}

#endif /* TURING_MACHINE_TRANSITION_H_ */
