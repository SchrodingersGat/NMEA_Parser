#include "nmea.h"
#include <string.h>
#include <stdio.h>

char DECODE_BUFFER[GPS_BUFFER_SIZE];

void NMEA_InitBuffer(NMEABuffer_t *buf)
{
	memset(buf,sizeof(buf),0);
}

void NMEA_InitData(NMEAData_t *gps)
{
	memset(gps,sizeof(gps),0);
}


unsigned char NMEA_LookForPacket(NMEABuffer_t *buffer, char byte)
{
	unsigned char result = PKT_NOT_YET_AVAILABLE;

	switch (buffer->state)
	{
	default:
		buffer->cursor = 0;
		buffer->state = STATE_START_OF_PACKET;
	case STATE_START_OF_PACKET:
		if (byte == '$')
		{
			buffer->data[0] = '$';
			buffer->state = STATE_G;
		}
		break;
	case STATE_G:
		if (byte == 'G')
		{
			buffer->state = STATE_P;
			buffer->data[1] = 'G';
		}
		else
		{
			buffer->state = STATE_START_OF_PACKET;
		}
		break;
	case STATE_P:
		if (byte == 'P')
		{
			buffer->state = STATE_DATA;
			buffer->data[2] = 'P';
			buffer->cursor = 3;
		}
		else
		{
			buffer->state = STATE_START_OF_PACKET;
		}
		break;
	case STATE_DATA:
		buffer->data[buffer->cursor++] = byte;
		if (byte == '\r') //signifies end of packet
		{
			buffer->state = STATE_LOOKING_FOR_LF;
		}
		break;
	case STATE_LOOKING_FOR_LF:
		if (byte == '\n')
		{
			buffer->data[buffer->cursor] = byte;
			buffer->data[buffer->cursor + 1] = 0; //Terminate the string

			result = PKT_AVAILABLE; //pkt available, might be OK (comp against checksum
		}
		buffer->cursor = 0;
		buffer->state = STATE_START_OF_PACKET;
		break;
	}

	if (buffer->cursor >= GPS_BUFFER_SIZE)
	{
		buffer->state = STATE_START_OF_PACKET;
		buffer->cursor = 0;
	}

	return result;
}

char NMEA_GetPacketType(NMEABuffer_t *buf)
{
	char pktType = PKT_UNKNOWN;

	char *ptr = &buf->data[3]; //start at the 4th byte as we know the first three are always $GP

	if (buf->data[0] != '$')
	{
		pktType = PKT_UNKNOWN;
	}
	else if (strncmp(ptr,"GGA",3) == 0)
	{
		pktType = PKT_GGA;
	}
	else if (strncmp(ptr,"GLL",3) == 0)
	{
		pktType = PKT_GLL;
	}
	else if (strncmp(ptr,"GSA",3) == 0)
	{
		pktType = PKT_GSA;
	}
	else if (strncmp(ptr,"GSV",3) == 0)
	{
		pktType = PKT_GSV;
	}
	else if (strncmp(ptr,"RMC",3) == 0)
	{
		pktType = PKT_RMC;
	}
	else if (strncmp(ptr,"VTG",3) == 0)
	{
		pktType = PKT_VTG;
	}
	else if (strncmp(ptr,"ZDA",3) == 0)
	{
		pktType = PKT_ZDA;
	}
	else if (strncmp(ptr,"DTM",3) == 0)
	{
		pktType = PKT_DTM;
	}

	return pktType;
}

void NMEA_DecodePacket(NMEABuffer_t *buf, NMEAData_t *gps)
{
	char pktType = NMEA_GetPacketType(buf);

	switch (pktType)
	{
	case PKT_GGA:
		NMEA_DecodeGGAPacket(buf,gps);
		break;
	case PKT_GLL:
		NMEA_DecodeGLLPacket(buf,gps);
		break;
	case PKT_GSA:
		NMEA_DecodeGSAPacket(buf,gps);
		break;
	case PKT_GSV:
		NMEA_DecodeGSVPacket(buf,gps);
		break;
	case PKT_RMC:
		NMEA_DecodeRMCPacket(buf,gps);
		break;
	case PKT_VTG:
		NMEA_DecodeVTGPacket(buf,gps);
		break;
	case PKT_ZDA:
		NMEA_DecodeZDAPacket(buf,gps);
		break;
	case PKT_DTM:
		NMEA_DecodeDTMPacket(buf,gps);
		break;
	}
}

void NMEA_DecodeGGAPacket(NMEABuffer_t *buf, NMEAData_t *gps)
{
	/* Format
	 * MSG ID - $GPGGA
	 * UTC Position: hhmmss.sss
	 * Latitude: ddmm.mmmm
	 * NS indicator: N/S
	 * Longitude: dddmm.mmmm
	 * EW Indicator: E/W
	 * Position Fix: {0,1,2,3}
	 * Satellites used: {0 -> 12}
	 * HDOP: x.y
	 * MSL Altitude: mmm.mm
	 * AltUnit: M
	 * GeoSep: m.mm
	 * GeoSepUnit M
	 * Age of Diff.Corr <NULL>
	 * Diff.Ref.Station ID <NULL>
	 * *
	 * Checksum: nn
	 * <CR>
	 * <LF>
	 */
}

void NMEA_DecodeGLLPacket(NMEABuffer_t *buf, NMEAData_t *gps)
{
	/* Message Format:
	 *
	 */
}

void NMEA_DecodeGSAPacket(NMEABuffer_t *buf, NMEAData_t *gps)
{

}

void NMEA_DecodeGSVPacket(NMEABuffer_t *buf, NMEAData_t *gps)
{

}

void NMEA_DecodeRMCPacket(NMEABuffer_t *buf, NMEAData_t *gps)
{
	/* RMC (Recommended Minimum Fix)
	 *
	 * Format:
	 * $GPRMC
	 * UTS Position: hhmmss.ss
	 * Status: A/V (A = Valid, V = Data Not Valid)
	 * Lat: ddmm.mmmm
	 * N/S Indicator: N/S
	 * Lon: dddmm.mmmm
	 * E/W Indicator: E/W
	 * SOG: x.x (knots)
	 * COG: ddd.d (deg)
	 * Date (UTC): ddmmyy
	 * Magnetic Variation: <NULL>
	 * Magnetic Variation Direction: <NULL>
	 * Fix Mode:
	 * *
	 * Checksm: CC
	 * <CR>
	 * <LF>
	 */

}

void NMEA_DecodeVTGPacket(NMEABuffer_t *buf, NMEAData_t *gps)
{

}

void NMEA_DecodeZDAPacket(NMEABuffer_t *buf, NMEAData_t *gps)
{

}

void NMEA_DecodeDTMPacket(NMEABuffer_t *buf, NMEAData_t *gps)
{

}
