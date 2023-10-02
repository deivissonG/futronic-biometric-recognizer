#ifndef FBR_LIBRARY_H
#define FBR_LIBRARY_H

typedef int FBR_FLAG;

/**
 * Callback function to be called when the sensor is waiting for a finger or processing the fingerprint.
 * @param flag:
 *       FBR_FLAG_SUCCESS if the sensor has successfully processed the fingerprint,
 *		FBR_FLAG_WAITING_FINGER if the sensor is waiting for a finger,
 *		FBR_FLAG_PROCESSING if the sensor is processing the fingerprint.
 *
 * @return int FBR_RET_CODE_OK if the FBR should continue processing, otherwise the FBR will stop and return FBR_RET_CODE_CANCELLED_BY_USER.
 */
typedef int (*FBR_MATCH_CB)(FBR_FLAG flag);

#define FBR_RET_CODE_OK 0

#define FBR_RET_CODE_INVALID_TEMPLATE		            2
#define FBR_RET_CODE_FILESYSTEM_ERROR		            3
#define FBR_RET_CODE_INVALID_CALLBACK		            4
#define FBR_RET_CODE_BASE_TEMPLATE_NOT_SET	            5
#define FBR_RET_CODE_MATCH_TEMPLATE_NOT_SET             6
#define FBR_RET_CODE_BOZORTH_ERROR			            7
#define FBR_RET_CODE_CWSQ_ERROR				            8
#define FBR_RET_CODE_MINDTCT_ERROR			            9
#define FBR_RET_CODE_CANCELLED_BY_USER		            10
#define FBR_RET_CODE_TEMPLATE_MAXIMUM_SIZE_REACHED		11

#define FBR_FLAG_SUCCESS			0
#define FBR_FLAG_WAITING_FINGER		1
#define FBR_FLAG_PROCESSING			2
#define FBR_FLAG_MATCH				3
#define FBR_FLAG_NO_MATCH			4
#define FBR_FLAG_ERROR				5
#define FBR_FLAG_MOVABLE_FINGER		6
#define FBR_FLAG_NO_CAPTURE		7

#define MAX_TEMPLATES 10
#define MAX_SCAN_RETRIES 3

/* Initializes the library. */
extern int FBRInit();

/* Checks if the device is connected. */
extern int FBRCheckDevice();

/* Sets the base template to be compared with the fingerprint input on sensor. */
extern int FBRAddBaseTemplate(const char *);

/* Sets the base template to be compared with the fingerprint input on sensor. */
extern int FBRClearBaseTemplates(const char *);

/* Verifies if the fingerprint input on sensor match the base template. */
extern int FBRValidateLiveFingerprint(int&);

/* Extracts the fingerprint template from the sensor. */
extern int FBRExtractFingerprintTemplate(char*&);

/* Sets the callback function to be called when the sensor is waiting for a finger or processing the fingerprint. */
extern int FBRSetMatchCallback(FBR_MATCH_CB);

/* Terminate the execution and clean files */
extern int FBRTerminate();

#endif //FBR_LIBRARY_H
