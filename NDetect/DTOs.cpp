#include "pch.h"
#include "DTOs.h"


Packet::Packet(u_char source_byte1, u_char source_byte2, u_char source_byte3, u_char source_byte4, u_short source_port, u_char dest_byte1, u_char dest_byte2, u_char dest_byte3, u_char dest_byte4, u_short dest_port) {


}

Packet::Packet()
{
}



Packet::~Packet()
{
}

void Packet::SetSource(u_char s_byte1, u_char s_byte2, u_char s_byte3, u_char s_byte4, u_short s_port)
{
	this->source_byte1 = s_byte1;
	this->source_byte2 = s_byte2;
	this->source_byte3 = s_byte3;
	this->source_byte4 = s_byte4;
	this->source_port = s_port;
}

void Packet::SetDestination(u_char d_byte1, u_char d_byte2, u_char d_byte3, u_char d_byte4, u_short d_port)
{
	this->dest_byte1 = d_byte1;
	this->dest_byte2 = d_byte2;
	this->dest_byte3 = d_byte3;
	this->dest_byte4 = d_byte4;
	this->dest_port = d_port;

}
