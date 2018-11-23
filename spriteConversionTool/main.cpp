#include <fstream>
#include <iostream>
#include <string>

int main()
{

	std::fstream file;
	std::fstream ofile;
	file.open("player.bmp", std::ios::in );
	ofile.open("test_tile.txt", std::ios::out );

	std::string string;
	std::string ostring;

	file.seekg(122, file.beg);

	file >> string;

	unsigned int spriteNumber = 2;
	ofile << "const unsigned short sprite" << spriteNumber << "[400] = {\t";

	unsigned short rgb = 0;
	unsigned int size = string.size();
	for (int i = 0; i < size; i += 3)
	{
		rgb = ((((unsigned char)string[i])/8));
		rgb = rgb << 6 | (((unsigned char)string[i+1])/4);
		rgb = rgb << 5 | (((unsigned char)string[i+2])/8);
		ofile << rgb;


		if (i < size - 4)
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