/*
 * Adafruit_S25FL1.cpp
 *
 *  Created on: Jan 2, 2018
 *      Author: deanm
 */


#include "Adafruit_S25FL1.h"

enum {
	S25FL1_READ_STATUS_2 = 0,
	S25FL1_READ_STATUS_3,
	S25FL1_WRITE_ENABLE_STATUS,
};

static const QSPIInstr cmdSetS25FL1[] = {
		//read status 2
		{ 0x35, false, QSPI_ADDRLEN_24_BITS, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN | QSPI_OPTION_ADDREN), QSPI_READ, 0 },
		//read status 3
		{ 0x33, false, QSPI_ADDRLEN_24_BITS, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN | QSPI_OPTION_ADDREN), QSPI_READ, 0 },
		//write enable status
		{ 0x50, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN), QSPI_READ, 0 },
};

bool Adafruit_S25FL1::begin()
{
	Adafruit_Flash_Generic::begin();

	//enable quad
	writeStatus(0x00, 0x06, 0x70);

	uint8_t s2;
	QSPI0.runInstruction(&cmdSetS25FL1[S25FL1_READ_STATUS_2], 0, NULL, &s2, 1);

	return (s2 == 0x06);
}

void Adafruit_S25FL1::writeStatus(byte s1, byte s2, byte s3)
{
	uint8_t c[] = {s1, s2, s3};

	uint8_t dummy;
	QSPI0.runInstruction(&cmdSetGeneric[FLASH_GENERIC_CMD_WRITE_ENABLE], 0, NULL, &dummy, 1);
	QSPI0.runInstruction(&cmdSetS25FL1[S25FL1_WRITE_ENABLE_STATUS], 0, NULL, &dummy, 1);

	QSPI0.runInstruction(&cmdSetGeneric[FLASH_GENERIC_CMD_WRITE_STATUS], 0, c, NULL, 3);
}

