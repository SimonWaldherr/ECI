
#include <math.h>
#include <ctype.h>
#include "sgp4Wrapper.h"

/* Declare variables with global scope so ECI can access them */
/* Inputs */
tle_lines_t tle_lines;
extern ECI_TimeStamp_t SL_Step_TimeStamp;

/* Internal */
TLE tle;
double mins = 0;
bool checksum1Passed = TRUE;
bool checksum2Passed = TRUE;

/* Outputs */
propState_t propState;

/* The SGP4 implementation used in this example is not suitable
 * for implementation via ECI because the top-level main function
 * in the implementation uses unavailable APIs (file reading) during
 * the call stack. Here we define a wrapper function to call the 
 * specific functionality needed for the propagation as a step 
 * function for ECI to call.
 */
void propagate(void){

    /* Get the current time from FSW */
    mins = ((double)SL_Step_TimeStamp.Seconds + ((double)
        SL_Step_TimeStamp.Subseconds/4294967296.0));
    
    /* This doesn't need to be re-processed each cycle, only 
     * when the TLE table is updated, but we but need some way 
     * to determine the table has changed in order to not 
     * reprocess every cycle.
     */
    parseLines(&tle,tle_lines.line1,tle_lines.line2);

    /* Do the propagation */
    getRV(&tle,mins,propState.r,propState.v);
    
    /* The results are stored in outData which ECI will emit
     * onto the software bus for downstream use.
     */

}

/* Begin implementation of table validation */

int digitToInt(char digit){
/* Convert a character containing a digit to the numerical
 * equivalent. 
 *
 * Note: This return incorrect results if the character 
 *     to convert is not a digit. Call isdigit() to check
 *     input before calling digitToInt()!
 */
    
    /* C gaurantees that digits 0-9 are stored contiguously
     * so we can subtract the character '0' to convert a
     * character to int*/
    return digit - '0';
}

int computeTLEChecksum(char line[70]){
/* Computes a checksum for a TLE line. 

 * Details of the algorithm are included in comments 
 * below and were implemented from the algorithm 
 * description found here:
 * http://www.stltracker.com/resources/tle
 */    
    
    /* set checksum to 0 */
    int checksum = 0;

    /* for character from 1 to 68 */
    for (int i = 0; i < 68; i++){
        
      /* if i'th character in the line is a digit */
      if (isdigit(line[i])){
        /* add value of i'th character in the line to checksum*/
        checksum += digitToInt(line[i]);
      }
      /* else if i'th character in the line is '-' */
      else if (line[i] == '-'){
        /* add 1 to checksum */
        checksum++;
      }
    }

    /* set checksum to the value of the checksum modulo 10 */
    return checksum % 10;
}

bool validateTLEChecksum(char line[70]){
/* Computes a TLE checksum and returns if the computed checksum 
 * matches the one given in the TLE. */
    
    /* checksum is stored in last character/digit in the TLE line */
    return computeTLEChecksum(line) == digitToInt(line[68]);
}

int32_t validateTLE(tle_lines_t * TblPtr){
/* Validates that each of the TLE lines match their built-in 
 * checksums.
 *
 * Note: Table validation function must have the following signature:
        int32 validateFcn(void * TblPtr);
 * and return a negative value if the table should fail validation.
 */
 
    /* Each line is validated independently */
    checksum1Passed = validateTLEChecksum(TblPtr->line1);
    checksum2Passed = validateTLEChecksum(TblPtr->line2);

    /* Additional validation of the format of the TLE could also be
     * done, but is not relevant to this example.
     */
        
    /* Each passed flag is a boolean value which is cast to either 
     * 0 (if the checksum validated) or 1 (if it didn't). By 
     * subtracting 1 from each flag, we convert each value to a 
     * negative value (if it failed) or zero if it passed. By adding
     * the two values, we get:
     *       0 if both passed 
     *      -1 if one line failed
     *      -2 if both lines failed
     * which meets the requirement of a negative return value to 
     * fail the table.
     */
    return ((int) checksum1Passed - 1) + ((int) checksum1Passed - 1);
}