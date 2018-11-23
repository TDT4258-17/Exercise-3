#include <fstream>
#include <iostream>
#include <string>

// g++ -o main main.cpp

int main()
{
	std::fstream file;
	std::fstream ofile;
	file.open("tile_textures/ice2.bmp", std::ios::in );
	ofile.open("converted_textures/ice2_tile.txt", std::ios::out );


	file.seekg(0, file.end);
	int size = file.tellg();
	std::cout << "Size: " << size << "\n";
	file.seekg(0, file.beg);

	char fileContent[size];
	file.read(fileContent, size);
	file.seekg(0, file.beg);

	ofile << "const unsigned short tile[400] = {\t";

	unsigned short rgb = 0;
	for (int i = (size-1); i >= (size-(400*3)); i -= 3)
	{
		rgb = ((((unsigned char)fileContent[i])/8));
		rgb = rgb << 6 | (((unsigned char)fileContent[i-1])/4);
		rgb = rgb << 5 | (((unsigned char)fileContent[i-2])/8);
		ofile << rgb;


		if (i > size + 4 - 1200)
			ofile << ", ";
		else
			std::cout << "Length of string: " << i+3 << "\n";

	//	if ((i+3) % 15 == 0)
	//		ofile << "\n\t\t\t\t\t\t\t\t\t\t";
	}

	ofile << "};";

	file.close();
	ofile.close();

	return 0;
}
