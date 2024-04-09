#CPlus

This is also used in many other functions like:
- sprintf
- sscanf
- ImGui::Text

#### Special formatting characters
%s - char array
%f - float
%d - int

%0.2f - float with 2 decimal places

#### Example usage
```c++
float someFloat = 5.0f;
printf( "%f", someFloat );  // prints "5.0000000"
printf( "%0.2f", someFloat );  // prints "5.00"

char* someCharArray = "Orange";
printf( "%s", someCharArray );

std::string someString = "Green";
printf( "%s", someString.c_str() );
```
