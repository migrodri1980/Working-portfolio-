# ePortfolio Github Page
### by Miguel CP Rodriguez IV 

Course: CS 499- Computer Science Capstone  

### Professional Self-Assessment:

&nbsp;&nbsp;&nbsp;&nbsp;The development of this ePortfolio has showcased several strengths as well as shaped some of my  values toward a career as a Junior Developer in the Computer Science industry. At the start of the initial development of the ePortfolio,  enhancement ideas and the progression toward the implementation of those ideas pressed the value of communication and its importance. Specifically, during the development of enhancement one (cs330 milestone) toward the implementation of an additional texture,  I engaged developer and professional based communities such as [Stack Overflow](https://stackoverflow.com/questions/67607881/attempting-to-implement-a-second-texture-in-opengl){:target="_blank"} via posts for insight into this implementation. This communication coupled with research inspired me to further my understanding of many of the enhancements I was attempting to implement regarding data structures and algorithms implemented in Loginapplication.cpp. During the initial discussion of enhancement two([Loginapplication.cpp](https://github.com/migrodri1980/Working-portfolio-/blob/main/Loginapplication.cpp){:target="_blank"}), I had ideas toward implementing an array to hold user information and thus administrator functions such as creating, deleting, and updating a user. The communication and collaboration of the developer community once again led to me to research hash tables thus allowed me to understand their efficiency regarding smaller data sets as well as their limitations in larger data sets (hash collisions). This need for research, communication and collaboration furthered into enhancement three titled [LoginapplicationWSQL.cpp](https://github.com/migrodri1980/Working-portfolio-/blob/main/LoginapplicationWSQL.cpp){:target="_blank"}. Research into different database options such as SingleStore (as well as preforming test trials), allowed me to understand the differences and focuses of different database applications. Upon communication and collaboration with a few previous teachers and professional friends I was able to make an educated decision regarding the use of SQLite within my application. Communication with stakeholders was apparent in my communication with the development community regarding ideas toward enhancement implementation during my initial portfolio submission. Collaboration in a team environment came in the form of brainstorming with the development community regarding solutions toward the application of these enhancements. Upon this collaboration and further research, I was able to make decisions toward as well as create data structures such as the hash table implemented in Loginapplication.cpp to be utilized to perform the desired functionality. This value of collaboration and communication extended to LoginapplicationWSQL.cpp during the development of software engineering and databases in deciding how to implement the functionality within the application regarding customer information. Furthermore, the idea of security was present in user input validation in the form of [???if??? loop cin checks](https://raw.githubusercontent.com/migrodri1980/Working-portfolio-/main/project%20pics2/input%20validation%20check%20pic.JPG){:target="_blank"} implemented throughout Loginapplication.cpp. Security continued to play a role in decisions regarding LoginapplicationWSQL.cpp as the principle of least privilege [(link to example in code)](https://raw.githubusercontent.com/migrodri1980/Working-portfolio-/main/project%20pics2/least%20priviledge.JPG){:target="_blank"} was implemented in the placement of insert and delete database functions being limited to the administrator menu. In summary, communication with stakeholders is key to the ability to collaborating within a team environment and is a value that has been strengthened during the development of this coursework which ultimately led to the software engineering decisions regarding creation and implementation of data structures, algorithms, database functionality, and security within the given projects displayed within this ePortfolio. 


&nbsp;&nbsp;&nbsp;&nbsp;The portfolio provided is an introduction to my abilities and skills regarding computer science. Artifact one named [cs330 milestone](https://github.com/migrodri1980/Working-portfolio-/tree/main/milestone%20cs330){:target="_blank"} is meant to showcase my ability to create three dimensional objects within an OpenGL environment. This artifact is meant to show not only my ability in producing these objects, lighting, and scene but also display my current interest in art and its application in computer science. The implementation and understanding of a Vertex Array Object (VAO), Vertex Buffer Objects (VBO), and Element Buffer Objects (EBO) are visible within this application as well as the creation of fragment, light, and vertex shader source code. Artifact two named [Loginapplication.cpp](https://github.com/migrodri1980/Working-portfolio-/blob/main/Loginapplication.cpp){:target="_blank"} is meant to display my understanding of data structure creation and implementation within an application as well as also providing an example of my ability to implement algorithms to produce functionality utilizing those data structures. Furthermore, the choice of data structure as Hash Table was a decisive choice regarding the smaller data set in relation to time complexity. Lastly the final artifact named [LoginapplicationWSQL.cpp](https://github.com/migrodri1980/Working-portfolio-/blob/main/LoginapplicationWSQL.cpp){:target="_blank"} is a continuation of the previous artifact and incorporates the addition of an SQL database utilizing SQLite to serve storage of customer records. This artifact is meant to serve as an example of my understanding regarding not only the implementation of SQL databases but also my understanding of the command functions to implement SQL CRUD [(Create, Read, Update, Delete) functionality](https://raw.githubusercontent.com/migrodri1980/Working-portfolio-/main/project%20pics2/SQL%20functions%20pic.JPG){:target="_blank"} within a C++ application. The creation of a [callback function](https://raw.githubusercontent.com/migrodri1980/Working-portfolio-/main/project%20pics2/callback%20SQL.JPG){:target="_blank"} was implemented to facilitate the selection output as well as the delete functionality of the application. Furthermore the SQL functions have an ["if/else" loop](https://raw.githubusercontent.com/migrodri1980/Working-portfolio-/main/project%20pics2/SQL%20error.JPG){:target="_blank"} that checks for error and outputs to provide more expression to the user. 


&nbsp;&nbsp;&nbsp;&nbsp;This ePortfolio is meant to display not only my abilities as provided by the examples of the work within it but also to serve as a display of my understanding of the computer science field through the choices made throughout the individual artifacts. The skills and values that I have obtained and strengthened during this endeavor range from my ability to research, communicate, and address a problem with my peers as well as my persistence and determination to pursue a solution despite challenges. This can be seen in the implementation of this work provided within this ePortfolio. In conclusion, I appreciate your consideration and welcome all constructive criticsm to the projects created below. 



### Code Review for the following three projects below: 

[Code Review Link](https://youtu.be/L57Ke2MWIyc){:target="_blank"}
  
This ia a code review for the following enhancements implemented below. 
 


### Project: Milestone CS330

#### [Enhancement for Category One Link](https://github.com/migrodri1980/Working-portfolio-/blob/main/milestone%20cs330/ms330.cpp){:target="_blank"}

[Download Milestone CS330 (link to raw code)](https://raw.githubusercontent.com/migrodri1980/Working-portfolio-/main/milestone%20cs330/ms330.cpp){:target="_blank"}


[Narrative for enhancement 1 link](https://github.com/migrodri1980/Working-portfolio-/blob/f1a1cee7802af6796befa6c1b48e2429b44ac736/Enhancement%201%20Narrative-%20Milestone%20cs330.pdf){:target="_blank"}

This project incorporates my working knowledge of OpenGL.

[Picture of project rendered](https://raw.githubusercontent.com/migrodri1980/Working-portfolio-/main/Project%20Pictures/ms330%20pic.JPG){:target="_blank"}

Several key elements incorporated in this project are as follows:


*The creation of an object by creating individual points within the XYZ axis

*The utilization of light and texture to produce scene 

*The implementation of created functions to allow for user navigation around the rendered scene.

*The utilization of different functions and calls such as (Redisplay/URenderGraphics/etc)

*The manipulation and binding of VAO,VBO and EBO to produce 3d objects. 


### Project : Loginapplication.cpp

#### [Enhancement for Category Two Link](https://github.com/migrodri1980/Working-portfolio-/blob/main/Loginapplication.cpp){:target="_blank"}

[Download Loginapplication.cpp (link to raw code)](https://raw.githubusercontent.com/migrodri1980/Working-portfolio-/main/Loginapplication.cpp){:target="_blank"}


[Narrative for enhancement 2 link](https://github.com/migrodri1980/Working-portfolio-/blob/f1a1cee7802af6796befa6c1b48e2429b44ac736/Enhancement%202%20Narrative-%20Loginapplication%20cpp.pdf){:target="_blank"}

This project incorporates a working knowledge of algorithms, data structures, and security input checks

Several Key elements are incorporated in this project as follows:

*The creation of the data structure of Hash Table to facilitate user information and validation

*Within the Hash Table "for/if/else" loops and pointers are utilized to create HashTable functionality 

*Hash Table functions created (insert,remove item) and (search,print,check table). 

*The program utlizes Hash Table functionality to not only check user creditionals but 
allow for Adminstrator functionality within the program. 

*The creation of input validations checks for cin inputs by user 

*The use of nested switch cases utlizing modular resuable functions with default cases to prevent incorrect menu selection

### Project : LoginapplicationWSQL.cpp

#### [Enhancement for Category Three Link](https://github.com/migrodri1980/Working-portfolio-/blob/main/LoginapplicationWSQL.cpp){:target="_blank"}

[Download LoginapplicationWSQL.cpp (link to raw code)](https://raw.githubusercontent.com/migrodri1980/Working-portfolio-/main/LoginapplicationWSQL.cpp){:target="_blank"}


[Narrative for enhancement 3 link](https://github.com/migrodri1980/Working-portfolio-/blob/f1a1cee7802af6796befa6c1b48e2429b44ac736/Enhancement%203%20Narrative-LoginapplicationWSQL.pdf){:target="_blank"}


This project incorporates a working knowledge of SQL databases and the commands to preform CRUD functions

Several Key elements are incorporated in this project as follows:

*The creation of a SQL data base to display and store information (customer information)

*The creation of functions to preform SQL tasks such as create, delete, update, and delete regarding user information

*The creation of a callback function to select specific items in a table based on input parameters.

*The principle of least privilege implemented in the insert, and delete functions being only accessible by the adminstrator menu

*The creation of error outputs and catches regarding SQL functions


## Additional Projects below:

### Project : RPI sensor project utilizing Python

#### [Link to project file folder](https://github.com/migrodri1980/Working-portfolio-/tree/main/RPI%20python%20sensor%20project){:target="_blank"}

#### [Link to Python code for RPI](https://github.com/migrodri1980/Working-portfolio-/blob/main/RPI%20python%20sensor%20project/msfinal.py){:target="_blank"}


[Picture of dashboard graph displayed with input readings from unit](https://raw.githubusercontent.com/migrodri1980/Working-portfolio-/main/RPI%20python%20sensor%20project/dashboardscreenshot.JPG){:target="_blank"}


This project incorporates the use of Python to analyze input sensor readings from a Raspberry Pi 4 unit and outputs to an online dashboard. 

Several key elements are incorporated into this project as follows:

*The creation of Python code to read data from the sensors attached to the RPI unit 

*The creation of functions to write output into a json file path as well as provide keyboard interrupt functionality within the system. 

*The creation of algorithms to interpret input data as well as provide calibrated tolerances allowing for required output readings from the RPI unit. 




### Project : Inventory Tracker Mobile App (Currently being developed utilizing Android Studio and Javascript)

#### [Link to project file folder](https://github.com/migrodri1980/Working-portfolio-/tree/main/miguelrodriguezinvmanager){:target="_blank"}

[Inventory application startup page pic](https://raw.githubusercontent.com/migrodri1980/Working-portfolio-/main/Project%20Pictures/inventory%20tracker%20pic.JPG){:target="_blank"}


This project incorporates a working knowledge of Android Studio with Javascript

Several key elements are incorporated in the project as follows.: 

*The creation of Javascript functions to navigate and create functionality within the application. 

*The use of SQL functions and commands to store item and customer information to facilitate login. (still being worked on)

*The use of themes and colors to create auniformity throughout the application. 























