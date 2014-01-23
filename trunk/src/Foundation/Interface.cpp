#include "StdAfx.h"
#include "Interface.h"


void ISerializable::ReadString( std::ifstream& stream,std::string& str )
{
	unsigned char textLength = 0;
	char buffer[256];
	stream >> (unsigned char) textLength;
	if (textLength == 0)
		return;

	stream.read(buffer,textLength); 
	str = buffer;
}

void ISerializable::WriteString( std::ofstream& stream,std::string& str )
{
	unsigned char textLength = str.length();	
	stream << textLength;
	if (textLength == 0)
		return;

	stream << str;
}

void ISerializable::ReadMatrix( std::ifstream& stream,D3DXMATRIX& mat )
{
	for (int row=0;row<4;row++)
	{
		for (int column=0;column<4;column++)
		{
			stream >> (float)mat.m[row][column];
		}
	}
}

void ISerializable::WriteMatrix( std::ofstream& stream,D3DXMATRIX& mat )
{
	for (int row=0;row<4;row++)
	{
		for (int column=0;column<4;column++)
		{
			stream << (float)mat.m[row][column];
		}
	}
}
