LMS - Learning Management System
================================

A C++ based Learning Management System using Qt for the graphical user interface.

Features
--------
- Multi-role support: Admin, Teacher, and Student.
- Admin Dashboard:
    - Manage Users (Create/Edit/Delete Students and Teachers).
    - Manage Courses (Create/Delete Online and Onsite courses).
    - Assign Teachers to Courses.
- Teacher Dashboard:
    - View assigned courses.
    - Create/Remove Assignments.
    - View Submissions and Grade students.
- Student Dashboard:
    - View enrolled courses.
    - View available courses.
    - Enroll/Drop courses.
    - Submit assignments.
- Data Persistence: All data is saved to text files (students.txt, teachers.txt, courses.txt, etc.).

Prerequisites
-------------
- C++ Compiler (GCC, Clang, or MSVC)
- CMake (3.16 or higher)
- Qt 5 or Qt 6

Build Instructions
------------------
1. Create a build directory:
   mkdir build
   cd build

2. Generate project files using CMake:
   cmake ..

3. Build the project:
   cmake --build .

Usage
-----
Run the executable generated in the build directory.
   ./ConsoleApplication1

Login Credentials
-----------------
(See admins.txt, teachers.txt, students.txt for existing users or create new ones via Admin Dashboard)

Admin:
Username: admin
Password: admin123

Or:
Username: Ahmad
Password: admin123

Notes
-----
- The application uses local text files for database storage. Ensure the executable has write permissions to the working directory.
- The UI is styled with a custom dark theme for better aesthetics.
