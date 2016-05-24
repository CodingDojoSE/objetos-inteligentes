#Copyright



Originally downloaded from:
https://github.com/notepad-plus-plus/notepad-plus-plus/blob/master/CONTRIBUTING.md

COPYING -- Describes the terms under which Notepad++ is distributed. You should have received a
copy of the GNU General Public License along with this file. If not, see <http://www.gnu.org/licenses/>.

IMPORTANT NOTEPAD++ LICENSE TERMS

* Copyright (C)2016 Don HO <don.h@free.fr>.
* Copyright (C)2016 Evandro Coan <evandrocoan@hotmail.com>.

This program is free software; you may redistribute and/or modify it under the terms of
the GNU General Public License as published by the Free Software Foundation; Version 2
with the clarifications and exceptions described below. This guarantees your right to use,
modify, and redistribute this software under certain conditions.

Note that we consider an application to constitute a "derivative work" for the purpose of this
license if it integrates/includes/aggregates Notepad++ into a proprietary executable installer,
such as those produced by InstallShield. Our interpretation applies only to Notepad++ - we don't
speak for other people's GPL works.

Our interpretation applies only to Notepad++ - we don't speak for other people's GPL works.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.



### Coding style
![stay clean](https://notepad-plus-plus.org/assets/images/good-bad-practice.jpg)



#### GENERAL

1. ##### Do not use Java-like braces.

  * ###### Good:
    ```cpp
    if()
    {
        // Do something
    }
    ```

  * ###### Bad:
    ```cpp
    if() {
        // Do something
    }
    ```

1. ##### Use white-spaces instead of tabs (we set our editors to 4 white-spaces for 1 tab).


1. ##### Always leave one space before and after binary and ternary operators.

  * ###### Good:
    ```cpp
    if( a == 10 && b == 42 )
    ```

  * ###### Bad:
    ```cpp
    if(a==10&&b==42)
    ```

1. ##### Only leave one space after semi-colons in "for" statements.

  * ###### Good:
    ```cpp
    for( int i = 0; i != 10; ++i )
    ```

  * ###### Bad:
    ```cpp
    for(int i=0;i<10;++i)
    ```

1. <h5>Keywords are not function calls;<br>
Function names are not separated from the first parenthesis.</h5>

  * ###### Good:
    ```cpp
    foo();
    myObject.foo( 24 );
    ```

  * ###### Bad:
    ```cpp
    foo ();
    myObject.foo ( 24 );
    ```

1. ##### Keywords are not separated from the first parenthesis by one space.

  * ###### Good:
    ```cpp
    if( true )
    while( true )
    ```

  * ###### Bad:
    ```cpp
    if ( myCondition )
    ```

1. ##### Use the following indenting for "switch" statements:

  ```cpp
  switch(test)
  {
      case 1:
      {
          // Do something
          break;
      }
      default:
      {
          // Do something else
      }
  } // No semi-colon here
  ```

1. ##### Avoid magic numbers.

  * ###### Good:
    ```cpp
    if( foo < I_CAN_PUSH_ON_THE_RED_BUTTON )
        startThermoNuclearWar();
    ```

  * ###### Bad:
    ```cpp
    while( lifeTheUniverseAndEverything != 42 )
        lifeTheUniverseAndEverything = buildMorePowerfulComputerForTheAnswer();
    ```

1. ##### Prefer enums for integer constants.

1. ##### Use initialization with curly braces.

  * ###### Good:
    ```cpp
    MyClass instance{ 10.4 };
    ```

  * ###### Bad:
    ```cpp
    MyClass instance( 10.4 );
    ```

1. ##### Always use `empty()` for testing if a string is empty or not.

  * ###### Good:
    ```cpp
    if( not string.empty() )
    ...
    ```

  * ###### Bad:
    ```cpp
    if( string != "" )
    ...
    ```



#### NAMING CONVENTIONS

1. ##### Classes (camel case)

  * ###### Good:
    ```cpp
    class IAmAClass
    {
    };
    ```

  * ###### Bad:
    ```cpp
    class iAmClass
    {
    };
    class I_am_class
    {
    };
    ```

1. <h5>methods (camel case + begins with a lower case)<br>
method parameters (camel case + begins with a lower case)</h5>

  ```cpp
  void myMethod( uint myVeryLongParameter );
  ```

1. <h5>member variables<br>
Any member variable name of class/struct should be preceded by an underscore.</h5>

  ```cpp
  public:
      int _publicAttribute;
  private:
      int _pPrivateAttribute;
      float _pAccount;
  ```

1. ##### Always prefer a variable name that describes what the variable is used for.

  * ###### Good:
    ```cpp
    if( hours < 24 && minutes < 60 && seconds < 60 )
    ```

  * ###### Bad:
    ```cpp
    if( a < 24 && b < 60 && c < 60 )
    ```



#### COMMENTS

1. ##### Use C++ comment line style than C comment style.

  * ###### Good:
    ```
    // Two lines comment
    // Use still C++ comment line style
    ```

  * ###### Bad:
    ```
    /*
    Please don't piss me off with that
    */
    ```



#### BEST PRACTICES

1. ##### Use C++11/14 whenever it is possible

1. ##### Use C++11 member initialization feature whenever it is possible

  ```cpp
  class Foo
  {
      int value = 0;
  };
  ```

1. ##### Prefer this form:
  ```cpp
  ++i
  ```

  **to:**
  ```cpp
  i++
  ```
  (It does not change anything for built-in types but it would bring consistency)

1. ##### Avoid using pointers. Prefer references. You might need the variable to be assigned a NULL
value: in this case the NULL value has semantics and must be checked. Wherever possible, use a
SmartPtr instead of old-school pointers.

1. ##### Avoid using new if you can use automatic variable. However, avoid `shared_ptr` as much as
possible. Prefer `unique_ptr` instead.

1. ##### Don't place any "using namespace" directives in headers.

1. ##### Compile time is without incidence. Increasing compile time to reduce execution time is encouraged.

1. ##### Code legibility and length is less important than easy and fast end-user experience.


