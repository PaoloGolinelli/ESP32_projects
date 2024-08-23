/*--------------------------------------------------------------------
/   Touch sensor with Auto Calibration
/ --------------------------------------------------------------------
/   Schematic
/   Use one integrated touch sensor from your ESP32. Connect a cable 
/   of whatever length to the touch pin. Connect an LED to LED_PIN: 
/   it will light when a press is detected.
/   
/   Usage:
/   Start touching the cable (also the non exposed part) alternatively
/   It might need some time to calibrate the pressing status to get 
/   to correctly detect when you press the cable.
/
/   Tweaking parameters:
/   Increase the CAL_SAMPLE if you dont need to calibrate often.
/   This will increment the time between calibrations. You might need
/   to tweak CAL_TRSHLD to allow to calibrate when only a couple of
/   short presses have been registered
/ --------------------------------------------------------------------*/

/* available pins:
/   pin     alias   status
/-------------------------------
/   GPIO04  T0      working
/   GPIO00  T1      not working
/   GPIO02  T2      not working
/   GPIO15  T3      working
/   GPIO13  T4      working
/   GPIO12  T5      not checked
/   GPIO14  T6      not checked
/   GPIO27  T7      not checked
/   GPIO33  T8      not checked 
/   GPIO32  T9      not checked 
*/

#include <math.h>

#define LED_PIN     32      // output LED pin

// calibration
#define CAL_SAMPLE  32      // number of samples taken before calibration
#define CAL_TRSHLD  3       // min difference required from avg to previous high and low val to allow for calibration

// calibration support variables
int cal_arr[CAL_SAMPLE] = {0};  // array holding the last CAL_SAMPLE readings
int cal_idx     = 0;            // loops from 0 to CAL_SAMPLE. After calibration starts back again
double high_val = 92;           // value corresponding to untouched (not pressed) sensor
double low_val  = 24;           // value corresponding to touched (pressed) sensor


void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, read_touch_sensor(T0));

  delay(100);
}

bool read_touch_sensor(int pin) {
  int     reading = touchRead(pin); // read analog value from integrated touch sensor 
  double  avg;                      // avg of the last CAL_SAMPLE samples
  double  low_avg, high_avg;        // avgs of the values below&above avg
  double  low_idx, high_idx;        // number of samples below&above avg

  // save value in buffer
  cal_arr[cal_idx] = reading;
  cal_idx++;

  // calibration procedure
  if (cal_idx >= CAL_SAMPLE) {
    
    // compute avg
    avg = 0;
    for (int i = 0; i < CAL_SAMPLE; i++) {  
      avg += cal_arr[i];
    }
    avg /= CAL_SAMPLE;

    // calibrate if difference is enough (if both samples of pressed and unpressed are taken)
    if (fabs(avg - high_val) > CAL_TRSHLD && fabs(avg - low_val) > CAL_TRSHLD) {
      low_avg = 0; high_avg = 0;
      low_idx = 0; high_idx = 0;

      // compute avg of samples below&above avg
      for (int j = 0; j < CAL_SAMPLE; j++) {
        if (cal_arr[j] > avg) {     // above
          high_avg += cal_arr[j];
          high_idx += 1;
        } else {                    // below
          low_avg += cal_arr[j];
          low_idx += 1;
        }
      }

      // Sets the new values for analog readings of pressed and non-pressed
      high_val = high_avg/high_idx;
      low_val  = low_avg/low_idx;
      Serial.printf("Hv = %.1f, Lv = %.1f\n", high_val, low_val);
    }

    cal_idx = 0;  // loops starts back at 0
  }

  Serial.printf("i=%d, GPIO%d = %d, state = ", cal_idx, pin, reading);

  // returns T/F if the analog reading is below/above the avg between a pressed and an unpressed
  if (reading < (high_val+low_val)/2.0) {
    Serial.println("pressed");
    return true;
  } else {
    Serial.println("not pressed");
    return false;
  }
}