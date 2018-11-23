#include <fstream>
#include <iostream>
#include <string>

// g++ -o main main.cpp

int main()
{

	std::fstream file;
	std::fstream ofile;
	file.open("test.bmp", std::ios::in );
	ofile.open("colour_test_tile.txt", std::ios::out );

	std::string string;
	std::string string2;
	std::string string3;

	// file.seekg(122, file.beg);

	file >> string;
	file >> string2;
	file >> string3;
	string = string + string2 + string3;

	unsigned int spriteNumber = 2;
	ofile << "const unsigned short sprite" << spriteNumber << "[400] = {\t";

	unsigned short rgb = 0;
	unsigned int size = string.size();
	std::cout << size << "\n";
	for (int i = (size-1); i >= (size-(400*3)); i -= 3)
	{
		rgb = ((((unsigned char)string[i])/8));
		rgb = rgb << 6 | (((unsigned char)string[i-1])/4);
		rgb = rgb << 5 | (((unsigned char)string[i-2])/8);
		ofile << rgb;


		if (i > size + 4 - 1200)
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
