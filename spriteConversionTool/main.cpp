#include <fstream>
#include <iostream>
#include <string>

// g++ -o main main.cpp

int main()
{
	std::fstream file;
	std::fstream ofile;

	file.open("tile_textures/leporcside2.bmp", std::ios::in );
	ofile.open("converted_textures/leporcside2_sprite.txt", std::ios::out );
	const int imageWidth = 16;
	const int imageHeight = 16;
	const int pixelCount = imageWidth*imageHeight;


	file.seekg(0, file.end);
	int size = file.tellg();
	file.seekg(0, file.beg);

	char fileContent[size];
	file.read(fileContent, size);
	file.seekg(0, file.beg);

	int pixelReadCount = 0;
	int currentRow = imageHeight;
	unsigned short rgb = 0;
	unsigned short rgbArray[pixelCount];
	int paddingPerRow = (4 - ((imageWidth*3)%4))%4;
	int byteCount = pixelCount*3 + imageHeight*paddingPerRow;
	
	
	

	
	for (int i = (size-1); i >= (size-byteCount); i -= 3)
	{
		if (i)
		rgb = ((((unsigned char)fileContent[i])/8));
		rgb = rgb << 6 | (((unsigned char)fileContent[i-1])/4);
		rgb = rgb << 5 | (((unsigned char)fileContent[i-2])/8);

		rgbArray[pixelReadCount++] = rgb;
		
		if (pixelReadCount%imageWidth == 0)
		{
			i -= paddingPerRow;
		}
	}
	
	std::cout << "Filesize in bytes: " << size << "\n";
	std::cout << "Byte padding each row: " << paddingPerRow << "\n";
	std::cout << "Imagesize in bytes without padding: " << pixelCount*3 << "\n";
	std::cout << "Imagesize in bytes with padding: " << byteCount << "\n";
	std::cout << "Pixel read count: " << pixelReadCount << "\n";
	

	ofile << "const unsigned short tile[" << pixelCount << "] = {\t";
	for (int j = 0; j < imageHeight; j++)
	{
		int index = 0;
		for(int i = 0; i < imageWidth; i++)
		{
			index = j*imageWidth + (imageWidth - i - 1);
			ofile << rgbArray[index];	
			if ((j*imageWidth + i) != (pixelCount-1))
				ofile << ", ";	
		}
	}

	ofile << "};";

	file.close();
	ofile.close();

	return 0;
}










