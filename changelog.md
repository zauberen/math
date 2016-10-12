###a0.1: Changes / Additions:
- Now compilable (although unstable)
- Able to do single-operation math
    - Needs some more work on the doMath function set
- Able to store variables (not to disk yet)
- Finished a help function
- Added a DEBUG variable (on by default in the MASTER repo, var is contained in the math header)
- Finished the doMath() function (assuming it works properly, it is in line with the v1 milestone)
- Finished the getter and setter functions for variables (for now, not in line with v1 milestone)

###Other notes:

- A release branch will be released with version 0.5 (which will be 'beta'), which will house the release source.
- The beta release has these basic requirements:
- Math functions work
- Variable functions work
- The file structure of the repo will be redone with this release as well.
    - There will be a src/ folder which will hold the source code
    - There will be a bin/ folder which will hold a compiled version of the latest compilable committed code (of that repo)
- So, the repo should look like this after the rework:
src/
    main.cpp
    math.cpp
    math.h
bin/
    math.o
    math.exe (on MASTER this file will likely be old, as development is done on linux)
README.md
changelog.md
LICENSE
(end)
Also, this project is being licensed under the GPLv3 as of THIS commit.
