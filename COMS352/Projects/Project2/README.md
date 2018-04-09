This project requires that the url be provided as an argument to the httpClient

such as: 

./httpClient https://www.w3.org/TR/html4/index/list.html -h -d 1:2:15

or 
./httpClient www.w3.org/TR/html4/index/list.html -h

or
./httpClient www.w3.org:80/TR/html4/index/list.html -d 1:2:15

or 
./httpClient https://www.w3.org:80/TR/html4/index/list.html -h -d 1:2:15



if you wish to connect to localhost, start server first in a new terminal window, then use use 

./httpClient localhost/resource.ext