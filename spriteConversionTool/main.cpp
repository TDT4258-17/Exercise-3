#include <fstream>
#include <iostream>
#include <string>

// g++ -o main main.cpp

int main()
{

	std::fstream file;
	std::fstream ofile;
	file.open("tile_textures/ice_break.bmp", std::ios::in );
	ofile.open("converted_textures/ice_break_tile.txt", std::ios::out );

	std::string string;
	std::string string2;
	std::string string3;

	// file.seekg(122, file.beg);

	file.seekg(0, file.end);
	int sizze = file.tellg();
	std::cout << "Size: " << sizze << "\n";
	file.seekg(0, file.beg);

	char fileContent[sizze];
	file.read(fileContent, sizze);
	file.seekg(0, file.beg);

	file >> string;
	std::cout << "Tellg1: " << file.tellg() << "\n";
	file >> string2;
	std::cout << "Tellg2: " << file.tellg() << "\n";
	file >> string3;
	std::cout << "Tellg3: " << file.tellg() << "\n";
	string = string + string2 + string3;

	unsigned int spriteNumber = 2;
	ofile << "const unsigned short sprite" << spriteNumber << "[400] = {\t";

	unsigned short rgb = 0;
	unsigned int size = string.size();
	std::cout << sizze << "\n";
	for (int i = (sizze-1); i >= (sizze-(400*3)); i -= 3)
	{
		rgb = ((((unsigned char)fileContent[i])/8));
		rgb = rgb << 6 | (((unsigned char)fileContent[i-1])/4);
		rgb = rgb << 5 | (((unsigned char)fileContent[i-2])/8);
		ofile << rgb;


		if (i > sizze + 4 - 1200)
			ofile << ", ";
		else
			std::cout << "Length of string: " << i+3 << "\n";

		if ((i+3) % 15 == 0)
			ofile << "\n\t\t\t\t\t\t\t\t\t\t";
	}

	ofile << "};";

	file.close();
	ofile.close();

	return 0;
}
