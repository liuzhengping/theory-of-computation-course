/*
 * main.cpp
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
 * main.cpp
 *
 *  Created on: May 3, 2010
 *      Author: Leo Osvald
 */

#include "generator.h"
#include "simulator.h"

#include <cstring>

static const char* GEN_OPTION = "gen";
static const char* SIM_OPTION = "sim";

#include "test.h"

void testAll() {
	test::generiraj();
	test::simuliraj();
}

int main(int argc, char **argv) {

//	testAll();

	if(argc <= 2 || (strcmp(argv[1], GEN_OPTION) != 0
			&& strcmp(argv[1], SIM_OPTION) != 0)) {
		printf("Usage: \tgen (generator input) (definition file)\n\
\tsim (definition file) (simulation file)\n");
		return 1;
	}

	if(strcmp(argv[1], GEN_OPTION) == 0) {
		static MyGenerator gen(argv[2]);
		gen.output(argv[3]);
	}
	else {
		static MyPA pa;
		pa.build_from_file(argv[2]);
		static MySimulator sim(&pa);
		sim.simulate_from_file(argv[3]);
	}

	return 0;
}
