CS 283 – Assignment 2: Tiny Computer Database
Author: Niyam Acharya

Description:
------------
This project implements a simple database of computer records using a fixed-size binary format. 
The database is stored in a single file (ccdb) and accessed through multiple programs:

Files:
------
- ccadd: Add a record (with auto-ID support using -a)
- ccitem: Show one record by ID
- cclist: List all records
- ccdel: Delete a record by ID
- ccmatch: Search all text fields for a substring
- ccyear: List records in a year range
- ccedit: Edit a record field-by-field

How to Build:
-------------
Run:
    make

This will compile all programs.

How to Clean:
-------------
Run:
    make clean

This will remove all compiled binaries.

Usage Examples:
---------------
Add with ID:
    ./ccadd 2 MacBook 2021 Apple M1 16 "Fast and light"

Add with auto ID:
    ./ccadd -a MacBook 2021 Apple M1 16 "Auto assigned"

Show a record:
    ./ccitem 2

Delete a record:
    ./ccdel 2

Match string:
    ./ccmatch "Apple"

Filter by year:
    ./ccyear 2020 2023

Edit a record:
    ./ccedit 2

List all:
    ./cclist

Locking:
--------
All programs use flock() for safe access to ccdb in concurrent environments.

ID Policy:
----------
IDs are not overwritten in ccadd; duplicate manual IDs are rejected.
