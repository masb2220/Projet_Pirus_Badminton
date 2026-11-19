#include <Arduino.h>

//Ceci est un test de push

// put function declarations here:
int myFunction(int, int);

void setup()
{
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop()
{
  // googoo gaga
}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}

bool test_function(int allo)
{
  if (allo > 10)
  {
    return true;
  }
}