#ifndef _NMEA_H
#define _NMEA_H


#define GPS_BUFFER_SIZE 80

typedef struct
{
	char data[GPS_BUFFER_SIZE + 1]; //ensure that the final byte is always zero for strcmp
	unsigned char cursor;
	unsigned char state;
} NMEABuffer_t;

typedef enum
{
	STATE_START_OF_PACKET,
	STATE_G,
	STATE_P,
	STATE_DATA,
	STATE_LOOKING_FOR_LF
} NMEAPacketState_t;

typedef enum
{
	PKT_NOT_YET_AVAILABLE = 0,
	PKT_AVAILABLE,
	PKT_ERROR
} NMEAPacketStatus_t;

typedef enum
{
	PKT_UNKNOWN = 0,
	PKT_GGA = 1,
	PKT_GLL,
	PKT_GSA,
	PKT_GSV,
	PKT_RMC,
	PKT_VTG,
	PKT_ZDA,
	PKT_DTM
} NMEAPacketType_t;

typedef struct
{
	double lat;
	double lon;

	double alt;

	unsigned char day;
	unsigned char month;
	unsigned short year;

	unsigned char fix;
} NMEAData_t;

void NMEA_InitBuffer(NMEABuffer_t *buf);
void NMEA_InitData(NMEAData_t *gps);

unsigned char NMEA_LookForPacket(NMEABuffer_t *buffer, char byte);

char NMEA_GetPacketType(NMEABuffer_t *buf);

void NMEA_DecodePacket(NMEABuffer_t *buf, NMEAData_t *gps);
void NMEA_DecodeGGAPacket(NMEABuffer_t *buf, NMEAData_t *gps);
void NMEA_DecodeGLLPacket(NMEABuffer_t *buf, NMEAData_t *gps);
void NMEA_DecodeGSAPacket(NMEABuffer_t *buf, NMEAData_t *gps);
void NMEA_DecodeGSVPacket(NMEABuffer_t *buf, NMEAData_t *gps);
void NMEA_DecodeRMCPacket(NMEABuffer_t *buf, NMEAData_t *gps);
void NMEA_DecodeVTGPacket(NMEABuffer_t *buf, NMEAData_t *gps);
void NMEA_DecodeZDAPacket(NMEABuffer_t *buf, NMEAData_t *gps);
void NMEA_DecodeDTMPacket(NMEABuffer_t *buf, NMEAData_t *gps);


#endif
