/*
 * turing_machine_state.h
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
 * turing_machine_state.h
 *
 *  Created on: Jun 4, 2010
 *      Author: Leo Osvald
 */

#ifndef TURING_MACHINE_STATE_H_
#define TURING_MACHINE_STATE_H_

#include <string>
#include <iostream>

namespace turing_machine {

class State {
public:
	enum StateType {
		YES, NO, HALT,
		NON_FINAL
	};
private:
	std::string name_;
	StateType type_;
public:
	State(const std::string& name = "", StateType stateType = NON_FINAL);
	const std::string& name() const;
	bool isAccept() const;
	bool isReject() const;
	bool isHalt() const;

	friend bool operator<(const State& A, const State& B);
	friend bool operator==(const State& A, const State& B);
	friend std::ostream& operator<<(std::ostream& os, const State& s);
};

}

#endif /* TURING_MACHINE_STATE_H_ */
