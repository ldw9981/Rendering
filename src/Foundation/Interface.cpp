#include "StdAfx.h"
#include "Interface.h"

namespace Sophia
{


void ISerializable::ReadString( std::ifstream& stream,std::string& str )
{
	unsigned char textLength = 0;
	char buffer[256]={0,};
	stream.read((char*)&textLength,sizeof(textLength));
	if (textLength == 0)
		return;

	stream.read(&buffer[0],textLength); 
	str = buffer;
}

void ISerializable::WriteString( std::ofstream& stream,std::string& str )
{
	unsigned char textLength = str.length();	
	stream.write((char*)&textLength,sizeof(textLength));
	if (textLength == 0)
		return;

	stream.write(str.c_str(),textLength);
}

void ISerializable::ReadMatrix( std::ifstream& stream,D3DXMATRIX& mat )
{
	stream.read((char*)&mat,sizeof(D3DXMATRIX));
}

void ISerializable::WriteMatrix( std::ofstream& stream,D3DXMATRIX& mat )
{
	stream.write((char*)&mat,sizeof(D3DXMATRIX));
}

void ISerializable::ReadFloat( std::ifstream& stream,float& var )
{
	stream.read((char*)&var,sizeof(float));
}

void ISerializable::WriteFloat( std::ofstream& stream,float& var )
{
	stream.write((char*)&var,sizeof(float));
}

}