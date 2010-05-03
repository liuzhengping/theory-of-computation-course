/*
 * test.h
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
 * test.h
 *
 *  Created on: May 3, 2010
 *      Author: Leo Osvald
 */

#ifndef TEST_H_
#define TEST_H_

#include "generator.h"
#include "simulator.h"

namespace test {

void generiraj() {

	MyGenerator gen("res/gen.in");
	gen.output("res/def");
}

void simuliraj() {
	MyPA pa;
	pa.build_from_file("res/def"); //XXX
	MySimulator sim(&pa);
	sim.simulate_from_file("res/sim.in");
}

}

#endif /* TEST_H_ */
