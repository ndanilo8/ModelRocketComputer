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

#include "kalman.h"

#include <BasicLinearAlgebra.h>

using namespace BLA;

// Process Noise Uncertainty Matrix Q
BLA::Matrix<2, 2> Q = {0.075, 0,
                       0, 0.075};
// BLA::Matrix<2, 2> Q = {pow(data.delta_t, 4) * 0.0175, pow(data.delta_t, 3) * 0.035f,
//                        pow(data.delta_t, 3) * 0.035f, data.delta_t *data.delta_t * 0.07f};

// Measurement Uncertainty Matrix R for the Barometer
// variance= 0.07 m
BLA::Matrix<1, 1> R_Baro = {.1};

// System State Vector X
// Position
// Velocity

BLA::Matrix<2, 1> X = {0,
                       0};

// Estimate Uncertainty P
BLA::Matrix<2, 2> P = {10, 0,
                       0, 10};

// Identity Matrix I
BLA::Matrix<2, 2> I = {1, 0,
                       0, 1};

// Observation Matrix H
BLA::Matrix<1, 2> H_Baro = {1, 0};

// State Transition Matrix F
BLA::Matrix<2, 2> F;

// Output Vector
BLA::Matrix<1, 1> Z_Baro;
// Kalman Gain
BLA::Matrix<2, 1> K_Baro;

// Control Matrix / input transition matrix G / B
BLA::Matrix<2, 1> B;
// Input Variable
BLA::Matrix<1, 1> U;

void EKF::initkalman()
{
    X = {0,
         0};

    P = {0, 0,
         0, 0};
}
void EKF::predict(float accel)
{
    currentTime = micros();
    delT = (currentTime - prevTime) / 1000000.0f;
    prevTime = currentTime;

    if (!isFirstStep)
    {
        // Dynamic model of the system pos = pos_0 + v_0 * delT + (accel*delT^2)/2
        //                             vel = v_0 + accel*delT
        F = {
            1, delT,
            0, 1};

        B = {delT * delT * 0.5,
             delT};

        U = {accel};

        X = F * X + B * U;
        P = F * P * ~F + Q;
    }
    isFirstStep = false;
}

void EKF::updateBaro(float altitude)
{
    Z_Baro = {altitude};
    K_Baro = P * ~H_Baro * Inverse((H_Baro * P * ~H_Baro + R_Baro));

    X = X + K_Baro * (Z_Baro - H_Baro * X);
    P = (I - K_Baro * H_Baro) * P * (~(I - K_Baro * H_Baro)) + K_Baro * R_Baro * ~K_Baro;

    data.kalman.altitude = X(0, 0);
}

// void EKF::updateGNSS(){}

float EKF::getKalmanPosition()
{
    return X(0, 0);
}

float EKF::getKalmanVelocity()
{
    return X(1, 0);
}

float EKF::getKalmanGain()
{
    return K_Baro(1, 0);
}