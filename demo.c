// String input;
// DS3231 rtc(SDA, SCL);

// const int DIGIT_TO_SEGMENT_MAPPING[10][7] = {
//     {1, 1, 1, 1, 1, 1, 0}, // 0
//     {0, 1, 1, 0, 0, 0, 0}, // 1
//     {1, 1, 0, 1, 1, 0, 1}, // 2
//     {1, 1, 1, 1, 0, 0, 1}, // 3
//     {0, 1, 1, 0, 0, 1, 1}, // 4
//     {1, 0, 1, 1, 0, 1, 1}, // 5
//     {1, 0, 1, 1, 1, 1, 1}, // 6
//     {1, 1, 1, 0, 0, 0, 0}, // 7
//     {1, 1, 1, 1, 1, 1, 1}, // 8
//     {1, 1, 1, 1, 0, 1, 1}  // 9
// };

// const int SEGMENT_INTERVALS[4][7][2] = {
//     {{141, 54},
//      {155, 69},
//      {150, 73},
//      {151, 70},
//      {159, 75},
//      {159, 75},
//      {125, 40}},
//     {{164, 76},
//      {155, 76},
//      {138, 61},
//      {180, 87},
//      {151, 63},
//      {145, 57},
//      {165, 78}},
//     {{157, 73},
//      {156, 70},
//      {165, 85},
//      {137, 52},
//      {133, 50},
//      {133, 50},
//      {168, 73}},
//     {{131, 52},
//      {147, 61},
//      {131, 51},
//      {158, 69},
//      {155, 73},
//      {116, 28},
//      {137, 60}}};

// const int COLON_INTERVAL[2][2] = {
//     {141, 62},
//     {137, 30},
// };

// const int DIGIT_STARTING_SEGMENT_INDEX[4] = {2, 9, 22, 29};
// const int COLON_STARTING_INDEX = 16;

// const int START_POS = 0;
// const int COLON = 2;
// const int DIGITS = 4;
// const int SEGMENTS_PER_DIGIT = 7;
// const int STEP_MS = 20;
// const int COUNT_MS = 2000;
// const int NUM_SERVOS = DIGITS * SEGMENTS_PER_DIGIT;

// int servoTargetDestination[DIGITS][NUM_SERVOS];
// int servoTargetDestinationColon[COLON];

// int count = 1200;
// int timeMS = 0;

// Servo servos[DIGITS][SEGMENTS_PER_DIGIT];
// Servo colonServos[COLON];

// void setup()
// {
//     rtc.begin();
//     for (int i = 0; i < DIGITS; i++)
//     {
//         for (int j = 0; j < SEGMENTS_PER_DIGIT; j++)
//         {
//             int offset = DIGIT_STARTING_SEGMENT_INDEX[i];
//             servoTargetDestination[i][j] = SEGMENT_INTERVALS[i][j][START_POS];
//             servos[i][j].attach(j + offset);
//             servos[i][j].write(servoTargetDestination[i][j]);
//         }
//     }
//     delay(500);
//     for (int i = 0; i < DIGITS; i++)
//     {

//         for (int j = 0; j < SEGMENTS_PER_DIGIT; j++)
//         {
//             servos[i][j].detach();
//         }
//     }
//     for (int i = 0; i < COLON; i++)
//     {
//         colonServos[i].attach(i + COLON_STARTING_INDEX);
//         colonServos[i].write(COLON_INTERVAL[i][START_POS]);
//     }
//     delay(500);
//     for (int i = 0; i < COLON; i++)
//     {
//         colonServos[i].detach();
//     }
//     for (int i = 0; i < COLON; i++)
//     {
//         colonServos[i].attach(i + COLON_STARTING_INDEX);
//         colonServos[i].write(COLON_INTERVAL[i][1]);
//     }
//     delay(500);
//     for (int i = 0; i < COLON; i++)
//     {
//         colonServos[i].detach();
//     }
// }

// void loop()
// {
//     // Retrieve and cleanup RTC string. "12:45" -> "1245"
//     String timeStr = rtc.getTimeStr();
//     String timeString = timeStr.substring(0, 2) + timeStr.substring(3, 5);

//     for (int activeDigit = 0; activeDigit < 4; activeDigit++)
//     {

//         // Step 1: Set servoTargetDestination
//         String stringCount = String(count); // Swap to timeString to use the clock.
//         for (int i = 0; i < SEGMENTS_PER_DIGIT; i++)
//         {
//             int displayNumber = stringCount.charAt(timeString.length() - 1 - activeDigit) - '0';
//             int placement = (activeDigit == 3 && displayNumber == 0) ? 0 : DIGIT_TO_SEGMENT_MAPPING[displayNumber][i];
//             servoTargetDestination[activeDigit][i] = SEGMENT_INTERVALS[activeDigit][i][placement];
//         }

//         // Step 2: Increment Segments
//         for (int i = 0; i < SEGMENTS_PER_DIGIT; i++)
//         {
//             Servo servo = servos[activeDigit][i];
//             int pos = servo.read();
//             int dest = servoTargetDestination[activeDigit][i];
//             if (pos != dest)
//             {
//                 if (pos < dest)
//                 {
//                     pos++;
//                 }
//                 else
//                 {
//                     pos--;
//                 }
//                 if (!servo.attached())
//                 {
//                     int offset = DIGIT_STARTING_SEGMENT_INDEX[activeDigit];
//                     servo.attach(i + offset);
//                 }
//                 servo.write(pos);
//             }
//         }
//     }

//     // Step 3: Wait
//     delay(STEP_MS);
//     timeMS = timeMS + STEP_MS;

//     // Step 4A: Countdown
//     if (timeMS >= COUNT_MS)
//     {
//         timeMS = 0;
//         count = count + 1;
//     }

//     // Step 5: Detach anything that is at its destination
//     for (int i = 0; i < DIGITS; i++)
//     {
//         for (int j = 0; j < NUM_SERVOS; j++)
//         {
//             Servo servo = servos[i][j];
//             int pos = servo.read();
//             int destination = servoTargetDestination[i][j];
//             if (pos == destination && servo.attached())
//             {
//                 servos[i][j].detach();
//             }
//         }
//     }
// }