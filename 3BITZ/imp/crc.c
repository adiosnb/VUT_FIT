/*********************************************************************************
 *  Autor : Adrián Tomašov
 *  Login : xtomas32
 *  Vlastníctvo : 80%
 *  Zmeny : prepisanie inicializacnych funkcii pre CRC modul,
 *          dopisanie dalsich funkcii pre vypocet CRC
 *
*********************************************************************************/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_crc.h"

#include "pin_mux.h"
#include "clock_config.h"

#include <stdint.h>

uint16_t crc16_ccitt_table[] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
	0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
	0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
	0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
	0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
	0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
	0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
	0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
	0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
	0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
	0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
	0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
	0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
	0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
	0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
	0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
	0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
	0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
	0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
	0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
	0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
	0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
	0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0,
};

static uint32_t crc32_lookup_table[] = {
	0x00000000L, 0x77073096L, 0xEE0E612CL, 0x990951BAL,
	0x076DC419L, 0x706AF48FL, 0xE963A535L, 0x9E6495A3L,
	0x0EDB8832L, 0x79DCB8A4L, 0xE0D5E91EL, 0x97D2D988L,
	0x09B64C2BL, 0x7EB17CBDL, 0xE7B82D07L, 0x90BF1D91L,
	0x1DB71064L, 0x6AB020F2L, 0xF3B97148L, 0x84BE41DEL,
	0x1ADAD47DL, 0x6DDDE4EBL, 0xF4D4B551L, 0x83D385C7L,
	0x136C9856L, 0x646BA8C0L, 0xFD62F97AL, 0x8A65C9ECL,
	0x14015C4FL, 0x63066CD9L, 0xFA0F3D63L, 0x8D080DF5L,
	0x3B6E20C8L, 0x4C69105EL, 0xD56041E4L, 0xA2677172L,
	0x3C03E4D1L, 0x4B04D447L, 0xD20D85FDL, 0xA50AB56BL,
	0x35B5A8FAL, 0x42B2986CL, 0xDBBBC9D6L, 0xACBCF940L,
	0x32D86CE3L, 0x45DF5C75L, 0xDCD60DCFL, 0xABD13D59L,
	0x26D930ACL, 0x51DE003AL, 0xC8D75180L, 0xBFD06116L,
	0x21B4F4B5L, 0x56B3C423L, 0xCFBA9599L, 0xB8BDA50FL,
	0x2802B89EL, 0x5F058808L, 0xC60CD9B2L, 0xB10BE924L,
	0x2F6F7C87L, 0x58684C11L, 0xC1611DABL, 0xB6662D3DL,
	0x76DC4190L, 0x01DB7106L, 0x98D220BCL, 0xEFD5102AL,
	0x71B18589L, 0x06B6B51FL, 0x9FBFE4A5L, 0xE8B8D433L,
	0x7807C9A2L, 0x0F00F934L, 0x9609A88EL, 0xE10E9818L,
	0x7F6A0DBBL, 0x086D3D2DL, 0x91646C97L, 0xE6635C01L,
	0x6B6B51F4L, 0x1C6C6162L, 0x856530D8L, 0xF262004EL,
	0x6C0695EDL, 0x1B01A57BL, 0x8208F4C1L, 0xF50FC457L,
	0x65B0D9C6L, 0x12B7E950L, 0x8BBEB8EAL, 0xFCB9887CL,
	0x62DD1DDFL, 0x15DA2D49L, 0x8CD37CF3L, 0xFBD44C65L,
	0x4DB26158L, 0x3AB551CEL, 0xA3BC0074L, 0xD4BB30E2L,
	0x4ADFA541L, 0x3DD895D7L, 0xA4D1C46DL, 0xD3D6F4FBL,
	0x4369E96AL, 0x346ED9FCL, 0xAD678846L, 0xDA60B8D0L,
	0x44042D73L, 0x33031DE5L, 0xAA0A4C5FL, 0xDD0D7CC9L,
	0x5005713CL, 0x270241AAL, 0xBE0B1010L, 0xC90C2086L,
	0x5768B525L, 0x206F85B3L, 0xB966D409L, 0xCE61E49FL,
	0x5EDEF90EL, 0x29D9C998L, 0xB0D09822L, 0xC7D7A8B4L,
	0x59B33D17L, 0x2EB40D81L, 0xB7BD5C3BL, 0xC0BA6CADL,
	0xEDB88320L, 0x9ABFB3B6L, 0x03B6E20CL, 0x74B1D29AL,
	0xEAD54739L, 0x9DD277AFL, 0x04DB2615L, 0x73DC1683L,
	0xE3630B12L, 0x94643B84L, 0x0D6D6A3EL, 0x7A6A5AA8L,
	0xE40ECF0BL, 0x9309FF9DL, 0x0A00AE27L, 0x7D079EB1L,
	0xF00F9344L, 0x8708A3D2L, 0x1E01F268L, 0x6906C2FEL,
	0xF762575DL, 0x806567CBL, 0x196C3671L, 0x6E6B06E7L,
	0xFED41B76L, 0x89D32BE0L, 0x10DA7A5AL, 0x67DD4ACCL,
	0xF9B9DF6FL, 0x8EBEEFF9L, 0x17B7BE43L, 0x60B08ED5L,
	0xD6D6A3E8L, 0xA1D1937EL, 0x38D8C2C4L, 0x4FDFF252L,
	0xD1BB67F1L, 0xA6BC5767L, 0x3FB506DDL, 0x48B2364BL,
	0xD80D2BDAL, 0xAF0A1B4CL, 0x36034AF6L, 0x41047A60L,
	0xDF60EFC3L, 0xA867DF55L, 0x316E8EEFL, 0x4669BE79L,
	0xCB61B38CL, 0xBC66831AL, 0x256FD2A0L, 0x5268E236L,
	0xCC0C7795L, 0xBB0B4703L, 0x220216B9L, 0x5505262FL,
	0xC5BA3BBEL, 0xB2BD0B28L, 0x2BB45A92L, 0x5CB36A04L,
	0xC2D7FFA7L, 0xB5D0CF31L, 0x2CD99E8BL, 0x5BDEAE1DL,
	0x9B64C2B0L, 0xEC63F226L, 0x756AA39CL, 0x026D930AL,
	0x9C0906A9L, 0xEB0E363FL, 0x72076785L, 0x05005713L,
	0x95BF4A82L, 0xE2B87A14L, 0x7BB12BAEL, 0x0CB61B38L,
	0x92D28E9BL, 0xE5D5BE0DL, 0x7CDCEFB7L, 0x0BDBDF21L,
	0x86D3D2D4L, 0xF1D4E242L, 0x68DDB3F8L, 0x1FDA836EL,
	0x81BE16CDL, 0xF6B9265BL, 0x6FB077E1L, 0x18B74777L,
	0x88085AE6L, 0xFF0F6A70L, 0x66063BCAL, 0x11010B5CL,
	0x8F659EFFL, 0xF862AE69L, 0x616BFFD3L, 0x166CCF45L,
	0xA00AE278L, 0xD70DD2EEL, 0x4E048354L, 0x3903B3C2L,
	0xA7672661L, 0xD06016F7L, 0x4969474DL, 0x3E6E77DBL,
	0xAED16A4AL, 0xD9D65ADCL, 0x40DF0B66L, 0x37D83BF0L,
	0xA9BCAE53L, 0xDEBB9EC5L, 0x47B2CF7FL, 0x30B5FFE9L,
	0xBDBDF21CL, 0xCABAC28AL, 0x53B39330L, 0x24B4A3A6L,
	0xBAD03605L, 0xCDD70693L, 0x54DE5729L, 0x23D967BFL,
	0xB3667A2EL, 0xC4614AB8L, 0x5D681B02L, 0x2A6F2B94L,
	0xB40BBE37L, 0xC30C8EA1L, 0x5A05DF1BL, 0x2D02EF8DL
};


static void InitCrc16_CcitFalse(CRC_Type *base, uint32_t seed)
{
    crc_config_t config;

    CRC_GetDefaultConfig(&config);
    config.seed = seed;
    CRC_Init(base, &config);
}


static void InitCrc32(CRC_Type *base, uint32_t seed)
{
    crc_config_t config;

    config.polynomial = 0x04C11DB7U;
    config.seed = seed;
    config.reflectIn = true;
    config.reflectOut = true;
    config.complementChecksum = true;
    config.crcBits = kCrcBits32;
    config.crcResult = kCrcFinalChecksum;

    CRC_Init(base, &config);
}

static uint16_t Crc16HW(void *data, size_t size){
    static CRC_Type *base = CRC0;
    InitCrc16_CcitFalse(base, 0xFFFF);
    CRC_WriteData(base, (uint8_t *)data, size);
    return CRC_Get16bitResult(base);
}

static uint32_t Crc32HW(void *data, size_t size){
    static CRC_Type *base = CRC0;
    InitCrc32(base, 0xFFFFFFFF);
    CRC_WriteData(base, (uint8_t *)data, size);
    return CRC_Get32bitResult(base);
}

static uint16_t Crc16Lookup_table(void *data, size_t size){
	uint16_t result = ~0;
	uint8_t *int_data = data;

	while (size--){
		result = (result << 8) ^ crc16_ccitt_table[((result >> 8) ^ *int_data++)];
	}
	return result;
}

static uint32_t Crc32Lookup_table(void *data, size_t size){
	uint32_t result = ~0;
	uint8_t *int_data = data;

	while (size--){
		result = crc32_lookup_table[(*int_data++ ^ result) & 0xFF] ^ (result >> 8 & 0xFFFFFF) ;
	}
	return ~result;
}

 static uint16_t Crc16_ccint_polynomial(void *data, size_t size){
         uint16_t result = ~0;
         uint16_t i;
         uint8_t *int_data = data;
 
           while(size--)
           {
             result ^= *int_data++<<8;
 
             for(i = 0; i < 8; i++)
             {
               if (result&0x8000) result = (result<<1) ^ 0x1021;
               else result = result<<1 ;
             }
           }
 
           return result;
 }
 
 static uint32_t Crc32_polynomial(void *data, size_t size){
         uint32_t result = ~0;
         uint32_t i;
         uint8_t *int_data = data;
 
           while(size--)
           {
             result ^= *int_data++;
 
             for(i = 0; i < 8; i++)
             {
               if (result&1) result = (result>>1) ^ 0xEDB88320;
               else result = result>>1 ;
             }
           }
 
           return ~result;
 }


int main(void)
{
    char testData[]  = "123456789qwertyuiopasdfghjklzxcvbnm";
    char errorData[] = "122346789qwertyuiopasdfghjklzxcvbnm";
    char mutlipleErrorData[] = "122346789qwertyuiopasdfghjjlyxcvbnm";
    const uint16_t checkCcitFalseCrc16 = 0xF455;
    const uint32_t checkCrc32 = 0xEA05065E;

    uint16_t checksum16;
    uint32_t checksum32;

    /* Init hardware*/
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("Test string: %s, length: %d\r\n", testData, sizeof(testData) - 1);
    PRINTF("CRC in hardware\r\n");

    /* ***************
    * CRC-16/CCIT-FALSE *
    *************** */
    checksum16 = Crc16HW(testData, sizeof(testData) - 1);
    PRINTF("\tCRC-16 CCIT FALSE: 0x%x\t", checksum16);
    if (checksum16 != checkCcitFalseCrc16) PRINTF("Fail. Expected: 0x%x\r\n", checkCcitFalseCrc16);
    else PRINTF("Success\r\n");

    /* ***************
    * CRC-32 *
    *************** */
    checksum32 = Crc32HW(testData, sizeof(testData) - 1);
    PRINTF("\tCRC-32: 0x%x\t\t", checksum32);
    if (checksum32 != checkCrc32) PRINTF("Fail. Expected: 0x%x\r\n", checkCrc32);
    else PRINTF("Success\r\n");


    /*******************
     * CRC16 - lookup_table
     *******************/
    checksum16 = Crc16Lookup_table(testData, sizeof(testData) - 1);
    PRINTF("Calculating CRC using lookup table\r\n");
    PRINTF("\tCRC-16 CCIT FALSE: 0x%x\t", checksum16);
    if (checksum16 != checkCcitFalseCrc16) PRINTF("Fail. Expected: 0x%x\r\n", checkCcitFalseCrc16);
    else PRINTF("Success\r\n");

    /*******************
     * CRC32 - lookup_table
     *******************/
    checksum32 = Crc32Lookup_table(testData, sizeof(testData) - 1);
    PRINTF("\tCRC-32: 0x%x\t\t", checksum32);
    if (checksum32 != checkCrc32) PRINTF("Fail. Expected: 0x%x\r\n", checkCrc32);
    else PRINTF("Success\r\n");


    /*******************
     * CRC16 - polynomial
     *******************/
    PRINTF("Calculating CRC with polynomial\r\n");
    checksum16 = Crc16_ccint_polynomial(testData, sizeof(testData) - 1);
    PRINTF("\tCRC-16 CCIT FALSE: 0x%x\t", checksum16);
    if (checksum16 != checkCcitFalseCrc16) PRINTF("Fail. Expected: 0x%x\r\n", checkCcitFalseCrc16);
    else PRINTF("Success\r\n");

    /*******************
     * CRC32 - polynomial
     *******************/
    checksum32 = Crc32_polynomial(testData, sizeof(testData) - 1);
    PRINTF("\tCRC-32: 0x%x\t\t", checksum32);
    if (checksum32 != checkCrc32) PRINTF("Fail. Expected: 0x%x\r\n", checkCrc32);
    else PRINTF("Success\r\n");



    /*****************************************
     * Error in data tests
     * This is only single error
     *****************************************/

    PRINTF("\r\n\r\nError in data check\r\n");
    PRINTF("Test string: %s, length: %d\r\n", errorData, sizeof(errorData) - 1);

    /* ***************
    * CRC-16/CCIT-FALSE *
    *************** */
    checksum16 = Crc16HW(errorData, sizeof(errorData) - 1);
    PRINTF("\tCRC-16 CCIT FALSE: 0x%x\t", checksum16);
    if (checksum16 != checkCcitFalseCrc16) PRINTF("Fail. Expected: 0x%x\r\n", checkCcitFalseCrc16);
    else PRINTF("Success\r\n");

    /* ***************
    * CRC-32
    *************** */
    checksum32 = Crc32HW(errorData, sizeof(errorData) -1);
    PRINTF("\tCRC-32: 0x%x\t\t", checksum32);
    if (checksum32 != checkCrc32) PRINTF("Fail. Expected: 0x%x\r\n", checkCrc32);
    else PRINTF("Success\r\n");


    /*******************
     * CRC16 - lookup_table
     *******************/
    PRINTF("Calculating CRC using lookup table\r\n");
    checksum16 = Crc16Lookup_table(errorData, sizeof(errorData) - 1);
    PRINTF("\tCRC-16 CCIT FALSE: 0x%x\t", checksum16);
    if (checksum16 != checkCcitFalseCrc16) PRINTF("Fail. Expected: 0x%x\r\n", checkCcitFalseCrc16);
    else PRINTF("Success\r\n");

    /*******************
     * CRC32 - lookup_table
     *******************/
    checksum32 = Crc32Lookup_table(errorData, sizeof(errorData) - 1);
    PRINTF("\tCRC-32: 0x%x\t\t", checksum32);
    if (checksum32 != checkCrc32) PRINTF("Fail. Expected: 0x%x\r\n", checkCrc32);
    else PRINTF("Success\r\n");

    /*******************
     * CRC16 - polynomial
     *******************/
    PRINTF("Calculating CRC with polynomial\r\n");
    checksum16 = Crc16_ccint_polynomial(errorData, sizeof(errorData) - 1);
    PRINTF("\tCRC-16 CCIT FALSE: 0x%x\t", checksum16);
    if (checksum16 != checkCcitFalseCrc16) PRINTF("Fail. Expected: 0x%x\r\n", checkCcitFalseCrc16);
    else PRINTF("Success\r\n");

    /*******************
     * CRC32 - polynomial
     *******************/
    checksum32 = Crc32_polynomial(errorData, sizeof(errorData) - 1);
    PRINTF("\tCRC-32: 0x%x\t\t", checksum32);
    if (checksum32 != checkCrc32) PRINTF("Fail. Expected: 0x%x\r\n", checkCrc32);
    else PRINTF("Success\r\n");
    



    /*****************************************
     * Error in data tests
     * This is multiple error data
     *****************************************/

    PRINTF("\r\n\r\nMultiple error in data check\r\n");
    PRINTF("Test string: %s, length: %d\r\n", mutlipleErrorData, sizeof(mutlipleErrorData) - 1);

    /* ***************
    * CRC-16/CCIT-FALSE *
    *************** */
    checksum16 = Crc16HW(mutlipleErrorData, sizeof(mutlipleErrorData) - 1);
    PRINTF("\tCRC-16 CCIT FALSE: 0x%x\t", checksum16);
    if (checksum16 != checkCcitFalseCrc16) PRINTF("Fail. Expected: 0x%x\r\n", checkCcitFalseCrc16);
    else PRINTF("Success\r\n");

    /* ***************
    * CRC-32
    *************** */
    checksum32 = Crc32HW(mutlipleErrorData, sizeof(mutlipleErrorData) - 1);
    PRINTF("\tCRC-32: 0x%x\t\t", checksum32);
    if (checksum32 != checkCrc32) PRINTF("Fail. Expected: 0x%x\r\n", checkCrc32);
    else PRINTF("Success\r\n");


    /*******************
     * CRC16 - lookup_table
     *******************/
    PRINTF("Calculating CRC using lookup table\r\n");
    checksum16 = Crc16Lookup_table(mutlipleErrorData, sizeof(mutlipleErrorData) - 1);
    PRINTF("\tCRC-16 CCIT FALSE: 0x%x\t", checksum16);
    if (checksum16 != checkCcitFalseCrc16) PRINTF("Fail. Expected: 0x%x\r\n", checkCcitFalseCrc16);
    else PRINTF("Success\r\n");

    /*******************
     * CRC32 - lookup_table
     *******************/
    checksum32 = Crc32Lookup_table(mutlipleErrorData, sizeof(mutlipleErrorData) - 1);
    PRINTF("\tCRC-32: 0x%x\t\t", checksum32);
    if (checksum32 != checkCrc32) PRINTF("Fail. Expected: 0x%x\r\n", checkCrc32);
    else PRINTF("Success\r\n");


    /*******************
     * CRC16 - polynomial
     *******************/
    PRINTF("Calculating CRC with polynomial\r\n");
    checksum16 = Crc16_ccint_polynomial(mutlipleErrorData, sizeof(mutlipleErrorData) - 1);
    PRINTF("\tCRC-16 CCIT FALSE: 0x%x\t", checksum16);
    if (checksum16 != checkCcitFalseCrc16) PRINTF("Fail. Expected: 0x%x\r\n", checkCcitFalseCrc16);
    else PRINTF("Success\r\n");

    /*******************
     * CRC32 - polynomial
     *******************/
    checksum32 = Crc32_polynomial(mutlipleErrorData, sizeof(mutlipleErrorData) - 1);
    PRINTF("\tCRC-32: 0x%x\t\t", checksum32);
    if (checksum32 != checkCrc32) PRINTF("Fail. Expected: 0x%x\r\n", checkCrc32);
    else PRINTF("Success\r\n");
    
    

    PRINTF("The END... \r\n", checksum32);
    return 0;
}
