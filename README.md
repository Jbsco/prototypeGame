# prototypeGame
An SDL2 project for a Video Game Design course, simulating 2D FPV drone flight - includes physics model, HUD elements, and dynamic screen behavior as well as rudimentary "course" generation
###### This repo is a copy of my individual efforts in what was a team-based project. Note that this repository doesn't include any of the development history, commit history, or anything of that nature - It is only a snapshot of the most recent master branch just before last-minute team contributions were merged. The entirety of my individual work on this project is presented here.

# protoGame
Contents [no longer] subject to change rapidly.

This project repo will contain source code and supporting resources, executables compiled for testing, and anything else related to development.

### Description
A 2D FPV quadcopter simulator. Simply put, navigate generated courses using the control scheme of an FPV quadcopter. Manage battery capacity and amperage limitations to finish each course in the fastest time possible. Planned support for leaderboards, different battery capacities (weight/available amperage/capacity strategy), procedural/wave-collapse course generation, different course obstacles/features, physics model, battery model, quadcopter control model, and more.

### Development Notes
Discord server invite link:
  [REDACTED]

Team:
  [REDACTED]

Please feel free to make a new branch in the repo to work on a specific feature. If the feature is functional and does not negatively impact existing master-branch features then a pull request may be submitted! If you select a [ToDO] to work on, change from [ToDo] to [In-Progress(userName)]. If multiple people are working on the same feature feel free to add muliple names to the features, ex: [In-Progress(user1)(user2)].

Helpful Git commands:
- git checkout
  - use this to access different branches
  - "git checkout master"
  - "git checkout courseGen"
- git add
  - use to add files or directories for commit
  - "git add Course.h"
  - "git add img"
- git commit -a -m 'commit comment'
  - use to commit changes or new files/folders to a branch
  - please use a pull request to submit changes to the master branch
  - "git commit -a -m '10-10-22 updated Course.h and protoGame.cpp with new course generation features for test.'
- git branch
  - use to create a new branch to work on without affecting the master branch
  - "git branch propScroll"
- git push origin
  - use to push commits to a branch
  - origin pushes to remote, i.e. GitHub
  - "git push origin propScroll"
- git branch -d
  - deletes a branch
  - "git branch -d propScroll"
- git push origin --delete
  - deletes a branch on the remote origin (GitHub etc)
  - "git push origin --delete propScroll"

If you need to reset your local repo:
  - "git fetch origin"
  - "git reset --hard origin/master"

Setting up git:
  - Recommended for Windows: install GCM [Git Credential Manager]
  - "git config --global user.name "your remote username"" [i.e. your GitHub username or other identification]
  - "git config --global user.email "email@domain.edu"" [i.e. your GitHub email for GitHub commit tracking]
  - "git remote add origin [REDACTED]" [set remote origin]
  - "git remote -v" [verify remote origin is set correctly]
  - "git pull origin master" [pull repo from remote]


### Features
- Press 'c' to display telemetry data in upper left
  - (x,y) coordinates
  - velocity
  - rotation angle (degrees)
- Press 'r' to reset to starting position with 0 velocity
- Press 'm' to change scrolling mode
  - mode 0: "test" mode, static background, no overlay, collision on window bounds, easy to visualize physics model
  - mode 1: "propScroll", default mode, proportional player sprite position, motion on grid overlay
- Press 'h' to enable HUD elements
- Press 'p' to enable course generation and timing overlay (static test scroll mode only)
  - A random number of bounding boxes are generated (red squares) and timers are initialized
    - Countdown timer will start from 10.0 seconds and count down
    - Overall timer will start from 0.0 seconds and count up
    - Moving the player sprite into a bounding box will reset the countdown timer, increase the score (under the timer) and the box will change to green
      - Green bounding boxes will not be interactable
    - Moving through all red bounding boxes will cause the timer to display the overall time, in green (score will also be green)
      - The course will regenerate (boxes, timers, score will all reset) after 5 seconds
    - Failing to interact with boxes before the countdown timer expires will cause the timer to turn red and stay at 0.0 seconds for 5 seconds before resetting again
- Take in input
  - Currently scans keypresses without waiting for event polling (faster input, no repeat delay, smoother input update rate)
  - If joystick is connected, stick input takes priority and keyboard input is disabled
    - Tested with Team Blacksheep Tango II, throttle: left stick y-axis, pitch: right stick y-axis
    - Time constant implemented with joystick input, rates are consistent [ToDo] fine-tuning
- Model physics
  - Currently models gravity, drone flight, roll (using trig fcns), velocity increase in current upward direction
    - Max velocity is 1000 (internal, HUD will show 100), in any direction (magnitude calculation)
    - If max is exceeded, apply proportional reduction to correct maximum magnitude to 700
  - [ToDo]: model roll using matrix rotations instead of trig fcns
- Create UI/graphics resources
  - Currently player drone sprite, HUD elements, and debug telemetry are modeled
    - HUD shows current angle direction, current velocity direction, and velocity magnitude (press 'h')
  - [ToDo] player drone sprite could be improved, need background, more and/or improved UI elements
- Generate courses
  - [ToDo] currently minimal, create any course elements, place randomly or via seed generation, wave collapse, etc
  - Scrolling currently implemented, proportional, horizontal and vertical
    - scrolling should be dynamic and proportional to velocity in the respective direction
    - when velocity is lower, player position should be more flexible and allow for ranging around the screen
      - ex. when velocity is low or hasn't been high for some specific time period, screen doesn't scroll and player position can be anywhere on the screen
    - when velocity is higher, player position should progressively be nearer to the receding edge of the screen, i.e. more relative
      - this should provide for more time in navigating obstacles at greater speed
      - ex. after maintaining high velocity for a specific time period, scrolling starts and gradually picks up speed proportional to velocity and time
        - relative player position should also be more constrained in this case
    - input and physics model should be unchanged, only relative motion should be modified during scrolling behavior
    - motion trail functional
    - [ToDo] blend over to vaporwave style colors/textures, fade background to black, when speed and throttle have both been maximum for ~5sec
      - "LEDs" on player sprite
      - cyan/pink grid overlay
      - other effects
      - fade back to normal after impact or letting off throttle
      - allow option to toggle vaporwave theme without timer
      - secret blackwatch plaid mode (as a joke)
        - use blackwatch plaid for grid overlay

### Mechanics
  - Physics
    - [ToDo] Model LiPo batteries of different capacities/weights
    - Model craft thrust/weight models
      - Currently baseline model
      - [ToDo] create additional models, options for roll rate, etc
    - [ToDo] Provide for setting degrees per second of rotation preference
    - Model acceleration due to gravity
      - Currently functional, could be fine-tuned for accuracy
    - Model acceleration due to thrust
      - Currently baseline functional, could be tuned for accuracy
    - Model impact with environment/objects
      - Currently baseline functional on window bounds
      - [ToDo] model impact with course elements
  - [ToDo] Interface
    - Implement generative course designs
    - Different course elements
      - Obstacles
      - Gates
      - Loops (display # of required loops etc)
      - Power-ups
        - Time-dependent battery % recharge
        - Proportional to battery remaining
          - Ex. running out of battery but momentum carries you to battery recharge = max benefit
    - Responsive UI
      - Respond to input events even as UI is animating
      - Allow user to make inputs as fast as they are able
      - Never miss an input

### Details
  - Audio
    - [ToDo] Simple sine frequency for motor pitch
    - [ToDo] Model all 4 motors to simulate motor RPM and prop blade audio
    - BGM implemented
    - UI sounds, etc
      - Currently boundary impact SFX, gate SFX, timer SFX, reset SFX are functional
      - [ToDo(minimal)] any other SFX/UI sounds
