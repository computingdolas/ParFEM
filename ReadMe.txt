# This is finite element program for Smallest eigenvalue

/********************   Starting the Program ***********/

Step 1: Type <./simulation> or type <make> for program to build . Once it is successfully build without any warnings or errors , executable will be produced with the name <waveguide> 

Step 2: Start the program by typing in <./waveguide> <delta> <epsilon> <refinement level> <filename containing geometry> ....even if you don't specify last two parameters , the program will take default parameters appropriately, but program will abort upon not mentioning important parameters 

Step 3: Once the program is started , output will be printed on screen as program goes from one module to another ....so you can verify the whether input parameters are same as taken by program to execute following FEM problem 

Step 4: After the program is completed , <A.txt> <M.txt> <eigenmode.txt> <lamda.txt> <ksq.txt> will be generated  ans as well lamba/iterations

step 5: you can compare the reference output with program generated output 


/*******************  Word of Caution ******************/  

Please don't enter negative entries...and illogical entries ...as program output will change drastically..please confirm your input parameters before entering in the command line
along with executable  
