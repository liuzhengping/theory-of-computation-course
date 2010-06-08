/*
 * turing_machine_base.h
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
 * turing_machine_base.h
 *
 *  Created on: Jun 6, 2010
 *      Author: Leo Osvald
 */

#ifndef TURING_MACHINE_BASE_H_
#define TURING_MACHINE_BASE_H_

#include <stdexcept>
#include <string>

namespace turing_machine {

struct Exception : public std::logic_error {
	Exception(const std::string& details);
};

}

#endif /* TURING_MACHINE_BASE_H_ */
