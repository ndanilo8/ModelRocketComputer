#ifndef KALMAN
#define KALMAN

/*
   EKF (Extended Kalman Filter) for altitude estimation

   Copyright (C) 2021 Danilo Nascimento
   GNU General Public License v3.0
   Permissions of this strong copyleft license are conditioned on making available complete source code of licensed works and modifications,
   which include larger works using a licensed work, under the same license.
   Copyright and license notices must be preserved. Contributors provide an express grant of patent rights.

   Contact information
   -------------------
   E-mail: ndanilo8@hotmail.com
   Website: daniloonspace.com
*/

/*DISCLAMER 
This Kalman filter is only for Altitude and Vertical Velocity estimation. (add more states according to your application)
No Orientation (attitute) Estimation

Work is based on Adam's work https://github.com/AdamMarciniak/Kalman-Filter-Tutorial-Video
 */


#include <config.h>
#include <global.h>
#include <data.h>

class EKF
{

public:
    EKF(){};
    void initkalman();
    void predict(float accel);
    void updateBaro(float altitude);
    //void updateGNSS();

    float getKalmanPosition();
    float getKalmanVelocity();
    float getKalmanGain();



private:
    unsigned long currentTime = 0;
    unsigned long prevTime = 0;
    float delT = 0.0f;

    bool isFirstStep = true;
};

#endif /* KALMAN */
