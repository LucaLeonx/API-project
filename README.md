# API-project 2022-2023
Project relative to the PoliMi course "Algorithms and Computer Science Principles".
The project aims to resolve the problem described in details [here](https://github.com/LucaLeonx/API-project/tree/main/assets/2022_2023.pdf).

The project is entirly devolped in C and all the data structures were created from scratch.

The basic idea was to implement a time and space-efficient algorithm to calculate the fastest path based on inputs.

One easy example is this: 

![](/assets/path.png)

here the correct path from 10 to 60 is:
- 10->20->40->60

while the following paths are wrong
- 10->30->40->60 because 30 > 20
- 10->20->50->60 because 50 > 40

