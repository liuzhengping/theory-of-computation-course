/*
 * turing_machine_state.cpp
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
 * turing_machine_state.cpp
 *
 *  Created on: Jun 4, 2010
 *      Author: Leo Osvald
 */

#include "turing_machine_state.h"

namespace turing_machine {

State::State(const std::string& name, StateType type) : name_(name), type_(type) {
}

const std::string& State::name() const {
	return name_;
}

bool State::isAccept() const {
	return type_ == YES;
}
bool State::isReject() const {
	return type_ == NO;
}
bool State::isHalt() const {
	return type_ == HALT;
}

bool operator<(const State& A, const State& B) {
	return A.name() < B.name();
}

bool operator==(const State& A, const State& B) {
	return A.name() == B.name();
}

std::ostream& operator<<(std::ostream& os, const State& s) {
	os << s.name();
	return os;
}

}
