/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       williamtan                                                */
/*    Created:      1/23/2025, 7:13:19 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

using namespace std;

#include "vex.h"

using namespace vex;

brain Brain;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

controller Controller1 = controller(primary);

// TODO: fill in drivetrain motors w nobel code


digital_out Clamp = digital_out(Brain.ThreeWirePort.A);
motor IntakeRoller = motor(PORT7, ratio18_1, false);
motor IntakeChain = motor(PORT8, ratio6_1, false);
motor LadyBrown = motor(PORT9, ratio18_1, false);

bool clampActivated = false;
int intakeIdx = 0;
int ladyBrownIdx = 0;
int ladyBrownAngles[3] = {0, 180, 720};


void pre_auton(void) {
  LadyBrown.setVelocity(100.0, percent);
  IntakeRoller.setVelocity(100.0, percent);
  IntakeChain.setVelocity(100.0, percent);
  LadyBrown.setPosition(0, degrees);
}

void autonomous(void) {
  // Cases
  // Left + / Right -
  // Right + / Left -
}

void unarmLadyBrown() {
  LadyBrown.spinToPosition(ladyBrownAngles[0], degrees, false);
  LadyBrown.setStopping(brake);
}

void armLadyBrown() {
  // Places lady brown in arm position
  LadyBrown.setStopping(hold);
  LadyBrown.spinToPosition(ladyBrownAngles[1], degrees, false);
}

void scoreLadyBrown() {
  LadyBrown.spinToPosition(ladyBrownAngles[2], degrees, false);
  setIntake(0);
}

void clampToggle() {
  clampActivated = !clampActivated;
  Clamp.set(clampActivated);
}

void setIntake(int value, bool toggle = false) {
  if(toggle && value == intakeIdx) {
    value = 0; // if toggle is activated and button is pressed again, stop.
  }
  if(value == 1) {
    IntakeChain.spin(forward);
    IntakeRoller.spin(forward);
  } else if(value == 0) {
    IntakeChain.stop();
    IntakeRoller.stop();
  } else if(value == -1) {
    IntakeRoller.spin(reverse);
    IntakeRoller.spin(reverse);
  }
  intakeIdx = value;
}

void usercontrol(void) {

  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    // TODO: DRIVETRAIN 

    // LADY BROWN Macros
    Controller1.ButtonDown.pressed(unarmLadyBrown);
    Controller1.ButtonRight.pressed(armLadyBrown);
    Controller1.ButtonUp.pressed(scoreLadyBrown);
    Controller1.ButtonR1.pressed([]() {LadyBrown.spin(forward);});
    Controller1.ButtonR1.released([]() {LadyBrown.stop();});
    Controller1.ButtonR2.pressed([]() {LadyBrown.spin(reverse);});
    Controller1.ButtonR2.released([]() {LadyBrown.stop();});

    // INTAKE
    Controller1.ButtonA.pressed([]() {setIntake(1);});
    Controller1.ButtonY.pressed([]() {setIntake(-1);});

    // CLAMP 
    Controller1.ButtonB.pressed(clampToggle);


    wait(10, msec);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
